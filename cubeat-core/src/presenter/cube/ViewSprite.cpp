
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

    body_->set<accessor::Pos2D>(pos_vec2());
    body_->setTexture("cubes/cube" + utils::to_s(utils::random(4)+1));

    data::Color col = data::Color::from_id(c->data()->color_id());
    col.offset();
    body_->set<accessor::ColorDiffuse>( 0xff000000 | col.rgb() );

    //shot_event(&model::Cube::go_exploding, &model::Cube::be_broken);
    shot_event(&model::Cube::go_exploding, &model::Cube::go_exploding);
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
    effect_body->set<accessor::Pos2D>( body_->get<accessor::Pos2D>() );

    IParticleSystemSceneNode* ps = effect_body->scene()->addParticleNodeTo(effect_body, false);
    ps->setIsDebugObject(true); // So it can't be picked.

    vec2 origpos = view_orig_.lock()->get<accessor::Pos2D>();
    vec2 pos( view_setting()->atf_x() - origpos.X, view_setting()->atf_y() - origpos.Y );
    vec2 flying_vector = (pos_vec2() - pos);
    int flying_distance = flying_vector.getLength();
    core::vector2df normal = flying_vector.normalize();

//    IParticleEmitter* em = ps->createBoxEmitter(
//        core::aabbox3d<f32>(0, 0, 0, 0.1, 0.1, 0.1), // emitter size
//        core::vector3df(0.0f, 0.0f, 0.0f),   // initial direction
//        flying_distance/2, flying_distance/2,    // emit rate
//        video::SColor(0,255,255,255),       // darkest color
//        video::SColor(0,255,255,255),       // brightest color
//        200, 200, 0,                         // min and max age, angle
//        core::dimension2df(40.f,40.f),         // min size
//        core::dimension2df(40.f,40.f));        // max size

    IParticleEmitter* em = new irr::scene::LinearParticleEmitter(
        core::vector3df(0.0f, 0.0f, 0.0f),  // center -- more depth, so no z-fighting
        core::vector3df(-normal.X, normal.Y, 0.0f), flying_distance/10, // normal, length
        core::vector3df(0.0f, 0.0f, 0.0f),   // initial direction
#if !defined(_SHOOTING_CUBES_ANDROID_)
        flying_distance/3, flying_distance/3,    // emit rate
#else
        flying_distance/3, flying_distance/3,    // emit rate
#endif
        video::SColor(0,255,255,255),       // darkest color
        video::SColor(0,255,255,255),       // brightest color
        200, 200, 0,                         // min and max age, angle
        core::dimension2df(40.f,40.f),         // min size
        core::dimension2df(40.f,40.f)         // max size
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
    body_->setDepth(-20);

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
    //shot_event(&model::Cube::restore, &model::Cube::go_exploding);
    shot_event(&model::Cube::restore, &model::Cube::restore);
    //audio::Sound::i().playBuffer("1/d/ShotA@11.wav");
}

void ViewSprite::restore(int color_id){
    if( color_id == -1 ) return;
    body_->setTexture("cubes/cube" + utils::to_s(utils::random(4)+1));
    data::Color col = data::Color::from_id(color_id);
    col.offset();
    body_->set<accessor::ColorDiffuse>( 0xff000000 | col.rgb() );
    body_->tween<easing::OBack, accessor::Scale>(vec3(.7,.7,.7), vec3(1,1,1), 300u);
    //shot_event(&model::Cube::go_exploding, &model::Cube::be_broken);
    shot_event(&model::Cube::go_exploding, &model::Cube::go_exploding);
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
        body_->tween<easing::SineCirc, accessor::GradientEmissive>(128, 500u, -1);
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

} //cube
} //presenter
} //psc
