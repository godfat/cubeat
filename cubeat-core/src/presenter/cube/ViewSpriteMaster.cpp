
#include "model/Chain.hpp"

#include "view/Sprite.hpp"
#include "view/Menu.hpp"
#include "view/Scene.hpp"
#include "view/AnimatedSprite.hpp"
#include "view/SpriteText.hpp"
#include "view/SFX.hpp"

#include "view/detail/LinearParticleHack.hpp"

#include "presenter/cube/ViewSprite.hpp"
#include "presenter/cube/ViewSpriteMaster.hpp"

#include "data/ViewSetting.hpp"
#include "data/Color.hpp"

#include "utils/Logger.hpp"
#include "utils/Random.hpp"
#include "utils/to_s.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "Player.hpp"
#include "Input.hpp"
#include "audio/Sound.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Conf.hpp"

#include <boost/foreach.hpp>

namespace psc{
namespace presenter{
namespace cube{

using namespace std::tr1::placeholders;

ViewSpriteMaster::ViewSpriteMaster(view::pScene scene, data::pViewSetting setting,
    ctrl::wpPlayer const& player): ViewMaster(setting),
    scene_(scene), player_(player), column_flag_(0),
    i_have_to_keep_track_of_garbage_count_visually_here_(0)
{   //temporary
    view_orig_ = view::Object::create( scene );
    view_orig_->set<accessor::Pos2D>( vec2(setting->x_offset(), setting->y_offset()) );
}

ViewBase::pointer_type ViewSpriteMaster::create(model::pCube cube) const {
    return ViewSprite::create(cube, view_orig_, map_setting(), view_setting(), player_);
}

void ViewSpriteMaster::setup_ability_button(){
    int n = player_.lock()->ability_left();
    ability_btn_ = view::Sprite::create( "cubes/garbage"+utils::to_s(n), scene_.lock(), 100, 100, true );
    ability_btn_->set<accessor::Pos2D>(vec2( view_setting()->abl_btn_x(), view_setting()->abl_btn_y() ));

    //setup button to invoke player ability
    ctrl::Button const* b = &player_.lock()->input()->trig1();
    ability_btn_->onPress(b) = std::tr1::bind(&ctrl::Player::invoke_ability, player_.lock().get(), _1);
}

void ViewSpriteMaster::column_full(int at){
    column_flag_ |= (1 << at);
    show_warning_at(at, true);
}
void ViewSpriteMaster::column_not_full(int at){
    int flag_old = column_flag_;
    if( column_flag_ & ( 1 << at ) ) {
        column_flag_ ^= (1 << at);
    }
    show_warning_at(at, false);

    if( column_flag_ == 0 && flag_old != 0 ) {
        using namespace easing; using namespace accessor;
//        box_top_  ->set<ColorDiffuseVec3>(vec3(255, 255, 255)).set<Alpha>(160);
//        box_left_ ->tween<Linear, ColorDiffuseVec3>(vec3(255, 255, 255), 800);
//        box_right_->tween<Linear, ColorDiffuseVec3>(vec3(255, 255, 255), 800);
//        box_bottom_->set<ColorDiffuseVec3>(vec3(255, 255, 255)).set<Alpha>(160);
    }
}

void ViewSpriteMaster::new_chain(model::wpChain const& chain){
    int combo = chain.lock()->step();
    int amounts = chain.lock()->last_step_amounts();
    if( amounts < 1 ) combo -= 1;

    using namespace accessor; using namespace easing;

    std::string str("Chain " + utils::to_s( combo ));

    /// Hack here
    std::string str2;
    for( int i = 1 ; i <= combo; ++i ) {
        int n = chain.lock()->step_amounts(i);
        str2 += utils::to_s(n) + ( i < combo ? "+" : "" );
    }
    str2 += " cubes";

    view::pScene s = scene_.lock();

    int y_offset = 200, x_offset = 0, w = map_setting()->width();
    if( map_setting()->width() % 2 == 0 )
        x_offset = (pos_vec2(w/2, 0).X + pos_vec2(w/2-1, 0).X) / 2;
    else x_offset = pos_vec2(w/2, 0).X;

    if( chain_texts_.find( chain ) == chain_texts_.end() ) {
        BOOST_FOREACH(ChainTextPair const& it, chain_texts_)
            y_offset += it.second->getSprite("chain").get<Size2D>().Y * 2.5;
        chain_texts_.insert( std::make_pair(chain, view::pMenu()) );
    } else y_offset = chain_texts_[chain]->get<Pos2D>().Y;

    view::pMenu m = view::Menu::create("", s, 100, 100, true);
    m->addSpriteText("chaino",  str, "GN-KillGothic", 0, 32, true, data::Color(255,0,0))
      .addSpriteText("chain",   str, "GN-KillGothic", 0, 32, true)
      .addSpriteText("amounto", str2,"GN-KillGothic", 0, 24, true, data::Color(255,0,0))
      .addSpriteText("amount",  str2,"GN-KillGothic", 0, 24, true)
      .setPickable(false);
    m->getSprite("chaino").set<Pos2D>(vec2(1,-1)).set<Scale>(vec3(1.03,1.15,1)).setPickable(false);
    m->getSprite("chain").setDepth(-10).setPickable(false);
    m->getSprite("amounto").set<Pos2D>(vec2(1,39)).set<Scale>(vec3(1.03,1.15,1)).setPickable(false);
    m->getSprite("amount").setDepth(-10).set<Pos2D>(vec2(0,40)).setPickable(false);
    m->setDepth(-100).set<Pos2D>( vec2(x_offset, y_offset) )
      .tween<OElastic, Scale>(vec3(0,0,0), vec3(1 + (0.12*combo),1 + (0.12*combo), 1), 1000u, 0,
                              std::tr1::bind(&ViewSpriteMaster::pop_a_chain_text, this, chain) );

    chain_texts_[ chain ] = m;
    audio::Sound::i().playBuffer( ("2/2b_" + utils::to_s(combo >= 7 ? 7 : combo) + ".wav"));
}

void remove_emitter_of(irr::scene::IParticleSystemSceneNode* ps) {
    ps->setEmitter(0);
}

void ViewSpriteMaster::new_chain_grouping(std::vector< std::tr1::tuple<int, int, int> > const& dying_cubes_position, int power) {
    using namespace accessor; using namespace easing;
    using std::tr1::bind; using std::tr1::get; using utils::to_s;

    view::pScene s = scene_.lock();
    int csize = view_setting()->cube_size();
    int size = dying_cubes_position.size();
//    int upmost = 0;
//    int downmost = map_setting()->height() - 2;  // these limits are setup inversely, so can be used to test the bounding area
//    int leftmost = map_setting()->width() - 1;
//    int rightmost = 0;

    for( int i = 0; i < size; ++i ) {
        int code = get<2>(dying_cubes_position[i]);
        int color_id = code / 100 % 10;
        int type = (code / 10) % 10;

        int x = get<0>(dying_cubes_position[i]);
        int y = get<1>(dying_cubes_position[i]);

        int edge_rot = (code % 10) * -90; // unit is in 90-degree: 0 means 0, 3 means 270 (clockwise)

        data::Color col = data::Color::from_id(color_id);
        col.offset();

        edges_[x][y]->clearAllTween().clearAllQueuedTween();

//        if( type > 0 && type <= 4 ) {
            edges_[x][y]->setTexture("cubes/cube-peri-"+to_s(type)).set<Visible>(true).set<Rotation>(vec3(0, 0, edge_rot))
                         .set<ColorDiffuse>( col.get_bright2_argb()/*0xffaa7744 | col.rgb()*/ )
                         .tween<Linear, Scale>(vec3(1.2, 1.2, 1.2), vec3(1,1,1), 150u)
                         .tween<SineCirc, Alpha>(255, 128, 128u, -1, 0, 150)
                         .queue<Linear, GradientEmissive>(0, 255, 150u)
                         .tween<Linear, GradientEmissive>(255, 0, 200u);

        view::pSprite glow_cube = view::Sprite::create("cubes/cube-white", view_orig_, csize, csize, true);
        glow_cube->setDepth(-10).setPickable(false).set<Pos2D>(pos_from_orig(x, y))
                  .set<ColorDiffuse>( col.get_bright2_argb() /*0xffaa7744 | col.rgb()*/ )
                  .tween<Linear, Alpha>(255, 0, 250u);

        view::SFX::i().hold(glow_cube, 250u);

        /// Note: although calling hide_edge() manually is definitely a problem, but it doesn't entail any
        ///       other bugs as it seems. So I am going to forcefully call hide_edge() from now on,
        ///       unless I noticed anything wrong.
        std::tr1::function<void()> f = bind(&ViewSpriteMaster::hide_edge, this, x, y);
        edges_[x][y]->tween<Linear, Red>(edges_[x][y]->get<Red>(), static_cast<time_t>(map_setting()->cube_dying_duration()), 0, f);

//        } else {
//            edges_[x][y]->set<Visible>(false).set<Rotation>(vec3(0,0,0));
//        }

        // resets scanline_ here:
        // scanlines_[x][y]->clearAllTween().set<Visible>(false).set<Pos2D>(pos_from_orig(x, y));

//        if( x > rightmost ) rightmost = x;
//        if( x < leftmost )  leftmost = x;
//        if( y > upmost )    upmost = y;
//        if( y < downmost )  downmost = y;
    }

    // when this chain's height span is greater than width span, scan go up
//    int fading_w = rightmost - leftmost;
//    int fading_h = upmost    - downmost;
//    bool scan_going_up = fading_h > fading_w ? true : false; // default is go right
//    int dying_duration = map_setting()->cube_dying_duration() - 150;  // WHY ? duration doesn't match???

//    for( int i = 0; i < size; ++i ) {
//        int x = get<0>(dying_cubes_position[i]);
//        int y = get<1>(dying_cubes_position[i]);
//
//        if( scan_going_up ) {
//            time_t unit_dur = dying_duration / fading_h;
//            vec2 start = pos_from_orig(x, y) + vec2(0, csize/2);
//            vec2 end   = pos_from_orig(x, y) - vec2(0, csize/2);
//            scanlines_[x][y]->set<Rotation>(vec3(0,0,0)).set<Visible>(true).set<Alpha>(0)
//                             .tween<Linear, Pos2D>(start, end, unit_dur, 0, 0, unit_dur * (y-downmost))
//                             .queue<Linear, Alpha>(255, 255, unit_dur, 0, 0, unit_dur * (y-downmost))  // This is very hacky...
//                             .tween<Linear, Alpha>(255, 0, 10u); // This is very hacky...
//        } else {
//            time_t unit_dur = dying_duration / fading_w;
//            vec2 start = pos_from_orig(x, y) - vec2(csize/2, 0);
//            vec2 end   = pos_from_orig(x, y) + vec2(csize/2, 0);
//            scanlines_[x][y]->set<Rotation>(vec3(0,0,-90)).set<Visible>(true).set<Alpha>(0)
//                             .tween<Linear, Pos2D>(start, end, unit_dur, 0, 0, unit_dur * (x-leftmost))
//                             .queue<Linear, Alpha>(255, 255, unit_dur, 0, 0, unit_dur * (x-leftmost))  // This is very hacky...
//                             .tween<Linear, Alpha>(255, 0, 10u); // This is very hacky...
//        }
//    }
}

void ViewSpriteMaster::new_garbage(std::vector< std::tr1::tuple<int, int, int> > const& dying_cubes_position, int power) {
    using namespace accessor; using namespace easing;
    using std::tr1::bind; using std::tr1::get;

    view::pScene s = scene_.lock();
    int csize = view_setting()->cube_size();
    int size = dying_cubes_position.size();

    // hide edges when new_garbage event occurs.
    for( int i = 0; i < size; ++i ) {
        int x = get<0>(dying_cubes_position[i]);
        int y = get<1>(dying_cubes_position[i]);
        edges_[x][y]->set<Visible>(false);
    }

//    if( power < 1 ) {
//        for( int i = 0; i < size; ++i ) {
//            view::pSprite glow_cube = view::Sprite::create("cubes/cube-white", s, csize, csize, true);
//            vec2 cube_pos = pos_vec2(get<0>(dying_cubes_position[i]), get<1>(dying_cubes_position[i]));
//            glow_cube->setDepth(-10).setPickable(false).set<Pos2D>(cube_pos)
//                      .tween<Linear, Alpha>(255, 0, 150u);
//            view::SFX::i().hold(glow_cube, 150u);
//        }
//        return;
//    }

    // calculate average:
//    vec2 central_pos(0, 0);
//    for( int i = 0; i < size; ++i ) {
//        central_pos += pos_vec2(get<0>(dying_cubes_position[i]), get<1>(dying_cubes_position[i]));
//    central_pos /= size;

    std::map<int, vec2> central_pos;
    std::map<int, int>  group_sizes;
    for( int i = 0; i < size; ++i ) {
        int gid = get<2>(dying_cubes_position[i]) / 1000;
        central_pos[gid] += pos_vec2(get<0>(dying_cubes_position[i]), get<1>(dying_cubes_position[i]));
        group_sizes[gid] += 1;
    }
    for( std::map<int, vec2>::iterator it = central_pos.begin(), iend = central_pos.end();
         it != iend; ++it ) {
        it->second /= group_sizes[it->first];
    }

    // "gathering" effect
//    for( int i = 0; i < size; ++i ) {
//        view::pSprite glow_cube = view::Sprite::create("cubes/cube-white", s, csize, csize, true);
//        view::pSprite glow_circle = view::Sprite::create("plight", s, csize, csize, true);
//
//        vec2 cube_pos = pos_vec2(get<0>(dying_cubes_position[i]), get<1>(dying_cubes_position[i]));
//
//        glow_cube->setDepth(-10).setPickable(false)
//                  .tween<OExpo, Pos2D>(cube_pos, central_pos, 400u)
//                  .tween<OExpo, Alpha>(255, 0, 400u);
//        glow_circle->setDepth(-10).setPickable(false)
//                    .tween<OExpo, Pos2D>(cube_pos, central_pos, 400u)
//                    .tween<Linear, Alpha>(0, 255, 400u);
//
//        view::SFX::i().hold(glow_cube, 400u).hold(glow_circle, 400u);
//    }
    int xyoff[4][2] = { {-csize/4, -csize/4}, {csize/4, -csize/4}, {csize/4, csize/4}, {-csize/4, csize/4} };

    for( int i = 0; i < size; ++i ) {
        int gid= get<2>(dying_cubes_position[i]) / 1000;
        int cx = get<0>(dying_cubes_position[i]);
        int cy = get<1>(dying_cubes_position[i]);
        vec2 cube_pos = pos_vec2(cx, cy);
        int code = get<2>(dying_cubes_position[i]);
        int color_id = code / 100 % 10;
        data::Color col = data::Color::from_id(color_id);
        col.offset();

        if( power > 1 ) {
            view::pSprite glow_circle = view::Sprite::create("plight", s, csize, csize, true);
            glow_circle->setDepth(-20).setPickable(false).set<Pos2D>(central_pos[gid])
                        .tween<Linear, Alpha>(0, 255, 200u)
                        .tween<OQuad, Scale>(vec3(0,0,0), vec3(1,1,1), 200u);

            view::SFX::i().hold(glow_circle, 200u);
        }

        for( int j = 0; j < 4; ++j ) {
            view::pObject effect_body_orig = view::Object::create(scene_.lock());
            view::pSprite effect_body = view::Sprite::create("smallblock", effect_body_orig, csize/4, csize/4, true);
            view::pSprite effect_body_out = view::Sprite::create("smallblock_out", effect_body, csize/4, csize/4, true);
            effect_body_orig->setPickable(false).set<Scale>(vec3(1.8f, 1.8f, 1));
            effect_body->setPickable(false);
            effect_body_out->setPickable(false);

            effect_body_orig->set<Pos3D>( vec3(cube_pos.X + xyoff[j][0], -cube_pos.Y + xyoff[j][1], -10) );
            effect_body_out->setDepth(-5);

            effect_body->set<ColorDiffuse>( col.get_bright1_argb() /*0xff553300 | col.rgb()*/ );
            effect_body_out->set<ColorDiffuse>( col.get_bright2_argb() /*0xffaa7744 | col.rgb()*/ );

            effect_body->tween<IQuad, GradientEmissive>(0, 255, 400u);
            effect_body_out->tween<Linear, Alpha>(255, 0, 400u);

            if( power > 1 ) {
                time_t delay_time = utils::random(200);

                vec2 selfp = effect_body_orig->get<Pos2D>();
                vec2 delp = (cube_pos - selfp) * (utils::random(50)/100.0 + 0.50);
                vec2 dest = selfp - delp + (vec2(utils::random(30)-15, utils::random(30)-15));

                effect_body_orig->tween<OQuad, Pos2D>(cube_pos, dest, 200u+ (200-delay_time), 0, 0, delay_time);
                effect_body->tween<Linear, Scale>(vec3(0,0,0), 250u+ (200-delay_time), 0, 0, delay_time);

                edges_[cx][cy]->clearAllTween().set<Visible>(true);
                edges_[cx][cy]->tween<OQuad, Scale>(vec3(1.44, 1.44, 1), 150u)
                               .tween<IExpo, GradientEmissive>(0, 255, 150u)
                               .tween<IExpo, Alpha>(255, 0, 150u);

                double rot = utils::random(720) - 360;
                effect_body->tween<OQuad, Rotation>(vec3(0, 0, rot), 400u);
            } else {
                time_t delay_time = utils::random(100);
                effect_body_orig->tween<Linear, Scale>(vec3(0,0,0), 200u + delay_time, 0, 0, delay_time);
            }

            view::SFX::i().hold(effect_body, 420);
            view::SFX::i().hold(effect_body_orig, 420);
            view::SFX::i().hold(effect_body_out, 420);
        }
    }

    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&ViewSpriteMaster::new_garbage_2ndphase, this, central_pos, power), shared_from_this(), 100);
}

void ViewSpriteMaster::new_garbage_2ndphase(std::map<int, vec2> const& pos, int new_count){

    using namespace accessor; using namespace easing; using std::tr1::bind; using utils::to_s;

    view::pScene s = scene_.lock();
    int num = new_count>20?20:new_count; //limit the animated garbage up to 20 ...in case of too many

    /// Remove the reference to Waypoint & Spline Animators.. If needed this functionality in the future,
    /// Will write new Quadratic Curve Animators.

    //2012.05 changed from throwing to off-screen, now throwing to over-head
    vec2 endp( view_setting()->ats_x(), view_setting()->ats_y() );

    for( std::map<int, vec2>::const_iterator it = pos.begin(), iend = pos.end(); it != iend ; ++it ) {

        int partial_num = num / pos.size();

        for( int i = 0; i < partial_num; ++i ) {

            using namespace irr; using namespace scene;

            view::pSprite g = view::Sprite::create("glow", s, 64, 64, true);
            g->setDepth(-50).set<Pos2D>(/*pos*/ it->second);
            g->setPickable(false);
            //g->body()->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
            vec2 midp = (/*pos*/ it->second + endp)/2;
            midp.X += utils::random(120) - 60; midp.Y += utils::random(120) - 60;

            IParticleSystemSceneNode* ps = s->addParticleNodeTo(g, false);
            ps->setIsDebugObject(true); // So it can't be picked.

            int flying_distance = (endp - /*pos*/ it->second).getLength();

            IParticleEmitter* em = new irr::scene::LinearParticleEmitter(
                &g->body()->getPosition(),  /// Very fucking hacky, but Irrlicht's Particle emitter is really rigid.
                core::vector3df(0.0f, 0.0f, 0.0f),   // initial direction
    #if !defined(_SHOOTING_CUBES_ANDROID_)
                flying_distance/6, flying_distance/6,    // emit rate
    #else
                flying_distance/6, flying_distance/6,    // emit rate
    #endif
                video::SColor(0,255,255,255),       // darkest color
                video::SColor(0,255,255,255),       // brightest color
                200, 200, 0,                         // min and max age, angle
                core::dimension2df(64.f,64.f),         // min size
                core::dimension2df(64.f,64.f)         // max size
            );

            ps->setEmitter(em); // this grabs the emitter
            em->drop(); // so we can drop it here without deleting it

            IParticleAffector* paf = ps->createFadeOutParticleAffector(video::SColor(0,0,0,0), 500);

            ps->addAffector(paf); // same goes for the affector
            paf->drop();

            ps->setPosition(core::vector3df(0,0,0));
            ps->setMaterialFlag(video::EMF_LIGHTING, true);
            ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
            ps->setMaterialTexture(0, IrrDevice::i().d()->getVideoDriver()->getTexture("rc/texture/fire.bmp"));
            ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

            /// Don't use the glowing ball effect for now:
            g->set<Alpha>(0);

            attack_cubes_.push_back(g);

            // Setup nodes above, setup animation below:
            data::AnimatorParam<Linear, Scale> scale;

            data::AnimatorParam<ISine, Pos2D> way1;
            data::AnimatorParam<IOSine, Pos2D> way2;

            /// Though all the animator life time here is 700 ms, I don't know why if I set way2's duration to 700,
            /// Some of its end callbacks won't fire. It has to be less than 700...
            /// You can imagine if some lag happens, then it will happen again. Why is it anyway??
            int total_dur = 470;
            int first_seg = 200 + utils::random(70);
            int second_seg = total_dur - first_seg;

            std::tr1::function<void()> remove_trail_emitter =
                std::tr1::bind( &remove_emitter_of, ps );
            way1.start(/*pos*/ it->second).end(midp).duration( first_seg );
            way2.start(midp).end(endp).duration( second_seg ).cb( remove_trail_emitter );

            g->queue(way1).tween(way2);
        }
    }

    // I should set the duration using config or script somehow,
    // Let's just take note that the "new_garbage" effect is supposed to cost 700ms.
    // This will probably cause more trouble when we consider rolling-back ...
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        bind(&ViewSpriteMaster::update_garbage, this, new_count), shared_from_this(), 500);
}

void ViewSpriteMaster::pop_garbage(int this_frame_lands) {

    update_garbage( - this_frame_lands ); // update first so the number is not affected by the attack_cubes' number,
                                          // attack_cubes should be retired though. in the future.

    if( static_cast<unsigned int>(this_frame_lands) >= attack_cubes_.size() ) {
        this_frame_lands = attack_cubes_.size();
    }
    for( int i = 0; i < this_frame_lands; ++i )
        attack_cubes_.pop_front();
}

void ViewSpriteMaster::hit_by_garbage(int this_frame_lands) {
    using namespace accessor; using namespace easing;

    vec2 offset(utils::random(15) + 15, utils::random(15) + 15);
    vec2 orig(view_setting()->x_offset(), view_setting()->y_offset());
    view_orig_->tween<OElastic, Pos2D>( orig + offset, orig, 500u );

    audio::Sound::i().playBuffer("tm2r_bom34.ogg");
}

// This is a private function that will be called to alter the representation of
// garbage count (might be animation or new visual effects etc)
// new_garbage and pop_garbage call to this function.
void ViewSpriteMaster::update_garbage(int delta) {

    using namespace accessor; using namespace easing; using std::tr1::bind; using utils::to_s;

    i_have_to_keep_track_of_garbage_count_visually_here_ += delta;

    vec2 rally_point( view_setting()->ats_x(), view_setting()->ats_y() );

    if( i_have_to_keep_track_of_garbage_count_visually_here_ <= 0 ) {
        i_have_to_keep_track_of_garbage_count_visually_here_ = 0;
        garbage_text_->changeText("0");
        garbage_text_outline_->changeText("0");
        garbage_text_outline_->set<Visible>(false);
        return;
    }
    else if( i_have_to_keep_track_of_garbage_count_visually_here_ <= 60 ) {
        std::string num_str = utils::to_s(i_have_to_keep_track_of_garbage_count_visually_here_);
        garbage_text_->changeText( num_str );
        garbage_text_outline_->changeText( num_str );

        // No matter what, when applying new effects on it, reset all motions and things.
        garbage_text_->set<Pos2D>( rally_point );
        garbage_text_->clearAllTween();
        garbage_text_outline_->clearAllTween();

        if( i_have_to_keep_track_of_garbage_count_visually_here_ < 20 ) {
            garbage_text_->set<ColorDiffuseVec3>(vec3(255, 255, 255));
            garbage_text_outline_->set<ColorDiffuseVec3>(vec3(255, 255, 255));
        } else if ( i_have_to_keep_track_of_garbage_count_visually_here_ < 40 ) {
            garbage_text_->set<ColorDiffuseVec3>(vec3(255, 255, 0));

            if( delta > 0 ) { /// WTF Bad state situation. We should do boolean calculation reduction here.
                vec2 offset(utils::random(15) + 15, utils::random(15) + 15);
                garbage_text_->tween<OElastic, Pos2D>(rally_point - offset, rally_point, 333u);
            }

            garbage_text_outline_->set<ColorDiffuseVec3>(vec3(255, 255, 0));
        } else {
            //garbage_text_->set<ColorDiffuseVec3>(vec3(255, 32, 0));

            if( delta > 0 ) { /// WTF Bad state situation. We should do boolean calculation reduction here.
                vec2 offset(utils::random(25) + 25, utils::random(25) + 25);
                garbage_text_->tween<OElastic, Pos2D>(rally_point - offset, rally_point, 333u);
            }

            // The middle ranged red will not be noticed if the background is middle-ranged gray, so just glow here.
            /// WTF NOTE: There is a bug if you call to this tween continuously, it will be freezed in the "starting color" somehow.
            /// I think it is SpriteText related bug.
            garbage_text_->set<ColorDiffuseVec3>(vec3(255, 255, 0));
            garbage_text_->tween<SineCirc, ColorDiffuseVec3>(vec3(255, 32, 0), vec3(255, 255, 255), 1000u, -1);
            garbage_text_outline_->set<ColorDiffuseVec3>(vec3(255, 128, 64));
        }
    } else if( i_have_to_keep_track_of_garbage_count_visually_here_ <= 99 ) {
        garbage_text_->changeText("??");
        garbage_text_outline_->changeText("??");

        if( delta > 0 ) { /// WTF Bad state situation. We should do boolean calculation reduction here.
            vec2 offset(utils::random(25) + 25, utils::random(25) + 25);
            garbage_text_->tween<OElastic, Pos2D>(rally_point - offset, rally_point, 333u);
        }

        garbage_text_->tween<SineCirc, ColorDiffuseVec3>(vec3(255, 255, 255), vec3(255, 32, 0), 500u, -1);
        garbage_text_outline_->set<ColorDiffuseVec3>(vec3(255, 128, 64));
    } else {
        garbage_text_->changeText("!!!");
        garbage_text_outline_->changeText("!!!");
    }

    if( delta > 0 ) { /// WTF Bad state situation. We should do boolean calculation reduction here.
        garbage_text_outline_->tween<Linear, Scale>(vec3(1.3, 1.3, 1), vec3(2.2, 2.2, 1), 500u);
        garbage_text_outline_->tween<Linear, Alpha>(255, 0, 500u);
        garbage_text_outline_->set<Visible>(true);
    } else {
        garbage_text_outline_->set<Alpha>(0);
    }
}

void ViewSpriteMaster::warning_sound(int warning_level){
    using namespace accessor; using namespace easing;
    audio::Sound::i().playBuffer("3/3d/alarm.wav");

    time_t warning_gap = map_setting()->warning_gap();

//    box_top_  ->set<ColorDiffuseVec3>(vec3(255, 32, 32)).set<Alpha>(224);
//    box_left_ ->tween<Linear, ColorDiffuseVec3>(vec3(255, 32, 32), 800).set<Alpha>(160);
//               .tween<SineCirc, ColorDiffuseVec3>(vec3(255, 255, 255), vec3(255, 32, 32), warning_gap);
//    box_right_->tween<Linear, ColorDiffuseVec3>(vec3(255, 32, 32), 800).set<Alpha>(160);
//               .tween<SineCirc, ColorDiffuseVec3>(vec3(255, 255, 255), vec3(255, 32, 32), warning_gap);
//    box_bottom_->set<ColorDiffuseVec3>(vec3(255, 32, 32)).set<Alpha>(224);

    // determine currently how many column are full:
    int column_count = 0;
    for( int x = 0; x < map_setting()->width(); ++x ) {
        if( column_flag_ & ( 1 << x ) )
            column_count += 1;
    }

//    for( int x = 0; x < map_setting()->width(); ++x ) {
//        warning_strip_[x]->set<Alpha>(0);
//        warning_strip_[x]->tween<SineCirc, Alpha>(0, 224 - column_count*12 , warning_gap);
//    }

    int csize = view_setting()->cube_size();
    int h = map_setting()->height()-1;

    for( int x = 0; x < map_setting()->width(); ++x ) {
        vec2 pos = vec2(0, -(h-0.33) * csize);
        warning_strip2_[x]->playAnime("moving", warning_gap/2, 1);
        warning_strip2_[x]->tween<SineCirc, Pos2D>(pos, pos + vec2(0, 15), warning_gap/2, 1);

        pos = vec2(0, csize*0.66);
        warning_strip3_[x]->playAnime("moving", warning_gap/2, 1);
        warning_strip3_[x]->tween<SineCirc, Pos2D>(pos, pos - vec2(0, 15), warning_gap/2, 1);
    }
}

void ViewSpriteMaster::alert_bar_animate(int warning_level){
    using namespace accessor; using namespace easing;

    time_t warning_gap = map_setting()->warning_gap();
    alert_bar_top_->playAnime("moving", warning_gap);
    //alert_bar_top_->tween<SineCirc, ColorDiffuseVec3>(vec3(255, 255, 255), vec3(255, 0, 0), warning_gap);
    alert_bar_bottom_->playAnime("moving", warning_gap);
    //alert_bar_bottom_->tween<SineCirc, ColorDiffuseVec3>(vec3(255, 255, 255), vec3(255, 0, 0), warning_gap);
//    alert_bar_cover_top_->set<Visible>(true).set<GradientDiffuse>(255).tween<SineCirc, Alpha>(0, 128, warning_gap);
//    alert_bar_cover_bottom_->set<Visible>(true).set<GradientDiffuse>(255).tween<SineCirc, Alpha>(0, 128, warning_gap);
    alert_text1_->playAnime("moving", warning_gap);
    alert_text2_->playAnime("moving", warning_gap);

    view::pSprite alert_text1_out1 = view::Sprite::create("alert_text/moving/0", alert_text1_, 18, 192, true);
    view::pSprite alert_text1_out2 = view::Sprite::create("alert_text/moving/0", alert_text1_, 18, 192, true);
    view::pSprite alert_text2_out1 = view::Sprite::create("alert_text/moving/0", alert_text2_, 18, 192, true);
    view::pSprite alert_text2_out2 = view::Sprite::create("alert_text/moving/0", alert_text2_, 18, 192, true);
    alert_text1_out1->setDepth(-5).set<Alpha>(0).set<Pos2D>(vec2(9, 96));
    alert_text1_out2->setDepth(-5).set<Alpha>(0).set<Pos2D>(vec2(9, 96));
    alert_text2_out1->setDepth(-5).set<Alpha>(0).set<Pos2D>(vec2(9, 96));
    alert_text2_out2->setDepth(-5).set<Alpha>(0).set<Pos2D>(vec2(9, 96));

    data::AnimatorParam<Linear, Alpha> alpha, alpha2;
    data::AnimatorParam<Linear, Scale> scale, scale2;
    alpha.start(255).end(0).duration(500).delay();
    scale.start(vec3(1,1,1)).end(vec3(2.0, 1.2, 1)).duration(500).delay();
    alpha2 = alpha; alpha2.delay(250);
    scale2 = scale; scale2.delay(250);

    alert_text1_out1->tween(alpha).tween(scale);
    alert_text1_out2->tween(alpha2).tween(scale2);
    alert_text2_out1->tween(alpha).tween(scale);
    alert_text2_out2->tween(alpha2).tween(scale2);

    view::SFX::i().hold(alert_text1_out1, 500);
    view::SFX::i().hold(alert_text1_out2, 500 + 250);
    view::SFX::i().hold(alert_text2_out1, 500);
    view::SFX::i().hold(alert_text2_out2, 500 + 250);
}

void ViewSpriteMaster::alert_bar_freeze(bool freezed){
    using namespace accessor;
    alert_bar_top_->set<ColorDiffuseVec3>(vec3(255, 255, 255));
    alert_bar_bottom_->set<ColorDiffuseVec3>(vec3(255, 255, 255));
    if( freezed ) {
        alert_bar_top_->clearAllTween();
        alert_bar_top_->setTexture("alert/moving/1");
        alert_bar_bottom_->clearAllTween();
        alert_bar_bottom_->setTexture("alert/moving/1");
        alert_bar_cover_top_->set<Visible>(true).set<ColorDiffuseVec3>(vec3(0,255,255)).set<Alpha>(128);
        alert_bar_cover_bottom_->set<Visible>(true).set<ColorDiffuseVec3>(vec3(0,255,255)).set<Alpha>(128);
    } else {
        alert_bar_cover_top_->set<Visible>(false);
        alert_bar_cover_bottom_->set<Visible>(false);
    }
}

void ViewSpriteMaster::alert_bar_update(int warning_level){
    using namespace accessor;
    if( warning_level < 1 ) {
        alert_bar_top_->set< Visible >( false );
        alert_bar_top_->set< ScaleWithUV >( vec2(1, 1) );
        alert_bar_bottom_->set< Visible >( false );
        alert_bar_bottom_->set< ScaleWithUV >( vec2(1, 1) );
        alert_bar_cover_top_->set< Visible >( false );
        alert_bar_cover_top_->set< ScaleWithUV >( vec2(0.0001, 1) );
        alert_bar_cover_bottom_->set< Visible >( false );
        alert_bar_cover_bottom_->set< ScaleWithUV >( vec2(0.0001, 1) );
        alert_text1_->set< Visible >( false );
        alert_text2_->set< Visible >( false );
        alert_leading_bg_->set< Visible >( false );
    } else {
        alert_bar_top_->set< ScaleWithUV >( vec2((warning_level)/100.0, 1) );
        alert_bar_top_->set< Visible >( true );
        alert_bar_bottom_->set< ScaleWithUV >( vec2((warning_level)/100.0, 1) );
        alert_bar_bottom_->set< Visible >( true );
        // The visibility of cover bar is decided by freeze function.
        alert_bar_cover_top_->set< ScaleWithUV >( vec2((warning_level)/100.0, 1) );
        alert_bar_cover_bottom_->set< ScaleWithUV >( vec2((warning_level)/100.0, 1) );
        alert_text1_->set< Visible >(true);
        alert_text2_->set< Visible >(true);

        alert_leading_orig1_->set< Pos2D >( vec2((warning_level)/100.0 * 640, 36) );
        alert_leading_orig2_->set< Pos2D >( vec2((warning_level)/100.0 * 640, -2) );

        alert_leading_bg_->set< Visible >(true);
        alert_leading_bg_->set< Pos2D >( -vec2(0, (warning_level)/100.0 * 640) );

        /// alert_leading "Grow" hack
        if( warning_level <= 27 ) {
            alert_leading_orig1_->set<Scale>( vec3(1, 1, 1) );
            alert_leading_orig2_->set<Scale>( vec3(1, 1, 1) );
        } else if( warning_level > 27 && warning_level <= 30 ) {
            alert_leading_orig1_->set<Scale>( vec3(1, 1.1, 1) );
            alert_leading_orig2_->set<Scale>( vec3(1, 1.1, 1) );
        } else if ( warning_level > 30 && warning_level <= 65 ) {
            alert_leading_orig1_->set<Scale>( vec3(1, 1.2, 1) );
            alert_leading_orig2_->set<Scale>( vec3(1, 1.2, 1) );
        } else if ( warning_level > 65 && warning_level <= 68 ) {
            alert_leading_orig1_->set<Scale>( vec3(1, 1.35, 1) );
            alert_leading_orig2_->set<Scale>( vec3(1, 1.35, 1) );
            alert_leading_orig2_->set< Pos2D >( vec2((warning_level)/100.0 * 640 - 0, -3) );
        } else if ( warning_level > 68 ) {
            alert_leading_orig1_->set<Scale>( vec3(1, 1.5, 1) );
            alert_leading_orig2_->set<Scale>( vec3(1, 1.5, 1) );
            alert_leading_orig2_->set< Pos2D >( vec2((warning_level)/100.0 * 640 - 0, -3) );
        }
    }
}

void ViewSpriteMaster::show_overheat(bool show){
    using namespace accessor;
    //audio::Sound::i().play("overheat_sound");
    overheat_->set<Visible>(show);
    overheat_bg_->set<Visible>(show);
}

void ViewSpriteMaster::ending_effect(){
    int duration = 800;
    alert_bar_top_->playAnime("moving", duration/4, 3);
    alert_bar_bottom_->playAnime("moving", duration/4, 3);
    alert_text1_->playAnime("moving", duration/4, 3);
    alert_text2_->playAnime("moving", duration/4, 3);
}

void ViewSpriteMaster::create_warning_strips(){
    using namespace accessor; using namespace easing;
    view::pScene scene = scene_.lock();
    for( int i=0, width=map_setting()->width(),
                  h=map_setting()->height()-1; i<width; ++i )
    {
        int csize = view_setting()->cube_size();
        view::pSprite temp = view::Sprite::create("warning", scene, csize, csize*h, true);
        vec2 pos;
        if( h % 2 == 0 )
            pos = (pos_vec2(i, h/2) + pos_vec2(i, h/2-1)) / 2;
        else pos = pos_vec2(i, h/2);
        temp->setDepth(-50).set<Pos2D>( vec2( 4000, pos.Y ) ).setPickable(false)
             .set<ColorDiffuseVec3>(vec3(255,255,255))
             .set<Alpha>(0).set<Visible>(/*false*/true);
        warning_strip_.push_back( temp );
    }
}

void ViewSpriteMaster::create_warning_strips2(){
    using namespace accessor; using namespace easing;
    view::pScene scene = scene_.lock();
    for( int i=0, width=map_setting()->width(),
                  h=map_setting()->height()-1; i<width; ++i )
    {
        int csize = view_setting()->cube_size();

        view::pObject temp0 = view::Object::create(scene);
        vec2 pos_holder = pos_vec2(i, 0);

        temp0->set<Pos2D>( vec2( 4000, pos_holder.Y ) ).setPickable(false).set<Visible>(true);
        warning_strip_holder_.push_back( temp0 );

        view::pAnimatedSprite temp = view::AnimatedSprite::create("red_tri", temp0, 128, 128, true);
        vec2 pos = vec2(0, -(h-0.33) * csize);

        temp->playAnime("moving", 1000).setDepth(-100).set<Pos2D>( pos )
             .setPickable(false).set<Visible>(true);
        warning_strip2_.push_back( temp );

        view::pAnimatedSprite temp2 = view::AnimatedSprite::create("red_tri", temp0, 128, 128, true);
        pos = vec2(0, csize*0.66);

        temp2->playAnime("moving", 1000).setDepth(-100).set<Pos2D>( pos )
              .set<Rotation>(vec3(0,0,180)).setPickable(false).set<Visible>(true);
        warning_strip3_.push_back( temp2 );
    }
}

void ViewSpriteMaster::create_overheat_overlay(){
    using namespace accessor; using namespace easing;
    int w = map_setting()->width();
    int h = map_setting()->height() - 1;
    int csize = view_setting()->cube_size();
    vec2 pos;
    if( w % 2 == 0 )
        pos = (pos_vec2(w/2, h/2) + pos_vec2(w/2-1, h/2)) / 2;
    else pos = pos_vec2(w/2, h/2); //memo: can we try to reuse this ..?
    if( h % 2 == 0 ) pos.Y += 32;

    overheat_ = view::Sprite::create("ui/overheat3", scene_.lock(), 270, 75, true);
    overheat_->setDepth(-60).set<Pos2D>( pos-vec2(0, 64) ).setPickable(false);
    overheat_->set<Visible>(false);
    overheat_->tween<SineCirc, Alpha>(0, 1000u, -1);

    overheat_bg_ = view::Sprite::create("overheat_bg", scene_.lock(), csize*w, csize*h, true);
    overheat_bg_->setDepth(-50).set<Pos2D>( pos ).setPickable(false);
    overheat_bg_->set<ColorDiffuseVec3>(vec3(0,0,0)).set<Alpha>(128).set<Visible>(false);
}

void ViewSpriteMaster::create_edges(){
    using namespace accessor;
    int w = map_setting()->width();
    int h = map_setting()->height() - 1;
    int csize = view_setting()->cube_size();

    for( int x = 0; x < w; ++x ) {
        std::vector< view::pSprite > col_edge;
        std::vector< view::pSprite > col_scanline;
        for( int y = 0; y < h; ++y ) {
            view::pSprite e  = view::Sprite::create("cubes/cube-peri-1", view_orig_, csize, csize, true);
            view::pSprite sl = view::Sprite::create("stroke0", view_orig_, csize, csize/3, true);
            vec2 pos = pos_from_orig(x, y);

            e->setDepth(-20).setPickable(false).set<Pos2D>(pos).set<Visible>(false);
            sl->setDepth(-10).setPickable(false).set<Pos2D>(pos).set<Visible>(false);
            col_edge.push_back(e);
            col_scanline.push_back(sl);
        }
        edges_.push_back(col_edge);
        scanlines_.push_back(col_scanline);
    }
}

void ViewSpriteMaster::hide_edge(int x, int y) {
    edges_[x][y]->set<accessor::Visible>(false);
}

void ViewSpriteMaster::derived_init(){
    using namespace accessor;
    int w = map_setting()->width();
    int h = map_setting()->height() - 1;
    vec2 pos, pos2;
    if( w % 2 == 0 )
        pos = (pos_vec2(w/2, h) + pos_vec2(w/2-1, h)) / 2;
    else pos = pos_vec2(w/2, h); //memo: can we try to reuse this ..?
    pos.Y += 10;

    int csize = view_setting()->cube_size();
    //UI base, note, these position follows view_orig_ like cubes, not UI texts:
//    box_bottom_ = view::Sprite::create("ui/warning_cap", view_orig_, csize*w, 42, false);
//    box_bottom_->set<Alpha>(160);
//    box_top_    = view::Sprite::create("ui/warning_cap", view_orig_, csize*w, 42, false);
//    box_top_->set<Pos2D>( vec2(0, -view_setting()->y_offset()) ).set<Alpha>(160).setDepth(30);
    box_bottom_ = view::Sprite::create("blankstrip", view_orig_, csize*w, 10, false);
    box_bottom_->set<Alpha>(160);
    box_top_    = view::Sprite::create("blankstrip", view_orig_, csize*w, 10, false);
    box_top_->set<Pos2D>( vec2(0, -view_setting()->y_offset() + 32) ).set<Alpha>(160).setDepth(30);
    box_left_   = view::Sprite::create("danger_meter", view_orig_, 46, 660, false);
    box_left_->set<Pos2D>( vec2(-46, -view_setting()->y_offset() + 32) ).set<Alpha>(160).setDepth(30);
    box_right_  = view::Sprite::create("danger_meter", view_orig_, 46, 660, false);
    box_right_->textureFlipH().set<Pos2D>( vec2(csize*w, -view_setting()->y_offset() + 32) ).set<Alpha>(160).setDepth(30);
    box_bg_     = view::Sprite::create("blocker", view_orig_, csize*w, csize*h, false);
    box_bg_->set<Pos2D>( vec2(0, -csize*h) ).set<GradientDiffuse>(0).set<Alpha>(160).setDepth(30).setPickable(false);

    //warning: the position and scale data here should be configurable.
    alert_bar_top_ = view::AnimatedSprite::create("alert", scene_.lock(), csize*h, 34, false);
    alert_bar_top_->playAnime("moving", 1000).setDepth(3).set<Pos2D>( pos_vec2(0, 0) + vec2(-72, 32) )
                   .set<Visible>(false).setPickable(false)
                   .set<Rotation>(vec3(0,0,90));
    alert_bar_cover_top_ = view::Sprite::create("shade", scene_.lock(), csize*h, 33, false);
    alert_bar_cover_top_->setDepth(-5).set<Pos2D>( pos_vec2(0, 0) + vec2(-71, 32) ).set<ColorDiffuseVec3>(vec3(255, 255, 255))
                         .set<Alpha>(128).set<ScaleWithUV>( vec2(0.0001,1) ).set<Visible>(false).setPickable(false)
                         .set<Rotation>(vec3(0,0,90));

    pos2 = pos; pos2.Y = 704;

    alert_bar_bottom_ = view::AnimatedSprite::create("alert", scene_.lock(), csize*h, 34, false);
    alert_bar_bottom_->playAnime("moving", 1000).textureFlipV().setDepth(3).set<Pos2D>( pos_vec2(w, 0) + vec2(-26, 32) )
                      .set<Visible>(false).setPickable(false)
                      .set<Rotation>(vec3(0,0,90));
    alert_bar_cover_bottom_ = view::Sprite::create("shade", scene_.lock(), csize*h, 33, false);
    alert_bar_cover_bottom_->textureFlipV().setDepth(-5).set<Pos2D>( pos_vec2(w, 0) + vec2(-26, 32) )
                            .set<ColorDiffuseVec3>(vec3(255, 255, 255)).set<Alpha>(128)
                            .set<ScaleWithUV>( vec2(0.0001,1) ).set<Visible>(false).setPickable(false)
                            .set<Rotation>(vec3(0,0,90));

    alert_text_bg1_ = view::Sprite::create("alert_text/moving/0", scene_.lock(), 18, 192, false);
    alert_text_bg1_->setDepth(-3).set<Pos2D>( pos_vec2(0, h) + vec2(-63, 48) ).set<Alpha>(64);

    alert_text_bg2_ = view::Sprite::create("alert_text/moving/0", scene_.lock(), 18, 192, false);
    alert_text_bg2_->setDepth(-3).set<Pos2D>( pos_vec2(w, h) + vec2(-18, 48) ).set<Alpha>(64);

    alert_text1_ = view::AnimatedSprite::create("alert_text", scene_.lock(), 18, 192, false);
    alert_text1_->playAnime("moving", 1000).setDepth(-10).set<Pos2D>( pos_vec2(0, h) + vec2(-63, 48) ).set<Visible>(false);

    alert_text2_ = view::AnimatedSprite::create("alert_text", scene_.lock(), 18, 192, false);
    alert_text2_->playAnime("moving", 1000).setDepth(-10).set<Pos2D>( pos_vec2(w, h) + vec2(-18, 48) ).set<Visible>(false);

    alert_leading_orig1_ = view::Object::create(alert_bar_top_);
    alert_leading_orig1_->set<Pos2D>( vec2(0, 36) );

    alert_leading_orig2_ = view::Object::create(alert_bar_bottom_);
    alert_leading_orig2_->set<Pos2D>( vec2(0, -2) );

    alert_leading1_ = view::Sprite::create("stroke2", alert_leading_orig1_, 24, 16, false);
    alert_leading1_->set<Pos2D>(vec2(0, -25)).set<Rotation>(vec3(0,0,-90));

    alert_leading2_ = view::Sprite::create("stroke3", alert_leading_orig2_, 24, 16, false);
    alert_leading2_->set<Pos2D>(vec2(0, 2)).set<Rotation>(vec3(0,0,-90));

    alert_leading_bg_ = view::Sprite::create("stroke0", view_orig_, csize*w, 16, false);
    alert_leading_bg_->setDepth(25).set<Visible>(false); //background

    create_overheat_overlay();
    create_warning_strips2();
    create_edges();

    garbage_text_ = view::SpriteText::create("0", scene_.lock(), "kimberley", 40, true);
    vec2 rally_point( view_setting()->ats_x(), view_setting()->ats_y() );
    garbage_text_->set<Pos2D>( rally_point );
    garbage_text_->set<Scale>( vec3(1.3, 1.3, 1) );

    garbage_text_outline_ = view::SpriteText::create("0", scene_.lock(), "kimberley", 40, true);
    garbage_text_outline_->set<Pos2D>( rally_point );
    garbage_text_outline_->set<Scale>( vec3(1.3, 1.3, 1) );
    garbage_text_outline_->set<Alpha>(0);
}

void ViewSpriteMaster::show_warning_at(int x, bool visible){
    if( !map_setting()->dropping_creatable() ) visible = false;
    using namespace accessor;
    // Invisible item will not be animated by Irrlicht. Damn it.. there should've been a switch to choose.

    if( visible ) {
//        vec2 pos = warning_strip_[x]->get<Pos2D>();
//        warning_strip_[x]->set<Pos2D>( vec2( pos_vec2(x, 0).X, pos.Y ) );  // we only want to ref X here

        vec2 pos = pos_vec2(x, 0);
        warning_strip_holder_[x]->set<Pos2D>( pos ); // we only want to ref X here
    } else {
//        vec2 pos = warning_strip_[x]->get<Pos2D>();
//        warning_strip_[x]->set<Pos2D>( vec2( 4000, pos.Y ) );  // move it out of screen horizontally

        vec2 pos = pos_vec2(x, 0);
        warning_strip_holder_[x]->set<Pos2D>( vec2( 4000, pos.Y ) ); // move it out of screen horizontally
    }
}

void ViewSpriteMaster::pop_a_chain_text(model::wpChain const& key) {
    chain_texts_to_be_deleted_.push_back( key );
}

void ViewSpriteMaster::cleanup_chaintext(){
    //delete chain_texts here
    BOOST_FOREACH(model::wpChain const& t, chain_texts_to_be_deleted_){
        chain_texts_.erase(t);
    }
    chain_texts_to_be_deleted_.clear();
}

vec2 ViewSpriteMaster::garbage_endpoint_vec2() const {
    return pos_vec2(map_setting()->width()/2, map_setting()->height()+2); //endpoint is out of bound
}

void ViewSpriteMaster::ability_button(int left) {
    using namespace accessor; using namespace easing;
    ability_btn_->setTexture("cubes/garbage" + utils::to_s(left) );
    ability_btn_->tween<OElastic, Scale>(vec3(.3,.3,1), vec3(1, 1, 1), 1000u);
}

vec2 ViewSpriteMaster::pos_vec2(int const& x, int const& y) const{
    return pos_vec2(view_setting(), x, y);
}

vec2 ViewSpriteMaster::pos_from_orig(int const& x, int const& y) const{
    return vec2( x*view_setting()->cube_size() + view_setting()->cube_size()/2,
                (y*view_setting()->cube_size() + view_setting()->cube_size()/2)*-1);
}

void ViewSpriteMaster::cycle(Map const& map) {
    cleanup_chaintext();
}

///// Class Method /////

vec2 ViewSpriteMaster::pos_vec2(data::pViewSetting const& setting, int const& x, int const& y){
    return vec2( setting->x_offset() + x*setting->cube_size() + setting->cube_size()/2,
                 setting->y_offset() +(y*setting->cube_size() + setting->cube_size()/2)*-1 );
                 //note: y reversed
}

} //cube
} //presenter
} //psc

