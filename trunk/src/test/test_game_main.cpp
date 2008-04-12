
#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "view/SpriteText.hpp"
#include "view/Menu.hpp"
#include "view/AnimatedSprite.hpp"
#include "view/AnimatedSceneObject.hpp"
#include "Accessors.hpp"

#include "presenter/Map.hpp"
#include "presenter/cube/ViewSprite.hpp"
#include "presenter/cube/ViewStdout.hpp"

#include "EventDispatcher.hpp"
#include "Input.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "Sound.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "Conf.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace psc;
using namespace easing;
using namespace accessor;

class TestGame{
    typedef std::tr1::shared_ptr<int> pDummy;
    enum STATE { STAND, ATTACK, HIT, NONE };
    enum FACE  { NORMAL, GOOD, BAD };

public:
    TestGame(){
        scene_ = psc::view::Scene::create("TestMapViewScene");
        scene_->setTo2DView().enableGlobalHittingEvent();     //important

        data::pViewSpriteSetting s0, s1;

        s0 = data::ViewSpriteSetting::create(159, 684, 64);   //must use config
        s0->push_ally(1).push_enemy(0);
        s1 = data::ViewSpriteSetting::create(740, 684, 64);   //must use config
        s1->push_ally(0).push_enemy(1);

        ///THIS IS IMPORTANT, ALL PLAYERS MUST BE DEFINED FIRST.
        player0_ = ctrl::Player::create(ctrl::Input::getInputByIndex(1), s0->ally_input_ids(), s0->enemy_input_ids());
        player1_ = ctrl::Player::create(ctrl::Input::getInputByIndex(0), s1->ally_input_ids(), s1->enemy_input_ids());

        // setup map0
        data::pMapSetting set0 = data::MapSetting::create();
        set0->centerx(352);
        map0_ = presenter::Map::create(set0);
        map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );

        // setup map1
        data::pMapSetting set1 = data::MapSetting::create();
        set1->centerx(932);
        map1_ = presenter::Map::create(set1);
        map1_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s1, player1_) );

        // mini view (currently not used, close it down)
//        map1_->push_view_slave( presenter::cube::ViewSpriteMaster::create(scene_,
//            data::ViewSpriteSetting::create(800, 300, 25) ) );

        // setup garbage land
        map0_->push_garbage_land(map1_);
        map1_->push_garbage_land(map0_);


        ///START OF SCARY GUI SETTING

        //temporary Scene: must use config it all
        stage_ = view::Scene::create("TestGameStage");
        stage_->setTo3DView(PI/5.f);

        data::AnimatorParam<Linear, Frame> ani;
        ani.end(90.f).duration(9000).loop(-1);
        bg_ = view::AnimatedSceneObject::create("jungle/bg", stage_ );
        bg_->set<Pos3D>(vec3(0, -17.5, 50)).set<Scale>(vec3(1.23f, 1.15f, 1.1f)).tween(ani);

        //create palms left / right
        for( int i = 0; i < 8; ++i ) {
            data::AnimatorParam<Linear, Frame> paramAni;
            paramAni.end(90.f).duration( 3000+utils::random(1000) ).loop(-1).delay( utils::random(3000) );
            std::string kind = boost::lexical_cast<std::string>( utils::random(3)+1 );
            palms_.push_back( view::AnimatedSceneObject::create("jungle/palm"+kind, stage_) );
            int x = - (utils::random(22) + 3);
            float y = -13 + utils::random(100)/50.f;
            float z = 38 + utils::random(14) + utils::random(100)/100.f ;
            palms_[i]->set<Pos3D>(vec3(x,y,z)).tween(paramAni);
            int bri = 255 - 192 * ((z-38.f)/15.f);
            palms_[i]->set<GradientDiffuse>( bri );
        }

        for( int i = 8; i < 16; ++i ) {
            data::AnimatorParam<Linear, Frame> paramAni;
            paramAni.end(90.f).duration( 3000+utils::random(1000) ).loop(-1).delay( utils::random(3000) );
            std::string kind = boost::lexical_cast<std::string>( utils::random(3)+1 );
            palms_.push_back( view::AnimatedSceneObject::create("jungle/palm"+kind, stage_) );
            int x = (utils::random(22) + 3);
            float y = -13 + utils::random(100)/50.f;
            float z = 38 + utils::random(10) + utils::random(100)/100.f ;
            palms_[i]->set<Pos3D>(vec3(x,y,z)).tween(paramAni);
            int bri = 255 - 192 * ((z-38.f)/10.f);
            palms_[i]->set<GradientDiffuse>( bri );
        }

        //create grasses (no left right)
        for( int i = 0; i < 16; ++i ) {
            data::AnimatorParam<Linear, Frame> paramAni;
            paramAni.end(90.f).duration( 3000+utils::random(1000) ).loop(-1).delay( utils::random(3000) );
            std::string kind = boost::lexical_cast<std::string>( utils::random(4)+1 );
            grass_.push_back( view::AnimatedSceneObject::create("jungle/grass"+kind, stage_) );
            int x = utils::random(50) - 25;
            float y = -13 + utils::random(100)/50.f;
            float z = 36 + utils::random(12) + utils::random(100)/100.f ;
            grass_[i]->set<Pos3D>(vec3(x,y,z)).set<Scale>(vec3(1.5, 1.5, 1.5)).tween(paramAni);
            int bri = 255 - 192 * ((z-36.f)/12.f);
            grass_[i]->set<GradientDiffuse>( bri );
        }

        //UI (by config):
        uiconf_ = utils::map_any::construct( utils::fetchConfig("config/ui/in_game_2p_layout.zzml") );
        utils::map_any const& base = uiconf_.M("base");
        ui_layout_ = view::Menu::create( base.S("layout_tex"), scene_, base.I("w"), base.I("h") );

        utils::map_any const& misc = uiconf_.M("misc");
        BOOST_FOREACH(utils::pair_any const& it, misc) {
            std::string    const& key  = boost::any_cast<std::string const>(it.first);
            utils::map_any const& attr = boost::any_cast<utils::map_any const>(it.second);
            ui_layout_->
                addSpriteText(key, attr.S("text"), attr.S("font"), 0, attr.I("fsize"), attr.I("center") )
               .getSpriteText(key).set<Pos2D>( vec2(attr.I("x"), attr.I("y")) );
        }

        //character sprite setup (by config):
        setup_char_sprite_by_config( char_1p_, conf1p_, "config/char/char1.zzml" );
        setup_char_sprite_by_config( char_2p_, conf2p_, "config/char/char2.zzml" );
        char_2p_->flipH();

        min_ = 0, sec_ = 0;

        last_garbage_1p_ = 0, last_garbage_2p_ = 0;
        /// END OF SCARY UI SETUP

        //start music
        Sound::i().play("jungle/bgm.mp3", true);

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestGame::update_ui_by_second, this), 1000, -1);

        for( size_t i = 0; i < conf2p_.V("face_pos").size(); ++i ) {
            utils::map_any& pos = conf2p_.V("face_pos").M(i);
            pos.I("x") *= -1;  //2p's face position needs to be flipped horizontally
        }

        clear_state( conf1p_ );
        clear_state( conf2p_ );
        conf1p_["current_face"] = static_cast<int>(NORMAL);
        conf2p_["current_face"] = static_cast<int>(NORMAL);
    }

    void setup_char_sprite_by_config(view::pMenu& charsp, utils::map_any& conf, std::string const& path) {
        conf = utils::map_any::construct( utils::fetchConfig( path ) );
        utils::map_any const& anim = conf.M("anim_attr");

        vec2 character_center( uiconf_.I("character_center_x"), uiconf_.I("character_center_y") );
        charsp = view::Menu::create("", scene_, 1, 1, true);
        charsp->set<Pos2D>( character_center );

        BOOST_FOREACH(utils::pair_any const& it, anim) {
            std::string    const& key  = boost::any_cast<std::string const>(it.first);
            utils::map_any const& attr = boost::any_cast<utils::map_any const>(it.second);
            charsp->addAnimSprite(key, attr.S("anim"), 0, attr.I("w"), attr.I("h"), true )
                   .getAnimSprite(key).playAnime( attr.S("defanim"), attr.I("ms"), attr.I("loop") )
                                      .set<Pos2D>( vec2(attr.I("x"), attr.I("y")) );
        }
        utils::map_any const& pos = conf.V("face_pos").M(0);
        utils::map_any const& face= conf.M("face");
        charsp->addSprite("face", 0, conf.I("face_w"), conf.I("face_h"), true, face.M(NORMAL).S("tex") )
               .getSprite("face").set<Pos2D>( vec2(pos.I("x"), pos.I("y")) );
    }

    void cycle(){
        update_ui();
        stage_->redraw();
        //IrrDevice::i().d()->getVideoDriver()->clearZBuffer();
        scene_->redraw();
        map0_->redraw().cycle();
        map1_->redraw().cycle();
    }

    void switch_character_sprite_state
        ( view::pMenu& charsp, utils::map_any& conf, STATE const& state )
    {
        using std::tr1::ref; using std::tr1::bind;
        conf["current_state"] = static_cast<int>(state);
        utils::map_any const& attr = conf.M("state").M(state);
        charsp->getSprite("face").set<Visible>( attr.I("face_visible") );
        charsp->getAnimSprite("body")
               .playAnime( attr.S("anim"), 1000, 0, bind(&TestGame::clear_state, this, ref(conf)) );
        if( attr.S("sound") != "" )
            Sound::i().play( attr.S("sound") );
        if( state == STAND ) {
            conf["face_pos_idx"] = 0;
            utils::vector_any& face_pos = conf.V("face_pos");
            charsp->getSprite("face").set<Pos2D>( vec2( face_pos.M(0).I("x"), face_pos.M(0).I("y") ) );
            ctrl::EventDispatcher::i().subscribe_timer(
                std::tr1::bind(&TestGame::face_update, this, ref(charsp), ref(conf)), 250, 2);
        }
    }

    void switch_character_face
        ( view::pMenu& charsp, utils::map_any& conf, FACE const& fstate )
    {
        if( conf1p_["current_face"] == fstate ) return;
        conf1p_["current_face"] = static_cast<int>(fstate);
        utils::map_any const& attr = conf.M("face").M(fstate);
        charsp->getSprite("face").setTexture( attr.S("tex") );
        charsp->getAnimSprite("bdeco").set<Visible>( attr.I("bdeco") );
        charsp->getAnimSprite("gdeco").set<Visible>( attr.I("gdeco") );
    }

    void clear_state( utils::map_any& conf ) {
        conf["current_state"] = static_cast<int>(NONE);
    }

    void face_update( view::pMenu& charsp, utils::map_any& conf){
        int& idx = conf.I("face_pos_idx");
        utils::vector_any& face_pos = conf.V("face_pos");
        ++idx;
        if( static_cast<unsigned int>(idx) >= face_pos.size() ) idx = 0;
        charsp->getSprite("face").set<Pos2D>( vec2( face_pos.M(idx).I("x"), face_pos.M(idx).I("y") ) );
    }

    void update_ui(){
        int new_garbage_1p_ = map0_->garbage_left() + map1_->current_sum_of_attack();
        int new_garbage_2p_ = map1_->garbage_left() + map0_->current_sum_of_attack();
        ui_layout_->getSpriteText("gar1p").showNumber(new_garbage_1p_);
        ui_layout_->getSpriteText("gar2p").showNumber(new_garbage_2p_);
        ui_layout_->getSpriteText("scr1p").showNumber(map0_->score(), 5);
        ui_layout_->getSpriteText("scr2p").showNumber(map1_->score(), 5);
        ui_layout_->getSpriteText("wep1p1").showNumber(player0_->weapon(0)->ammo(), 2);
        ui_layout_->getSpriteText("wep1p2").showNumber(player0_->weapon(1)->ammo(), 2);
        ui_layout_->getSpriteText("wep1p3").showNumber(player0_->weapon(2)->ammo(), 2);
        ui_layout_->getSpriteText("wep2p1").showNumber(player1_->weapon(0)->ammo(), 2);
        ui_layout_->getSpriteText("wep2p2").showNumber(player1_->weapon(1)->ammo(), 2);
        ui_layout_->getSpriteText("wep2p3").showNumber(player1_->weapon(2)->ammo(), 2);

        if( conf1p_.I("current_state") != HIT && last_garbage_1p_ > new_garbage_1p_ )
            switch_character_sprite_state( char_1p_, conf1p_, HIT );
        else if( conf1p_.I("current_state") != ATTACK && conf1p_.I("current_state") != HIT &&
                 map0_->current_sum_of_attack() > 1 )
            switch_character_sprite_state( char_1p_, conf1p_, ATTACK );
        else if( conf1p_.I("current_state") == NONE )
            switch_character_sprite_state( char_1p_, conf1p_, STAND );

        if( conf2p_.I("current_state") != HIT && last_garbage_2p_ > new_garbage_2p_ )
            switch_character_sprite_state( char_2p_, conf2p_, HIT );
        else if( conf2p_.I("current_state") != ATTACK && conf2p_.I("current_state") != HIT &&
                 map1_->current_sum_of_attack() > 1 )
            switch_character_sprite_state( char_2p_, conf2p_, ATTACK );
        else if( conf2p_.I("current_state") == NONE )
            switch_character_sprite_state( char_2p_, conf2p_, STAND );

        bool map0_column_full = map0_->has_column_full(), map1_column_full = map1_->has_column_full();

        if( !map0_column_full && !map1_column_full ) {
            switch_character_face( char_1p_, conf1p_, NORMAL );
            switch_character_face( char_2p_, conf2p_, NORMAL );
        } else {
            switch_character_face( char_1p_, conf1p_, map0_column_full ? BAD:GOOD );
            switch_character_face( char_2p_, conf2p_, map1_column_full ? BAD:GOOD );
        }

        last_garbage_1p_ = new_garbage_1p_;
        last_garbage_2p_ = new_garbage_2p_;
    }

    void update_ui_by_second(){
        ++sec_;
        if( sec_ > 59 ) ++min_, sec_ = 0;
        std::string sec = boost::lexical_cast<std::string>(sec_); if( sec.size() < 2 ) sec = "0" + sec;
        std::string min = boost::lexical_cast<std::string>(min_); if( min.size() < 2 ) min = "0" + min;
        ui_layout_->getSpriteText("time").changeText( min + ":" + sec );
    }

private:
    view::pScene scene_;
    presenter::pMap map0_;
    presenter::pMap map1_;
    ctrl::pPlayer player0_;
    ctrl::pPlayer player1_;

    utils::map_any uiconf_;
    utils::map_any conf1p_;
    utils::map_any conf2p_;

    //Temporary Scene
    view::pScene  stage_;
    view::pAnimatedSceneObject bg_;
    std::vector< view::pAnimatedSceneObject > palms_;
    std::vector< view::pAnimatedSceneObject > grass_;

    //Temporary UI
    view::pMenu ui_layout_;
    //Temporary Character
    view::pMenu char_1p_;
    view::pMenu char_2p_;
    int min_, sec_;

    //used for temporary state comparison
    int last_garbage_1p_, last_garbage_2p_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <tr1/functional> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::App::i();
    TestGame tester;
    return psc::App::i().run(std::tr1::bind(&TestGame::cycle, &tester));
}
