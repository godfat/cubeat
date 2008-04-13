
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
#include "utils/MapLoader.hpp"
#include "Conf.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace psc;
using namespace easing;
using namespace accessor;

class TestGame{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;
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
        map0_ = presenter::Map::create(set0, utils::MapLoader::load_cube_colors("config/puzzle.zzml"));
        map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s0, player0_) );

        // setup map1
        data::pMapSetting set1 = data::MapSetting::create();
        set1->centerx(932);
        map1_ = presenter::Map::create(set1);
        map1_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s1, player1_) );

        // setup garbage land
        map0_->push_garbage_land(map1_);
        map1_->push_garbage_land(map0_);

        //temporary Scene:
        setup_stage_by_config( "config/stage/jungle.zzml" );
        setup_ui_by_config( "config/ui/in_game_2p_layout.zzml" );
        setup_char_sprite_by_config( char_1p_, conf1p_, "config/char/char1.zzml" );
        setup_char_sprite_by_config( char_2p_, conf2p_, "config/char/char2.zzml" );
        char_2p_->flipH();

        min_ = 0, sec_ = 0 ,last_garbage_1p_ = 0, last_garbage_2p_ = 0;

        //start music
        Sound::i().play( stageconf_.S("music"), true);

        for( size_t i = 0; i < conf2p_.V("face_pos").size(); ++i ) {
            utils::map_any& pos = conf2p_.V("face_pos").M(i);
            pos.I("x") *= -1;  //2p's face position needs to be flipped horizontally
        }

        clear_state( conf1p_ );
        clear_state( conf2p_ );
        conf1p_["current_face"] = static_cast<int>(NORMAL);
        conf2p_["current_face"] = static_cast<int>(NORMAL);

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestGame::update_ui_by_second, this), 1000, -1);
    }

    void setup_stage_by_config(std::string const& path)
    {
        stageconf_ = utils::map_any::construct( utils::fetchConfig( path ) );
        stage_ = view::Scene::create( stageconf_.S("name") );
        stage_->setTo3DView( stageconf_.I("FoV") / 180.f * PI );
        utils::vector_any const& lists = stageconf_.V("all_items");
        BOOST_FOREACH(boost::any const& list, lists) {
            slists_.push_back( SceneObjList() );
            SceneObjList& slist = slists_.back();
            utils::vector_any const& items = boost::any_cast<utils::vector_any const>(list);
            BOOST_FOREACH(boost::any const& it, items) {
                utils::map_any const& item = boost::any_cast<utils::map_any const>(it);
                view::pAnimatedSceneObject obj;
                obj = view::AnimatedSceneObject::create( item.S("xfile") ,stage_ );

                utils::map_any const& pos = item.M("position"), &rot = item.M("rotation"),
                    &sca = item.M("scale"), &dif = item.M("diffuse");
                obj->set<Pos3D>(    vec3( pos.I("x"), pos.I("y"), pos.I("z") ) )
                    .set<Rotation>( vec3( rot.I("x"), rot.I("y"), rot.I("z") ) )
                    .set<Scale>(    vec3( sca.F("x"), sca.F("y"), sca.F("z") ) )
                    .set<ColorDiffuseVec3>( vec3( dif.I("r"), dif.I("g"), dif.I("b") ) )
                    .set<Alpha>( dif.I("a") );

                utils::map_any const& econf = item.M("anim").M("emerge");
                utils::map_any const& iconf = item.M("anim").M("idle");
                data::AnimatorParam<Linear, Frame> emerge;
                data::AnimatorParam<Linear, Frame> idle;
                emerge.start( econf.I("s") ).end( econf.I("e") ).duration( econf.I("duration") ).loop( econf.I("loop") ).delay( 2500 );
                idle.start( iconf.I("s") ).end( iconf.I("e") ).duration( iconf.I("duration") ).loop( iconf.I("loop") );
                obj->queue(emerge).tween(idle);
                slist.push_back( obj );
            }
        }
    }

    void setup_ui_by_config( std::string const& path ) {
        uiconf_ = utils::map_any::construct( utils::fetchConfig( path ) );
        utils::map_any const& base = uiconf_.M("base");
        ui_layout_ = view::Menu::create( base.S("layout_tex"), scene_, base.I("w"), base.I("h") );
        ui_layout_->set<Alpha>(192);

        utils::map_any const& misc = uiconf_.M("misc");
        BOOST_FOREACH(utils::pair_any const& it, misc) {
            std::string    const& key  = boost::any_cast<std::string const>(it.first);
            utils::map_any const& attr = boost::any_cast<utils::map_any const>(it.second);
            ui_layout_->
                addSpriteText(key, attr.S("text"), attr.S("font"), 0, attr.I("fsize"), attr.I("center") )
               .getSpriteText(key).set<Pos2D>( vec2(attr.I("x"), attr.I("y")) );
        }
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

    void hit_stage_group(int const& id) {
        SceneObjList& slist = slists_[id];
        utils::vector_any const& listconf = stageconf_.V("all_items").V(id);
        for( size_t i = 0; i < slist.size(); ++i ) {
            view::pAnimatedSceneObject& obj = slist[i];
            utils::map_any const& hconf = listconf.M(i).M("anim").M("hit");
            utils::map_any const& rconf = listconf.M(i).M("anim").M("recover");
            utils::map_any const& iconf = listconf.M(i).M("anim").M("idle");
            data::AnimatorParam<OBounce, Frame> hit;
            data::AnimatorParam<Linear, Frame> recover;
            data::AnimatorParam<Linear, Frame> idle;
            int fn = obj->get<Frame>(), s = fn < hconf.I("s") ? hconf.I("s") : fn;
            hit.start( s ).end( hconf.I("e") ).duration( hconf.I("duration") );
            recover.start( rconf.I("s") ).end( rconf.I("e") ).duration( rconf.I("duration") );
            idle.start( iconf.I("s") ).end( iconf.I("e") ).duration( iconf.I("duration") ).loop(-1);
            obj->clearAllTween().queue(hit).queue(recover).tween(idle);
        }
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

        int state1p = conf1p_.I("current_state");
        if( state1p != HIT && last_garbage_1p_ > new_garbage_1p_ ) {
            hit_stage_group(1);
            switch_character_sprite_state( char_1p_, conf1p_, HIT );
        }
        else if( state1p != ATTACK && state1p != HIT && map0_->current_sum_of_attack() > 1 )
            switch_character_sprite_state( char_1p_, conf1p_, ATTACK );
        else if( state1p == NONE )
            switch_character_sprite_state( char_1p_, conf1p_, STAND );

        int state2p = conf2p_.I("current_state");
        if( state2p != HIT && last_garbage_2p_ > new_garbage_2p_ ) {
            hit_stage_group(2);
            switch_character_sprite_state( char_2p_, conf2p_, HIT );
        }
        else if( state2p != ATTACK && state2p != HIT && map1_->current_sum_of_attack() > 1 )
            switch_character_sprite_state( char_2p_, conf2p_, ATTACK );
        else if( state2p == NONE )
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
    view::pScene scene_, stage_;
    presenter::pMap map0_;
    presenter::pMap map1_;
    ctrl::pPlayer player0_;
    ctrl::pPlayer player1_;

    utils::map_any stageconf_;
    utils::map_any uiconf_;
    utils::map_any conf1p_;
    utils::map_any conf2p_;

    std::vector< SceneObjList > slists_;
    view::pMenu ui_layout_;
    view::pMenu char_1p_;
    view::pMenu char_2p_;
    int min_, sec_;
    int last_garbage_1p_, last_garbage_2p_; //used for temporary state comparison
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
