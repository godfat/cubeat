
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

        //other ui which needs to be configured....
        character_1p_ = view::AnimatedSprite::create("char1", scene_, 192, 192, true);
        character_1p_->textureFlipH().set<Pos2D>(vec2(640, 580));
        character_2p_ = view::AnimatedSprite::create("char2", scene_, 192, 192, true);
        character_2p_->set<Pos2D>(vec2(640,580));

        good_deco_1p_ = view::AnimatedSprite::create("good_deco", character_1p_, 50, 50);
        good_deco_1p_->textureFlipH().set<Pos2D>(vec2(-24, -96));
        good_deco_1p_->playAnime("moving", 1000, -1).set<Visible>(false);

        good_deco_2p_ = view::AnimatedSprite::create("good_deco", character_2p_, 50, 50);
        good_deco_2p_->set<Pos2D>(vec2(-21, -96));
        good_deco_2p_->playAnime("moving", 1000, -1).set<Visible>(false);

        bad_deco_1p_ = view::AnimatedSprite::create("bad_deco", character_1p_, 50, 75);
        bad_deco_1p_->textureFlipH().setDepth(-1).set<Pos2D>(vec2(-96, -96));
        bad_deco_1p_->playAnime("moving", 1000, -1).set<Visible>(false);

        bad_deco_2p_ = view::AnimatedSprite::create("bad_deco", character_2p_, 50, 75);
        bad_deco_2p_->set<Pos2D>(vec2(46, -96));
        bad_deco_2p_->playAnime("moving", 1000, -1).set<Visible>(false);

        face_1p_ = view::Sprite::create("char1/face_normal", character_1p_, 50, 50);
        face_1p_->textureFlipH().set<Pos2D>(vec2(-72, -64));

        face_2p_ = view::Sprite::create("char2/face_normal", character_2p_, 50, 50);
        face_2p_->set<Pos2D>(vec2(114, 33));

        character_1p_->playAnime("stand", 1000, -1);
        character_2p_->playAnime("stand", 1000, -1);

        min_ = 0, sec_ = 0, last_garbage_1p_ = 0, last_garbage_2p_ = 0;
        state_1p_ = STAND, state_2p_ = STAND;

        /// END OF SCARY UI SETUP

        //start music
        Sound::i().play("jungle/bgm.mp3", true);

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestGame::update_ui_by_second, this), 1000, -1);

        //about face... how to do it?
        face_1p_pos_.push_back(vec2(-72, -64));
        face_1p_pos_.push_back(vec2(-72, -65));
        face_1p_pos_.push_back(vec2(-72, -66));
        face_1p_pos_.push_back(vec2(-72, -65));
//        face_2p_pos_.push_back(vec2(114, 33));
//        face_2p_pos_.push_back(vec2(115, 34));
//        face_2p_pos_.push_back(vec2(117, 35));
//        face_2p_pos_.push_back(vec2(115, 34));
        face_2p_pos_.push_back(vec2(18, -63));
        face_2p_pos_.push_back(vec2(19, -62));
        face_2p_pos_.push_back(vec2(21, -61));
        face_2p_pos_.push_back(vec2(19, -62));

        restore_1p_state();
        restore_2p_state();
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
        charsp->addSprite("face", 0, conf.I("face_w"), conf.I("face_h"), true, conf.S("normal") )
               .getSprite("face").set<Pos2D>( vec2(pos.I("x"), pos.I("y")) );
    }

    void cycle(){
        update_ui();
        stage_->redraw();
        //IrrDevice::i().d()->getVideoDriver()->clearZBuffer();
        scene_->redraw();
//        map0_->redraw().cycle_fast();
//        map1_->redraw().cycle_fast();
        map0_->redraw().cycle();
        map1_->redraw().cycle();
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

        //1p and 2p state change here.. should become state pattern:
        if( state_1p_ != HIT && last_garbage_1p_ > new_garbage_1p_ ) {
            state_1p_ = HIT;
            face_1p_->set<Visible>(false);
            stupid_dummy_1p_.reset();
            character_1p_->playAnime("hit", 1000, 0, std::tr1::bind(&TestGame::restore_1p_state, this));
            Sound::i().play("char1/hit.wav");
        }
        else if( state_1p_ != ATTACK && state_1p_ != HIT && map0_->current_sum_of_attack() > 1 ) {
            state_1p_ = ATTACK;
            face_1p_->set<Visible>(false);
            stupid_dummy_1p_.reset();
            character_1p_->playAnime("attack", 1000, 0, std::tr1::bind(&TestGame::restore_1p_state, this));
            Sound::i().play("char1/attack.wav");
        }

        if( state_2p_ != HIT && last_garbage_2p_ > new_garbage_2p_ ) {
            state_2p_ = HIT;
            face_2p_->set<Visible>(false);
            stupid_dummy_2p_.reset();
            character_2p_->playAnime("hit", 1000, 0, std::tr1::bind(&TestGame::restore_2p_state, this));
            Sound::i().play("char2/hit.wav");
        }
        else if( state_2p_ != ATTACK && state_2p_ != HIT && map1_->current_sum_of_attack() > 1 ) {
            state_2p_ = ATTACK;
            face_2p_->set<Visible>(false);
            stupid_dummy_2p_.reset();
            character_2p_->playAnime("attack", 1000, 0, std::tr1::bind(&TestGame::restore_2p_state, this));
            Sound::i().play("char2/attack.wav");
        }

        bool map0_column_full = map0_->has_column_full(), map1_column_full = map1_->has_column_full();
        if( map0_column_full && map1_column_full ) {
            face_1p_->setTexture("char1/face_bad");
            face_2p_->setTexture("char2/face_bad");
            bad_deco_1p_->set<Visible>(true); good_deco_1p_->set<Visible>(false);
            bad_deco_2p_->set<Visible>(true); good_deco_2p_->set<Visible>(false);
        }
        else if( !map0_column_full && map1_column_full ) {
            face_1p_->setTexture("char1/face_good");
            face_2p_->setTexture("char2/face_bad");
            bad_deco_1p_->set<Visible>(false); good_deco_1p_->set<Visible>(true);
            bad_deco_2p_->set<Visible>(true);  good_deco_2p_->set<Visible>(false);
        }
        else if( map0_column_full && !map1_column_full ) {
            face_1p_->setTexture("char1/face_bad");
            face_2p_->setTexture("char2/face_good");
            bad_deco_1p_->set<Visible>(true);  good_deco_1p_->set<Visible>(false);
            bad_deco_2p_->set<Visible>(false); good_deco_2p_->set<Visible>(true);
        }
        else {
            face_1p_->setTexture("char1/face_normal");
            face_2p_->setTexture("char2/face_normal");
            bad_deco_1p_->set<Visible>(false); good_deco_1p_->set<Visible>(false);
            bad_deco_2p_->set<Visible>(false); good_deco_2p_->set<Visible>(false);
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

    void restore_1p_state(){
        face_1p_->set<Visible>(true);
        state_1p_ = STAND;
        stupid_dummy_1p_ = std::tr1::shared_ptr<int>(new int); //refresh
        restore_1p_state_();
        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestGame::restore_1p_state_, this), stupid_dummy_1p_, 1000, -1);
    }

    void restore_1p_state_(){
        character_1p_->playAnime("stand", 1000, -1);
        f1_it_ = face_1p_pos_.begin();
        face_1p_->set<Pos2D>( *f1_it_ );
        stupid_dummy_1p2_ = std::tr1::shared_ptr<int>(new int); //refresh2
        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestGame::face_1p_update, this), stupid_dummy_1p2_, 250, -1);
    }

    void restore_2p_state(){
        face_2p_->set<Visible>(true);
        state_2p_ = STAND;
        stupid_dummy_2p_ = std::tr1::shared_ptr<int>(new int); //refresh
        restore_2p_state_();
        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestGame::restore_2p_state_, this), stupid_dummy_2p_, 1000, -1);
    }

    void restore_2p_state_(){
        character_2p_->playAnime("stand", 1000, -1);
        f2_it_ = face_2p_pos_.begin();
        face_2p_->set<Pos2D>( *f2_it_ );
        stupid_dummy_2p2_ = std::tr1::shared_ptr<int>(new int); //refresh2
        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestGame::face_2p_update, this), stupid_dummy_2p2_, 250, -1);
    }

    //SUPER BRUTE FORCE... but I can't think of any faster way to do this....
    void face_1p_update(){
        ++f1_it_;
        if( f1_it_ == face_1p_pos_.end() ) f1_it_ = face_1p_pos_.begin();
        face_1p_->set<Pos2D>( *f1_it_ );
    }
    void face_2p_update(){
        ++f2_it_;
        if( f2_it_ == face_2p_pos_.end() ) f2_it_ = face_2p_pos_.begin();
        face_2p_->set<Pos2D>( *f2_it_ );
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
    view::pAnimatedSprite character_1p_, character_2p_;
    view::pSprite         face_1p_, face_2p_;
    view::pAnimatedSprite good_deco_1p_, good_deco_2p_;
    view::pAnimatedSprite bad_deco_1p_, bad_deco_2p_;
    int min_, sec_;

    //used for temporary state comparison
    enum { STAND, ATTACK, HIT };
    enum { NORMAL, GOOD, BAD };
    int last_garbage_1p_, last_garbage_2p_;
    int state_1p_, state_2p_;
    std::vector<vec2> face_1p_pos_;
    std::vector<vec2> face_2p_pos_;
    std::vector<vec2>::iterator f1_it_;
    std::vector<vec2>::iterator f2_it_;
    std::tr1::shared_ptr<int> stupid_dummy_1p_, stupid_dummy_2p_;
    std::tr1::shared_ptr<int> stupid_dummy_1p2_, stupid_dummy_2p2_;
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
