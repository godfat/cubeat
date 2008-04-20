
#include "presenter/Stage.hpp"
#include "view/Scene.hpp"
#include "view/AnimatedSceneObject.hpp"
#include "EasingEquations.hpp"
#include "Accessors.hpp"
#include "Sound.hpp"

#include <boost/foreach.hpp>

using namespace psc;
using namespace presenter;
using namespace easing;
using namespace accessor;

Stage::Stage()
{
    Sound::i().stopAll();
}

Stage::~Stage()
{
}

pStage Stage::init( std::string const& path )
{
    conf_ = utils::map_any::construct( utils::fetchConfig( path ) );
    scene_ = view::Scene::create( conf_.S("name") );
    scene_->setTo3DView( conf_.I("FoV") / 180.f * PI );
    utils::vector_any const& lists = conf_.V("all_items");
    BOOST_FOREACH(boost::any const& list, lists) {
        slists_.push_back( SceneObjList() );
        SceneObjList& slist = slists_.back();
        utils::vector_any const& items = boost::any_cast<utils::vector_any const>(list);
        BOOST_FOREACH(boost::any const& it, items) {
            utils::map_any const& item = boost::any_cast<utils::map_any const>(it);
            view::pAnimatedSceneObject obj;
            obj = view::AnimatedSceneObject::create( item.S("xfile") ,scene_ );

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
    return shared_from_this();
}

Stage& Stage::hitGroup(int const& id)
{
    SceneObjList& slist = slists_[id];
    utils::vector_any const& listconf = conf_.V("all_items").V(id);
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
    return *this;
}

Stage& Stage::playBGM()
{
    Sound::i().play( conf_.S("music"), true );
    return *this;
}

void Stage::cycle()
{
    scene_->redraw();
}
