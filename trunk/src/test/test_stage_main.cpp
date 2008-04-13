
#include "view/Scene.hpp"
#include "view/AnimatedSceneObject.hpp"
#include "EasingEquations.hpp"
#include "Accessors.hpp"

#include "EventDispatcher.hpp"
#include "Sound.hpp"
#include "utils/Random.hpp"
#include "utils/dictionary.hpp"
#include "Conf.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace psc;
using namespace easing;
using namespace accessor;

class TestStage{
    typedef std::vector< view::pAnimatedSceneObject > SceneObjList;

public:
    TestStage(){
        //temporary Scene:
        utils::map_any stage = utils::map_any::construct( utils::fetchConfig("config/test_stage.zzml") );

        setup_stage_by_config( stage.S("test_stage") );
        Sound::i().play( stageconf_.S("music"), true);

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestStage::hit_stage_group, this, 1), 3000, -1);

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&TestStage::hit_stage_group, this, 2), 6000, -1);
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

    void cycle(){
        stage_->redraw();
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

private:
    view::pScene stage_;
    utils::map_any stageconf_;
    std::vector< SceneObjList > slists_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <tr1/functional> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::App::i();
    TestStage tester;
    return psc::App::i().run(std::tr1::bind(&TestStage::cycle, &tester));
}
