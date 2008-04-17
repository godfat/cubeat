
#include "presenter/PlayerView.hpp"
#include "presenter/Map.hpp"
#include "view/AnimatedSprite.hpp"
#include "view/Menu.hpp"
#include "EventDispatcher.hpp"
#include "EasingEquations.hpp"
#include "Accessors.hpp"
#include "Sound.hpp"

using namespace psc;
using namespace presenter;
using namespace easing;
using namespace accessor;

PlayerView::PlayerView()
    :last_garbage_(0)
{
}

PlayerView::~PlayerView()
{
}

pPlayerView PlayerView::init(std::string const& path, view::pObject const& parent, vec2 const& pos)
{
    conf_ = utils::map_any::construct( utils::fetchConfig( path ) );
    utils::map_any const& anim = conf_.M("anim_attr");

    character_ = view::Menu::create("", parent, 1, 1, true);
    character_->set<Pos2D>( pos );

    BOOST_FOREACH(utils::pair_any const& it, anim) {
        std::string    const& key  = boost::any_cast<std::string const>(it.first);
        utils::map_any const& attr = boost::any_cast<utils::map_any const>(it.second);
        character_->addAnimSprite(key, attr.S("anim"), 0, attr.I("w"), attr.I("h"), true )
                   .getAnimSprite(key).playAnime( attr.S("defanim"), attr.I("ms"), attr.I("loop") )
                                      .set<Pos2D>( vec2(attr.I("x"), attr.I("y")) );
    }
    utils::map_any const& fpos = conf_.V("face_pos").M(0);
    utils::map_any const& face = conf_.M("face");
    character_->addSprite("face", 0, conf_.I("face_w"), conf_.I("face_h"), true, face.M(NORMAL).S("tex") )
               .getSprite("face").set<Pos2D>( vec2(fpos.I("x"), fpos.I("y")) );

    clearFaceState();
    conf_["current_face"] = static_cast<int>(NORMAL);
    return shared_from_this();
}

PlayerView& PlayerView::flipPosition()
{
    character_->flipH();
    for( size_t i = 0; i < conf_.V("face_pos").size(); ++i ) {
        utils::map_any& pos = conf_.V("face_pos").M(i);
        pos.I("x") *= -1;
    }
    return *this;
}

PlayerView& PlayerView::switchCharacterState( STATE const& state )
{
    using std::tr1::ref; using std::tr1::bind;
    conf_["current_state"] = static_cast<int>(state);
    utils::map_any const& attr = conf_.M("state").M(state);
    character_->getSprite("face").set<Visible>( attr.I("face_visible") );
    character_->getAnimSprite("body")
               .playAnime( attr.S("anim"), 1000, 0, bind(&PlayerView::clearFaceState, this));
    if( attr.S("sound") != "" )
        Sound::i().play( attr.S("sound") );
    if( state == STAND ) {
        conf_["face_pos_idx"] = 0;
        utils::vector_any& face_pos = conf_.V("face_pos");
        character_->getSprite("face").set<Pos2D>( vec2( face_pos.M(0).I("x"), face_pos.M(0).I("y") ) );
        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&PlayerView::faceUpdate, this), shared_from_this(), 250, 2);
    }
    return *this;
}

PlayerView& PlayerView::switchCharacterFace( FACE const& fstate )
{
    if( conf_["current_face"] == fstate ) return *this;
    conf_["current_face"] = static_cast<int>(fstate);
    utils::map_any const& attr = conf_.M("face").M(fstate);
    character_->getSprite("face").setTexture( attr.S("tex") );
    character_->getAnimSprite("bdeco").set<Visible>( attr.I("bdeco") );
    character_->getAnimSprite("gdeco").set<Visible>( attr.I("gdeco") );
    return *this;
}

PlayerView& PlayerView::clearFaceState() {
    conf_["current_state"] = static_cast<int>(NONE);
    return *this;
}

void PlayerView::faceUpdate() {
    int& idx = conf_.I("face_pos_idx");
    utils::vector_any& face_pos = conf_.V("face_pos");
    ++idx;
    if( static_cast<unsigned int>(idx) >= face_pos.size() ) idx = 0;
    character_->getSprite("face").set<Pos2D>( vec2( face_pos.M(idx).I("x"), face_pos.M(idx).I("y") ) );
}

void PlayerView::cycle()
{
    if( pMap map = map_.lock() ) {
        int new_garbage = map->garbage_left() + map->sum_of_all_enemy();
        int state1p = conf_.I("current_state");
        if( state1p != HIT && last_garbage_ > new_garbage )
            switchCharacterState( HIT );
        else if( state1p != ATTACK && state1p != HIT && map->current_sum_of_attack() > 1 )
            switchCharacterState( ATTACK );
        else if( state1p == NONE )
            switchCharacterState( STAND );

        bool self_full = map->has_column_full(), enemy_full = map->enemy_column_full();

        if( !self_full && !enemy_full ) switchCharacterFace(NORMAL);
        else switchCharacterFace( self_full ? BAD : GOOD );

        last_garbage_ = new_garbage;
    }
}
