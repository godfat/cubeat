
#include "model/Chain.hpp"

#include "view/Sprite.hpp"
#include "view/Menu.hpp"
#include "view/Scene.hpp"
#include "view/AnimatedSprite.hpp"
#include "view/SpriteText.hpp"
#include "view/SFX.hpp"

#include "presenter/cube/ViewSprite.hpp"
#include "presenter/cube/viewSpriteMaster.hpp"

#include "data/ViewSetting.hpp"
#include "data/Color.hpp"

#include "utils/Random.hpp"
#include "utils/to_s.hpp"
#include "utils/Logger.hpp"

#include "EventDispatcher.hpp"

#include "Accessors.hpp"
#include "EasingEquations.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "audio/Sound.hpp"

#include <boost/foreach.hpp>

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

    body_->set<accessor::Pos2D>(pos_vec2());
    body_->setTexture("cubes/cube" + utils::to_s(utils::random(4)+1));
    data::Color col = data::Color::from_id(c->data()->color_id());
    col.offset();
    body_->set<accessor::ColorDiffuse>( 0xff000000 | col.rgb() );
    shot_event(&model::Cube::go_exploding, &model::Cube::be_broken);
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

void ViewSprite::garbage_fly(){ //only called once when model::Map::insert_garbage
    std::tr1::function<void()> cb = std::tr1::bind(&model::Cube::new_garbage, cube_.lock().get(), false);
    int factor = utils::random(4)-1;
    int dur = map_setting()->cube_dropping_duration();
    if (factor <= 0) { factor -= 1; } //make sure no one is zero
    vec3 rot(0, 0, 360 * factor);
    body_->tween<easing::Linear, accessor::Rotation>(rot, dur);
    switch( utils::random(6) ) {
        case 0:
            body_->tween<easing::IOQuad, accessor::Pos2D>(pos_vec2(), dur, 0, cb);
            break;
        case 1:
            body_->tween<easing::IOCubic, accessor::Pos2D>(pos_vec2(), dur, 0, cb);
            break;
        case 2:
            body_->tween<easing::IOQuart, accessor::Pos2D>(pos_vec2(), dur, 0, cb);
            break;
        case 3:
            body_->tween<easing::IOExpo, accessor::Pos2D>(pos_vec2(), dur, 0, cb);
            break;
        case 4:
            body_->tween<easing::IOCirc, accessor::Pos2D>(pos_vec2(), dur, 0, cb);
            break;
        case 5:
            body_->tween<easing::IOSine, accessor::Pos2D>(pos_vec2(), dur, 0, cb);
            break;
        default:
            body_->tween<easing::IOSine, accessor::Pos2D>(pos_vec2(), dur, 0, cb);
            break;
    }
}

void ViewSprite::goto_garbage_orig(){ //called from presenter::Map
    vec2 origpos = view_orig_.lock()->get<accessor::Pos2D>();
    vec2 pos( view_setting()->atf_x() - origpos.X, view_setting()->atf_y() - origpos.Y );
    body_->set<accessor::Pos2D>( pos );
}

void ViewSprite::go_dying(){
    unsigned int duration = map_setting()->cube_dying_duration();
    body_->tween<easing::Linear, accessor::GradientEmissive>(128, duration);
    body_->tween<easing::Linear, accessor::Alpha>(0, duration);
    body_->tween<easing::Linear, accessor::Rotation>(vec3(0,0,0), vec3(0,0,180), duration);
    body_->tween<easing::IQuad, accessor::Scale>(vec3(0,0,0), duration);
}

void ViewSprite::go_exploding(){
    body_->clearAllTween();
    body_->setPickable(false);
    if( cube_.lock()->is_garbage() )
        body_->setTexture( "cubes/garbage0" );

    double csize = view_setting_.lock()->cube_size();
    view::pSprite fx_body = view::Sprite::create(body_->body()->getName(), view_orig_.lock(), csize, csize, true);
    fx_body->setPickable(false);
    fx_body->set<accessor::Pos2D>( body_->get<accessor::Pos2D>() );
    fx_body->set<accessor::ColorDiffuse>( body_->get<accessor::ColorDiffuse>() );
    view::SFX::i().cube_explode(fx_body);
}

void ViewSprite::be_broken(){
    body_->setTexture("cubes/cube" + utils::to_s(utils::random(4)+1));
    body_->set<accessor::GradientDiffuse>( 255 );
    body_->tween<easing::OBack, accessor::Scale>(vec3(.7,.7,.7), vec3(1,1,1), 300u);
    shot_event(&model::Cube::restore, &model::Cube::go_exploding);
    //audio::Sound::i().playBuffer("1/d/ShotA@11.wav");
}

void ViewSprite::restore(int color_id){
    if( color_id == -1 ) return;
    body_->setTexture("cubes/cube" + utils::to_s(utils::random(4)+1));
    data::Color col = data::Color::from_id(color_id);
    col.offset();
    body_->set<accessor::ColorDiffuse>( 0xff000000 | col.rgb() );
    body_->tween<easing::OBack, accessor::Scale>(vec3(.7,.7,.7), vec3(1,1,1), 300u);
    shot_event(&model::Cube::go_exploding, &model::Cube::be_broken);
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
    if( !cube_.lock()->is_garbage() && !cube_.lock()->is_broken() ) {
        body_->tween<easing::SineCirc, accessor::GradientEmissive>(100, 500u, -1);
    }
}

void ViewSprite::lose_chain(){
    body_->tween<easing::Linear, accessor::GradientEmissive>(0, 100u);
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

///////////////////////////////////////////////////////////////////////////////

ViewSpriteMaster::ViewSpriteMaster(view::pScene scene, data::pViewSetting setting,
    ctrl::wpPlayer const& player): ViewMaster(setting),
    scene_(scene), player_(player)
{   //temporary
    view_orig_ = view::Object::create( scene );
    view_orig_->set<accessor::Pos2D>( vec2(setting->x_offset(), setting->y_offset()) );

    //moved from ViewSpriteMaster::create to here
//    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
//        std::tr1::bind(&ViewSpriteMaster::cleanup_chaintext, this), result, 10, -1);
}

ViewBase::pointer_type ViewSpriteMaster::create(model::pCube cube) const {
    return ViewSprite::create(cube, view_orig_, map_setting(), view_setting(), player_);
}

void ViewSpriteMaster::column_full(int at){
    show_warning_at(at, true);
}
void ViewSpriteMaster::column_not_full(int at){
    show_warning_at(at, false);
}

void ViewSpriteMaster::new_chain(model::wpChain const& chain){
    int combo = chain.lock()->step();
    int amounts = chain.lock()->last_step_amounts();

    using namespace accessor; using namespace easing;
    std::string str("Chain " + utils::to_s(combo));
    std::string str2( utils::to_s(amounts) + " cubes!" );
    view::pScene s = scene_.lock();

    int y_offset = 200, x_offset = 0, w = map_setting()->width();
    if( map_setting()->width() % 2 == 0 )
        x_offset = (pos_vec2(w/2, 0).X + pos_vec2(w/2-1, 0).X) / 2;
    else x_offset = pos_vec2(w/2, 0).X;

    if( chain_texts_.find( chain ) == chain_texts_.end() ) {
        BOOST_FOREACH(ChainTextPair const& it, chain_texts_)
            y_offset += it.second->getSprite("chain").get<Size2D>().Y * 2;
        chain_texts_.insert( std::make_pair(chain, view::pMenu()) );
    } else y_offset = chain_texts_[chain]->get<Pos2D>().Y;

    view::pMenu m = view::Menu::create("", s, 100, 100, true);
    m->addSpriteText("chaino",  str, "kimberley", 0, 40, true, data::Color(255,0,0))
      .addSpriteText("chain",   str, "kimberley", 0, 40, true)
      .addSpriteText("amounto", str2,"kimberley", 0, 25, true, data::Color(255,0,0))
      .addSpriteText("amount",  str2,"kimberley", 0, 25, true)
      .setPickable(false);
    m->getSprite("chaino").set<Pos2D>(vec2(1,-1)).set<Scale>(vec3(1.03,1.15,1)).setPickable(false);
    m->getSprite("chain").setDepth(-10).setPickable(false);
    m->getSprite("amounto").set<Pos2D>(vec2(1,39)).set<Scale>(vec3(1.03,1.15,1)).setPickable(false);
    m->getSprite("amount").setDepth(-10).set<Pos2D>(vec2(0,40)).setPickable(false);
    m->setDepth(-100).set<Pos2D>( vec2(x_offset, y_offset) )
      .tween<OElastic, Scale>(vec3(0,0,0), vec3(1 + (0.12*combo),1 + (0.12*combo), 1), 1000, 0,
                              std::tr1::bind(&ViewSpriteMaster::pop_a_chain_text, this, chain) );

    chain_texts_[ chain ] = m;
    audio::Sound::i().playBuffer( ("2/2b_" + utils::to_s(combo >= 7 ? 7 : combo) + ".wav"));
}

void ViewSpriteMaster::new_garbage(model::wpChain const& chain, int n){
    int modelx = chain.lock()->last_step_x(), modely = chain.lock()->last_step_y();

    using namespace accessor; using namespace easing; using std::tr1::bind;
    using utils::to_s;
    vec2 pos = pos_vec2(modelx, modely);
    view::pScene s = scene_.lock();
    int num = n>20?20:n; //limit the animated garbage up to 20 ...in case of too many

    //2012.05 changed from throwing to off-screen, now throwing to over-head
    vec2 endp( view_setting()->ats_x(), view_setting()->ats_y() );

    for( int i = 0; i < num; ++i ) {
        view::pSprite g = view::Sprite::create("cubes/cube"+to_s(utils::random(4)+1), s, 64, 64, true);
        vec2 midp = (pos + endp)/2;
        midp.X += utils::random(120) - 60; midp.Y += utils::random(120) - 60;
        std::vector<vec2> wp; wp.push_back(pos); wp.push_back(midp); wp.push_back(endp);
        //std::vector<float> tension(3, .7f); //dont use tension yet, SplineAnimator has bug.
        data::AnimatorParam<Linear, Rotation> rota;
        data::AnimatorParam<IQuad, Alpha> alpha;
        data::AnimatorParam<Linear, Scale> scale;

        //data::WaypointParam<ICirc, Pos2D> way;
        data::AnimatorParam<ISine, Pos2D> way1;
        data::AnimatorParam<Linear, Pos2D> way2;

        data::CirclingParam<Linear, Pos3D> circle;

        rota.end(vec3(0,0,360)).duration(700).loop(-1).delay(-utils::random(500));
        alpha.start(0).end(255).duration(700);
        double sc = (utils::random(20)/100.0) + .6;
        scale.start(vec3(.1,.1,.1)).end(vec3(sc,sc,sc)).duration(700);
        //way.waypoints(wp).tensions(tension).duration(700+utils::random(300)); //dont use tension yet
        //way.waypoints(wp).duration(700+utils::random(300)); //buggy

        way1.start(pos).end(midp).duration(500+utils::random(150));
        way2.start(midp).end(endp).duration(100+utils::random(100));

        int rad = utils::random(30)+20;
        int arc = utils::random(180);
        circle.center(vec3(endp.X,-endp.Y,0)).
                      start(vec2(rad, arc)).
                      end(vec2(rad, arc+360)).
                      rotation(vec3(utils::random(180)-90,0,0)).
                      duration(1666+utils::random(666)).
                      loop(-1);

        attack_cubes_.push_back(g);
        //way.cb(bind(&view::Object::set<Visible>, g, false)); //hold copy of shared ptr so its alive.

        g->setDepth(-50).set<GradientDiffuse>(192 + utils::random(64)).tween(rota).tween(alpha).tween(scale);
        g->queue(way1).queue(way2).tween(circle);
    }
}

void ViewSpriteMaster::pop_garbage(int amount) {
    if( static_cast<unsigned int>(amount) >= attack_cubes_.size() ) {
        amount = attack_cubes_.size();
    }
    for( int i = 0; i < amount; ++i )
        attack_cubes_.pop_front();
}

void ViewSpriteMaster::warning_counting(int warning_level){
    using std::tr1::bind; using namespace accessor; using namespace easing;

    audio::Sound::i().playBuffer("3/3d/alarm.wav");
    alert_bar_top_->playAnime("moving", 1000);
    alert_bar_top_->tween<SineCirc, ColorDiffuseVec3>(vec3(255, 255, 255), vec3(255, 0, 0), 1000u);
    alert_bar_bottom_->playAnime("moving", 1000);
    alert_bar_bottom_->tween<SineCirc, ColorDiffuseVec3>(vec3(255, 255, 255), vec3(255, 0, 0), 1000u);
}

void ViewSpriteMaster::alert_bar_update(int warning_level){
    using namespace accessor;
    if( warning_level < 1 ) {
        alert_bar_top_->set< Visible >( false );
        alert_bar_top_->set< ScaleWithUV >( vec2(1, 1) );
        alert_bar_bottom_->set< Visible >( false );
        alert_bar_bottom_->set< ScaleWithUV >( vec2(1, 1) );
    } else {
        alert_bar_top_->set< ScaleWithUV >( vec2((warning_level)/100.0, 1) );
        alert_bar_top_->set< Visible >( true );
        alert_bar_bottom_->set< ScaleWithUV >( vec2((warning_level)/100.0, 1) );
        alert_bar_bottom_->set< Visible >( true );
    }
}

void ViewSpriteMaster::show_overheat(bool show){
    using namespace accessor;
    //audio::Sound::i().play("overheat_sound");
    overheat_->set<Visible>(show);
    overheat_bg_->set<Visible>(show);
}

void ViewSpriteMaster::create_warning_strips(){
    using namespace accessor; using namespace easing;
    view::pScene scene = scene_.lock();
    for( int i=0, width=map_setting()->width(),
                  h=map_setting()->height()-1; i<width; ++i )
    {
        view::pSprite temp = view::Sprite::create("warning", scene, 64, 64*h, true);
        vec2 pos;
        if( h % 2 == 0 )
            pos = (pos_vec2(i, h/2) + pos_vec2(i, h/2-1)) / 2;
        else pos = pos_vec2(i, h/2);
        temp->setDepth(-50).set<Pos2D>( pos ).setPickable(false);
        temp->set<ColorDiffuseVec3>(vec3(255,0,0)).set<Alpha>(96).set<Visible>(false);
        temp->tween<SineCirc, Alpha>(0, 1000u, -1);
        warning_strip_.push_back( temp );
    }
}

void ViewSpriteMaster::create_overheat_overlay(){
    using namespace accessor; using namespace easing;
    int w = map_setting()->width();
    int h = map_setting()->height() - 1;
    vec2 pos;
    if( w % 2 == 0 )
        pos = (pos_vec2(w/2, h/2) + pos_vec2(w/2-1, h/2)) / 2;
    else pos = pos_vec2(w/2, h/2); //memo: can we try to reuse this ..?
    if( h % 2 == 0 ) pos.Y += 32;

    overheat_ = view::Sprite::create("ui/overheat3", scene_.lock(), 270, 75, true);
    overheat_->setDepth(-60).set<Pos2D>( pos-vec2(0, 64) ).setPickable(false);
    overheat_->set<Visible>(false);
    overheat_->tween<SineCirc, Alpha>(0, 1000u, -1);

    overheat_bg_ = view::Sprite::create("overheat_bg", scene_.lock(), 64*w, 64*h, true);
    overheat_bg_->setDepth(-50).set<Pos2D>( pos ).setPickable(false);
    overheat_bg_->set<ColorDiffuseVec3>(vec3(0,0,0)).set<Alpha>(128).set<Visible>(false);
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

    //warning: the position and scale data here should be configurable.
    alert_bar_top_ = view::AnimatedSprite::create("alert", scene_.lock(), 64*w, 44, true);
    alert_bar_top_->playAnime("moving", 1000).setDepth(-50).set<Pos2D>( pos )
                   .set<Visible>(false).setPickable(false);

    pos2 = pos; pos2.Y = 706;

    alert_bar_bottom_ = view::AnimatedSprite::create("alert", scene_.lock(), 64*w, 44, true);
    alert_bar_bottom_->playAnime("moving", 1000).setDepth(-50).set<Pos2D>( pos2 )
                      .set<Visible>(false).setPickable(false);

    create_overheat_overlay();
    create_warning_strips();
}

void ViewSpriteMaster::show_warning_at(int x, bool visible){
    if( !map_setting()->dropping_creatable() ) visible = false;
    warning_strip_[x]->set<accessor::Visible>(visible);
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

vec2 ViewSpriteMaster::pos_vec2(int const& x, int const& y) const{
    return pos_vec2(view_setting(), x, y);
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
