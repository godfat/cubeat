
#include "presenter/PlayerView.hpp"
#include "presenter/Map.hpp"
#include "view/AnimatedSprite.hpp"
#include "view/Menu.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "EasingEquations.hpp"
#include "Accessors.hpp"
#include "audio/Sound.hpp"
#include "Input.hpp"
#include "Conf.hpp"

#include "utils/Logger.hpp"

using namespace psc;
using namespace presenter;
using namespace easing;
using namespace accessor;
using utils::Logger;

PlayerView::PlayerView()
    :input_(0), last_my_garbage_(0), last_enemy_garbage_(0), current_state_(NONE), current_face_(NORMAL), face_pos_idx_(0)
{
}

PlayerView::~PlayerView()
{
}

pPlayerView PlayerView::init(std::string const& path, view::pObject const& parent, vec2 const& pos)
{
    conf_ = Conf::i().config_of(path);
    utils::map_any const& anim = conf_.M("anim_attr");

    character_ = view::Menu::create("character", parent, 0, 0, true);
    character_->set<Pos2D>( pos );

    BOOST_FOREACH(utils::pair_any const& it, anim) {
        std::string    const& key  = boost::any_cast<std::string const>(it.first);
        utils::map_any const& attr = boost::any_cast<utils::map_any const>(it.second);
        character_->addAnimSprite(key, attr.S("anim"), 0, attr.I("w"), attr.I("h"), true )
                   .getAnimSprite(key).playAnime( attr.S("defanim"), attr.I("ms"), attr.I("loop") )
                                      .set<Pos2D>( vec2(attr.I("x"), attr.I("y")) );
    }
    utils::map_any const& fpos = conf_.V("face_pos").M(0);
    utils::vector_any const& face = conf_.V("face");
    character_->addSprite("face", 0, conf_.I("face_w"), conf_.I("face_h"), true, face.M(NORMAL).S("tex") )
               .getSprite("face").set<Pos2D>( vec2(fpos.I("x"), fpos.I("y")) );

    BOOST_FOREACH(utils::any_type const& it, conf_.V("face")) {
        utils::map_any const& face = boost::any_cast<utils::map_any const>(it);
        faces_.push_back( FaceState() );
        faces_.back().tex = face.S("tex");
        faces_.back().gdeco = face.I("gdeco");
        faces_.back().bdeco = face.I("bdeco");
    }
    BOOST_FOREACH(utils::any_type const& it, conf_.V("state")) {
        utils::map_any const& stat = boost::any_cast<utils::map_any const>(it);
        states_.push_back( CharState() );
        states_.back().anim = stat.S("anim");
        states_.back().face_visible = stat.I("face_visible");
        states_.back().sound = stat.S("sound");
    }
    BOOST_FOREACH(utils::any_type const& it, conf_.V("face_pos")) {
        utils::map_any const& pos = boost::any_cast<utils::map_any const>(it);
        face_pos_.push_back( vec2() );
        face_pos_.back().X = pos.I("x");
        face_pos_.back().Y = pos.I("y");
    }
    character_->getAnimSprite("bdeco").set<Visible>( false );
    character_->getAnimSprite("gdeco").set<Visible>( false );

    return shared_from_this();
}

PlayerView& PlayerView::flipPosition()
{
    character_->flipH();
    for( size_t i = 0; i < face_pos_.size(); ++i ) {
        face_pos_[i].X *= -1;
    }
    return *this;
}

PlayerView& PlayerView::switchCharacterState( STATE const& state )
{
    using std::tr1::ref; using std::tr1::bind;

    current_state_ = state;
    CharState stat = states_[state];

    character_->getSprite("face").set<Visible>( stat.face_visible );
    character_->getAnimSprite("body")
               .playAnime( stat.anim, 1000, 0, bind(&PlayerView::clearFaceState, this));

    if( stat.sound != "" )
        audio::Sound::i().playBuffer( stat.sound );

    if( state == STAND ) {
        face_pos_idx_ = 0;
        character_->getSprite("face").set<Pos2D>( face_pos_[0] );
        ctrl::EventDispatcher::i().get_timer_dispatcher( character_->getSceneName() )->subscribe(
            std::tr1::bind(&PlayerView::faceUpdate, this), shared_from_this(), 250, 2);

        // 2011.12.19 FUCK!!!!!!!!!!!!!!!!!!
        // YOU CAN'T GET TIMER DISPATCHER BY NAME BECAUSE SOME CLASS CAN BE REUSED ACROSS DIFFERENT SCENE!!!!
        // HENCE SCENE NAME IS NOT RELIABLE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! FUCK!!!!!!!!!!!!!!!!!!!!!
    }
    return *this;
}

PlayerView& PlayerView::switchCharacterFace( FACE const& fstate )
{
    if( current_face_ == fstate ) return *this;
    current_face_ = fstate;

    FaceState face = faces_[fstate];
    character_->getSprite("face").setTexture( face.tex );
    character_->getAnimSprite("bdeco").set<Visible>( face.bdeco );
    character_->getAnimSprite("gdeco").set<Visible>( face.gdeco );
    return *this;
}

PlayerView& PlayerView::clearFaceState() {
    current_state_ = NONE;
    return *this;
}

void PlayerView::faceUpdate() {
    int& idx = face_pos_idx_;
    ++idx;
    if( static_cast<unsigned int>(idx) >= face_pos_.size() ) idx = 0;
    character_->getSprite("face").set<Pos2D>( face_pos_[ face_pos_idx_ ] );
}

void PlayerView::cycle()
{
    if( pMap map = map_.lock() ) {
        int my_garbage    = map->garbage_left() + map->sum_of_all_enemy();
        int enemy_garbage = 0;
        if( map->garbage_lands().size() > 0 ) {
            enemy_garbage = map->garbage_lands().front().lock()->garbage_left();
        }
        int state1p = current_state_;

        if( state1p != HIT && last_my_garbage_ > my_garbage ) {
#ifdef _USE_WIIMOTE_
            if( input_ ) {
                int rumble_factor = last_my_garbage_ - my_garbage;
                if( rumble_factor > 10 ) rumble_factor = 10;
                input_->rumbleWiimote( rumble_factor * 50 ); //unit: millisecond
            }
#endif //_USE_WIIMOTE_
            //switchCharacterState( HIT );
            //delay this effect for 850ms
            ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
                std::tr1::bind(&PlayerView::switchCharacterState, this, HIT), shared_from_this(), 850);
        }
        //2012.05 attack effect logic changed. so the state switching condition changed.
        else if( state1p != ATTACK && state1p != HIT && last_enemy_garbage_ > enemy_garbage )
            switchCharacterState( ATTACK );
        else if( state1p == NONE )
            switchCharacterState( STAND );

// 2012.05 No faces for now.
//        bool self_full = map->has_column_full(), enemy_full = map->enemy_column_full();

//        if( !self_full && !enemy_full ) switchCharacterFace(NORMAL);
//        else switchCharacterFace( self_full ? BAD : GOOD );

        last_my_garbage_    = my_garbage;
        last_enemy_garbage_ = enemy_garbage;
    }
}
