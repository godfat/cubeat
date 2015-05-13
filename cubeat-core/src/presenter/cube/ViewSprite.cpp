
#include "view/Sprite.hpp"
#include "view/Scene.hpp"
#include "view/SFX.hpp"

#include "view/detail/LinearParticleHack.hpp"

#include "presenter/cube/ViewSprite.hpp"

#include "data/MapSetting.hpp"
#include "data/ViewSetting.hpp"
#include "data/Color.hpp"

#include "utils/Logger.hpp"
#include "utils/Random.hpp"
#include "utils/to_s.hpp"

//#include "EventDispatcher.hpp"
//#include "ctrl/TimerDispatcher.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "Player.hpp"
#include "audio/Sound.hpp"

namespace psc{
namespace presenter{
namespace cube{

using namespace std::tr1::placeholders;

std::tr1::function<void(model::pCube, int)>
ViewSprite::Exploding = std::tr1::bind(&model::Cube::go_exploding, _1, _2);

std::tr1::function<void(model::pCube, int)>
ViewSprite::Restore   = std::tr1::bind(&model::Cube::restore, _1, _2);

std::tr1::function<void(model::pCube, int)>
ViewSprite::Broken    = std::tr1::bind(&model::Cube::be_broken, _1, _2);

ViewSprite::ViewSprite(model::pCube c, view::pObject orig, data::pMapSetting ms,
           data::pViewSetting s, ctrl::wpPlayer p):
    ViewBase(c), view_orig_(orig), cube_(c), map_setting_(ms), view_setting_(s), player_(p)
{
    body_ = view::Sprite::create("cubes/cube1", view_orig_.lock(),
                                 s->cube_size(), s->cube_size(), true);

    outline_ = view::Sprite::create("cubes/cube1-out", body_,
                                    s->cube_size(), s->cube_size(), true);
    outline_->setPickable(false);
    //outline_->tween<easing::SineCirc, accessor::Alpha>(0, 128, 500u, -1);
    outline_->set<accessor::Alpha>(128);
    outline_->set<accessor::Visible>(false);
    outline_->setDepth(-5);

    body_->set<accessor::Pos2D>(pos_vec2());
    set_base_color_and_texture(c->data()->color_id());

    //shot_event(&model::Cube::go_exploding, &model::Cube::be_broken);
    if( map_setting()->tutorial() ) {
        shot_event(&model::Cube::go_exploding);
    } else {
        shot_event(&model::Cube::go_exploding, &model::Cube::go_exploding);
    }
}

void ViewSprite::set_base_color_and_texture(int color_id){
    std::string temp = utils::to_s(utils::random(4)+1);

    body_->setTexture("cubes/cube" + temp);
    outline_->setTexture("cubes/cube" + temp + "-out");

    data::Color col = data::Color::from_id(color_id);
    col.offset();
    body_->set<accessor::ColorDiffuse>( 0xff000000 | col.rgb() );
    outline_->set<accessor::ColorDiffuse>( 0xff000000 | col.rgb() );
}

void ViewSprite::drop_a_block(){
    body_->tween<easing::Linear, accessor::Pos2D>(
        pos_vec2(), map_setting()->cube_dropping_duration());
}

void ViewSprite::sink_a_block(){
    body_->tween<easing::Linear, accessor::Pos2D>(
        pos_vec2(), map_setting()->cube_sinking_duration());
}

//    void ViewSprite::drop_a_block_with_callback(callback_type callback){
////        body_->tween<easing::Linear, accessor::Pos2D>(
////            pos_vec2(), map_setting()->cube_dropping_duration(), 0,
////            std::tr1::bind(&ViewSprite::check_if_need_callback, shared_from_this(), callback));
//
//        //hotfix: Map::next_state depends on the execution order of cubes' states in the same frame,
//        //but Irrlicht animator is not reliable (actually we should not depend on this.)
//        //so I am going to use Timer for the callback here, for now. next_state is bound to be revamped.
//        //similar problem will still happen in sink_a_block event, however that's a little harder to notice,
//        //and it won't break puzzle mode. (no one is sinking in puzzle mode.)
//
//        body_->tween<easing::Linear, accessor::Pos2D>(
//            pos_vec2(), map_setting()->cube_dropping_duration());
//        ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
//            std::tr1::bind( &ViewSprite::check_if_need_callback, this, callback),
//            std::tr1::static_pointer_cast<ViewSprite>( shared_from_this() ),
//            map_setting()->cube_dropping_duration() );
//    }

//    void ViewSprite::sink_a_block_with_callback(callback_type callback){
//        if( ctrl::pPlayer p = player_.lock() ) {
//            body_->tween( data::AnimatorParam<easing::Linear, accessor::Pos2D>().end(pos_vec2())
//                  .cb(callback).speedfunc( std::tr1::bind(&ctrl::Player::haste_speedfunc, p->shared_from_this(), map_setting()->sink_speed()) ) );
//        } else {
//            body_->tween<easing::Linear, accessor::Pos2D>(
//                pos_vec2(), map_setting()->cube_sinking_duration(), 0,
//                std::tr1::bind(&ViewSprite::check_if_need_callback, shared_from_this(), callback));
//        }
//    }

void ViewSprite::update_pos(){
    body_->set<accessor::Pos2D>(pos_vec2());
}

void ViewSprite::approach_pos(){
    vec2 pos1 = pos_vec2();
    vec2 pos2 = body_->get<accessor::Pos2D>();
    vec2 tmp_pos = (pos1-pos2)/30;
    pos2 += tmp_pos;
    body_->set<accessor::Pos2D>( pos2 );
}

static void stop_emitting_trail(irr::scene::IParticleSystemSceneNode* ps)
{
    ps->setEmitter(0);
}

static void garbage_fly_end(model::Cube* raw_cp, view::pSprite sp)
{
    raw_cp->new_garbage(false);
    sp->setDepth(0);
    sp->setPickable(true);
}

template<template <class> class Eq>
static void animating_helper(view::pSprite sp, view::pObject o, vec2 dest, vec2 dest2, unsigned int dur, std::tr1::function<void()> cb, std::tr1::function<void()> cb2)
{
    sp->tween<Eq, accessor::Pos2D>(dest, dur, 0, cb);
    data::AnimatorParam<Eq, accessor::Pos2D> move;
    o->tween<easing::Linear, accessor::Alpha>(255, 255, dur - 100, 0, cb2); // This is pure dummy animation, I just need a non-EventDispatcher related timer to fire up stop_emitting_trail
    move.end(dest).duration(dur);
    view::SFX::i().custom_effect_holder(o, move);
}

void ViewSprite::garbage_fly(){ //only called once when model::Map::insert_garbage

    using namespace irr;
    using namespace scene;

    view::pObject effect_body = view::Object::create(view_orig_.lock());
    effect_body->setPickable(false);
    vec3 tmp = body_->get<accessor::Pos3D>();
    effect_body->set<accessor::Pos3D>( vec3(tmp.X, tmp.Y, -50) );

    IParticleSystemSceneNode* ps = effect_body->scene()->addParticleNodeTo(effect_body, false);
    ps->setIsDebugObject(true); // So it can't be picked.

    vec2 origpos = view_orig_.lock()->get<accessor::Pos2D>();
    vec2 pos( view_setting()->atf_x() - origpos.X, view_setting()->atf_y() - origpos.Y );
    vec2 flying_vector = (pos_vec2() - pos);
    int flying_distance = flying_vector.getLength();
    core::vector2df normal = flying_vector.normalize();

    IParticleEmitter* em = new irr::scene::LinearParticleEmitter(
        &body_->body()->getPosition(),  /// Very fucking hacky, but Irrlicht's Particle emitter is really rigid.
        core::vector3df(0.0f, 0.0f, 0.0f),   // initial direction
#if !defined(_SHOOTING_CUBES_ANDROID_)
        flying_distance/3, flying_distance/3,    // emit rate
#else
        flying_distance/3, flying_distance/3,    // emit rate
#endif
        video::SColor(0,255,255,255),       // darkest color
        video::SColor(0,255,255,255),       // brightest color
        225, 225, 0,                         // min and max age, angle
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

    // Setup particle node above, animation below

    unsigned int dur = map_setting()->cube_dropping_duration();

    std::tr1::function<void()> cb = std::tr1::bind(&garbage_fly_end, cube_.lock().get(), body_);
    std::tr1::function<void()> cb2 = std::tr1::bind(&stop_emitting_trail, ps);

    int factor = utils::random(4)-1;
    if (factor <= 0) { factor -= 1; } //make sure no one is zero

    vec3 rot(0, 0, 360 * factor);
    body_->tween<easing::Linear, accessor::Rotation>(rot, dur);
    body_->tween<easing::OSine, accessor::Alpha>(0, 255, dur);
    body_->setDepth(-50);      // when the body_ is still flying, make sure it won't be covered by other stuff
    body_->setPickable(false); // when the body_ is still flying, make it unpickable.

    vec2 effect_dest = pos_vec2() - (normal * 20.0f);

    switch( utils::random(6) ) {
        case 0:
            animating_helper<easing::IOQuad>(body_, effect_body, pos_vec2(), effect_dest, dur, cb, cb2);
            break;
        case 1:
            animating_helper<easing::IOCubic>(body_, effect_body, pos_vec2(), effect_dest, dur, cb, cb2);
            break;
        case 2:
            animating_helper<easing::IOQuart>(body_, effect_body, pos_vec2(), effect_dest, dur, cb, cb2);
            break;
        case 3:
            animating_helper<easing::IOQuint>(body_, effect_body, pos_vec2(), effect_dest, dur, cb, cb2);
            break;
        case 4:
            animating_helper<easing::IOCirc>(body_, effect_body, pos_vec2(), effect_dest, dur, cb, cb2);
            break;
        case 5:
            animating_helper<easing::IOSine>(body_, effect_body, pos_vec2(), effect_dest, dur, cb, cb2);
            break;
        default:
            animating_helper<easing::IOSine>(body_, effect_body, pos_vec2(), effect_dest, dur, cb, cb2);
            break;
    }
}

template<class T>
static T OBack_curve(float t, T const& b, T const& c, float const& d)
{   // note: t is 0->1, and I have to speed the curve up by giving bigger t gaps
    float s = 1.70158f;
    t=(t/d)-1;
    return c*(t*t*((s+1)*t + s) + 1) + b;
}

void ViewSprite::normal_entry() { // called from model::Map::cycle_creation AND model::Cube::sink_a_frame
    //std::tr1::function<void()> cb = std::tr1::bind(&garbage_fly_end, cube_.lock().get(), body_);
    //body_->setPickable(false);
    //body_->tween<easing::OBack, accessor::Scale>(vec3(.1, .1, .1), vec3(1, 1, 1), 500u, 0, cb);

    body_->set<accessor::Pos2D>( pos_vec2() );

    double entry_progress = cube_.lock()->entry_progress();
    if( entry_progress < 64.0 ) {
        vec3 scale = OBack_curve( entry_progress, vec3(.1,.1,.1), vec3(1, 1, 1), 64.f );
        body_->set<accessor::Scale>(scale);
    } else {
        body_->set<accessor::Scale>(vec3(1, 1, 1));
    }
}

void ViewSprite::goto_garbage_orig(){ //called from presenter::Map
    vec2 origpos = view_orig_.lock()->get<accessor::Pos2D>();
    vec2 pos( view_setting()->atf_x() - origpos.X, view_setting()->atf_y() - origpos.Y );
    body_->set<accessor::Pos2D>( pos );
}

void ViewSprite::go_dying(int color_id){
    using namespace easing; using namespace accessor;
//    unsigned int duration = map_setting()->cube_dying_duration();

//    body_->tween<Linear, Alpha>(255, 0, 150u, 0, 0, duration-200);
//    body_->tween<SineCirc, GradientEmissive>(0, 128, duration-200);

//    outline_->set<Alpha>(0);

//    int csize = view_setting()->cube_size();

//    view::pSprite white_cube = view::Sprite::create("cubes/cube-white", view_orig_.lock(), csize, csize, true);
//    white_cube->setDepth(-5).set<Pos2D>( body_->get<Pos2D>() ).set<Alpha>(0).set<GradientDiffuse>(255)
//               .setPickable(false)
//               .tween<Linear, Alpha>(0, 255, 250u, 0, 0, duration-200);

//    view::SFX::i().hold(white_cube, duration+50);
//
//    view::pSprite stroke = view::Sprite::create("stroke", body_, csize, 15, true);
//    stroke->setDepth(-5)
//           .set<ColorDiffuseVec3>( body_->get<ColorDiffuseVec3>() )
//           .set<GradientEmissive>(255)
//           .tween<SineCirc, Alpha>(0, 255, duration-200)
//           .tween<Linear, Pos2D>(vec2(0, csize/2), vec2(0, -csize/2), duration-200);
//
//    view::SFX::i().hold(stroke, duration-200);

    data::Color col = data::Color::from_id(color_id);
    outline_->setTexture("cubes/cube-white").set<Visible>(true).set<Alpha>(0)
               .set<ColorDiffuse>( col.get_bright2_argb() )
               .set<Scale>( vec3(1.02, 1.02, 1) )
               .tween<SineCirc, Alpha>(0, 128, 128u, -1);
}

void ViewSprite::go_exploding(int color_id){
    body_->clearAllTween();
    body_->setPickable(false);
//    if( cube_.lock()->is_garbage() )
//        body_->setTexture( "cubes/garbage0" );

    using namespace irr;
    using namespace scene;

    for( int i = 0; i < 15; ++i ) {
        view::pObject effect_body_orig = view::Object::create(view_orig_.lock());
        view::pSprite effect_body = view::Sprite::create("smallblock", effect_body_orig, 16, 16, true);
        view::pSprite effect_body_out = view::Sprite::create("smallblock_out", effect_body, 16, 16, true);
        effect_body->setPickable(false);
        effect_body_orig->setPickable(false);
        effect_body_out->setPickable(false);

        vec3 tmp = body_->get<accessor::Pos3D>();
        //effect_body->set<accessor::Pos3D>( vec3(tmp.X, tmp.Y, -50) );
        effect_body_orig->set<accessor::Pos3D>( vec3(tmp.X, tmp.Y, -50) );
        effect_body_out->setDepth(-5);

        data::Color col = data::Color::from_id(color_id);
        col.offset();
        effect_body->set<accessor::ColorDiffuse>( col.get_bright1_argb()/*0xff553300 | col.rgb()*/ );
        effect_body_out->set<accessor::ColorDiffuse>( col.get_bright2_argb()/*0xffaa7744 | col.rgb()*/ );

        effect_body->tween<easing::IQuad, accessor::Alpha>(255, 0, 400u);
        effect_body_out->tween<easing::IQuad, accessor::Alpha>(255, 0, 400u);

        /// Old translation calculation
//        vec2 from = body_->get<accessor::Pos2D>();
//        vec2 to = from + vec2( utils::random(80) - 40, utils::random(80) - 40 );
//        effect_body->tween<easing::OExpo, accessor::Pos2D>(from, to, 333u);

        vec2 to = vec2( utils::random(25) + 25, 0 );
        effect_body->tween<easing::OExpo, accessor::Pos2D>(vec2(0,0), to, 400u);
        effect_body_orig->set<accessor::Rotation>(vec3(0, 0, utils::random(360) - 180));

        double rot1 = utils::random(180) - 90;
        double rot2 = utils::random(180) - 90;
        effect_body->tween<easing::OQuad, accessor::Rotation>(vec3(0, 0, rot1), vec3(0, 0, rot2), 400u);

        double scale_ratio = (utils::random(175) + 75) / 150.f;
        effect_body->set<accessor::Scale>(vec3( scale_ratio, scale_ratio, 1));

        view::SFX::i().hold(effect_body, 420);
        view::SFX::i().hold(effect_body_orig, 420);
        view::SFX::i().hold(effect_body_out, 420);
    }
}

void ViewSprite::ending(int time_delay){
//    body_->clearAllTween();
    body_->setPickable(false);

    bodylayer2_.reset();
    bodylayer3_.reset();

    body_->setTexture( "cubes/cube-dead-" + utils::to_s(utils::random(4)+1) );
    body_->tween<easing::OBack, accessor::Scale>(vec3(.7,.7,.7), vec3(1,1,1), 300u);
    body_->set<accessor::GradientDiffuse>(255);
}

void ViewSprite::be_broken(int color_id){
    std::string temp = utils::to_s(utils::random(4)+1);
    body_->setTexture("cubes/cube-br-" + temp);
    body_->set<accessor::GradientDiffuse>( 255 );
    outline_->setTexture("cubes/cube" + temp + "-out");
    outline_->set<accessor::GradientDiffuse>( 255 );
    body_->tween<easing::OBack, accessor::Scale>(vec3(.7,.7,.7), vec3(1,1,1), 300u);
    //shot_event(&model::Cube::restore, &model::Cube::go_exploding);
    if( map_setting()->tutorial() ) {
        shot_event(&model::Cube::restore);
    } else {
        shot_event(&model::Cube::restore, &model::Cube::restore);
    }//audio::Sound::i().playBuffer("1/d/ShotA@11.wav");

    bodylayer2_ = view::Sprite::create("smallblock", body_, 24, 24, true);
    data::Color col = data::Color::from_id(color_id);
    col.offset();
    bodylayer2_->setDepth(-5).set<accessor::ColorDiffuse>( ( col.rgb() | 0xff353520 ) - 0x00202020 );
    bodylayer2_->setPickable(false);

    bodylayer3_ = view::Sprite::create("smallblock_out", body_, 24, 24, true);
    bodylayer3_->setDepth(-7).set<accessor::ColorDiffuse>( 0xffafafaf );
    bodylayer3_->setPickable(false);
}

void ViewSprite::restore(int color_id){
    if( color_id == -1 ) return;
    set_base_color_and_texture(color_id);
    body_->tween<easing::OBack, accessor::Scale>(vec3(.7,.7,.7), vec3(1,1,1), 300u);
    //shot_event(&model::Cube::go_exploding, &model::Cube::be_broken);
    shot_event(&model::Cube::go_exploding, &model::Cube::go_exploding);

    // effects
    using namespace easing; using namespace accessor;

    bodylayer2_.reset(); // resets broken's layer2 color
    bodylayer3_.reset();

    view::pObject effect_body_orig = view::Object::create(view_orig_.lock());
    effect_body_orig->set<Pos2D>( body_->get<Pos2D>() );

    view::pSprite effect_t = view::Sprite::create("cube-br-2-t", effect_body_orig, 64, 20, true);
    view::pSprite effect_b = view::Sprite::create("cube-br-2-b", effect_body_orig, 64, 20, true);
    view::pSprite effect_l = view::Sprite::create("cube-br-2-l", effect_body_orig, 20, 64, true);
    view::pSprite effect_r = view::Sprite::create("cube-br-2-r", effect_body_orig, 20, 64, true);
    effect_body_orig->setPickable(false);
    effect_t->setDepth(-5).setPickable(false);
    effect_b->setDepth(-5).setPickable(false);
    effect_l->setDepth(-5).setPickable(false);
    effect_r->setDepth(-5).setPickable(false);

    effect_t->tween<OExpo, Pos2D>( vec2(0,-24), vec2(0,-50), 300u ).tween<Linear, Alpha>(255, 0, 300u);
    effect_b->tween<OExpo, Pos2D>( vec2(0, 24), vec2(0, 50), 300u ).tween<Linear, Alpha>(255, 0, 300u);
    effect_l->tween<OExpo, Pos2D>( vec2(-24,0), vec2(-50,0), 300u ).tween<Linear, Alpha>(255, 0, 300u);
    effect_r->tween<OExpo, Pos2D>( vec2(24, 0), vec2(50, 0), 300u ).tween<Linear, Alpha>(255, 0, 300u);

    view::SFX::i().hold(effect_body_orig, 300u);
    view::SFX::i().hold(effect_t, 300u);
    view::SFX::i().hold(effect_b, 300u);
    view::SFX::i().hold(effect_l, 300u);
    view::SFX::i().hold(effect_r, 300u);
}

void ViewSprite::be_garbage(){
    body_->setTexture("cubes/garbage3");
    shot_event(&model::Cube::go_exploding);
}

void ViewSprite::hit(int /*dmg*/, int hp){
    body_->tween<easing::OBack, accessor::Scale>(vec3(.7,.7,.7), vec3(1,1,1), 300u);
    if( cube_.lock()->is_garbage() ) {
        body_->setTexture("cubes/garbage" + utils::to_s(hp));
        audio::Sound::i().playBuffer("1/c/GlassF@11.wav");
    }
    else {
        int alpha = body_->get<accessor::Alpha>();
        body_->tween<easing::Linear, accessor::Alpha>(alpha/2 + 30, 300);
        audio::Sound::i().playBuffer("1/b/HitB@11.wav");
    }
}

void ViewSprite::get_chain(){
//    outline_->set<accessor::Alpha>(255);
//    outline_->set<accessor::Visible>(true);
}

void ViewSprite::losing_chain(double percentage){
//    outline_->set<accessor::Alpha>(percentage * 255);
}

void ViewSprite::lose_chain(){
//    outline_->set<accessor::Visible>(false);
}

ViewSprite& ViewSprite::shot_event( ShotEvent ally_cb, ShotEvent enemy_cb ) {
    using namespace std::tr1::placeholders;
    if( ctrl::pPlayer p = player_.lock() ) {
        p->subscribe_shot_event( body_,
            std::tr1::bind( &ViewSprite::check_cube_and_do, ally_cb, cube_, _1),
            std::tr1::bind( &ViewSprite::check_cube_and_do, enemy_cb, cube_, _1) );
    }
    return *this;
}

ViewSprite& ViewSprite::shot_event( ShotEvent ally_cb ) {
    using namespace std::tr1::placeholders;
    if( ctrl::pPlayer p = player_.lock() ) {
        p->subscribe_shot_event( body_,
            std::tr1::bind( &ViewSprite::check_cube_and_do, ally_cb, cube_, _1),
            0);
    }
    return *this;
}

data::pMapSetting ViewSprite::map_setting() const{ return map_setting_.lock(); }

data::pViewSetting ViewSprite::view_setting() const{ return view_setting_.lock(); }

vec2 ViewSprite::pos_vec2() const{
    data::pViewSetting s = view_setting();
    data::pCube d = data();
//    return vec2( ( d->x()*s->cube_size() + s->cube_size()/2 ),
//                 ( d->y()*s->cube_size() + s->cube_size()/2 )*-1 );
    return vec2( ( d->x()*s->cube_size() + s->cube_size()/2 ),
                 ( d->real_y()           + s->cube_size()/2 )*-1 );
                                           // NOTE: reversed y
}

void ViewSprite::check_if_need_callback(callback_type callback){
    if(model::pCube c = cube_.lock()) callback();
}

///// Class method /////

void ViewSprite::check_cube_and_do(ShotEvent method, model::wpCube c, int dmg)
{
    if(model::pCube cube = c.lock()) method(cube, dmg);
}

} //cube
} //presenter
} //psc
