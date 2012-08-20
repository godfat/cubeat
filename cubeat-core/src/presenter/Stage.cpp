
#include "presenter/Stage.hpp"
#include "view/Scene.hpp"
#include "view/AnimatedSceneObject.hpp"
#include "EasingEquations.hpp"
#include "Accessors.hpp"
#include "audio/Sound.hpp"
#include "audio/SoundObject.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Conf.hpp"

#include <boost/foreach.hpp>

using namespace irr;
using namespace video;
using namespace scene;

using namespace psc;
using namespace presenter;
using namespace easing;
using namespace accessor;

Stage::Stage() : need_release_(false)
{
    audio::Sound::i().stopAll();
}

Stage::~Stage()
{
    if( need_release_ ) {
        std::cerr << "Releasing Scene: " << conf_.S("name") << std::endl;
        IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
        IMeshCache*   mcache = IrrDevice::i().d()->getSceneManager()->getMeshCache();

        BOOST_FOREACH(SceneObjList& slist, slists_) {
            BOOST_FOREACH(view::pAnimatedSceneObject& obj, slist) {
                IAnimatedMeshSceneNode* body = static_cast<IAnimatedMeshSceneNode*>(obj->body());
                for(unsigned int i=0; i < body->getMaterialCount(); ++i ) {
                    for(unsigned int j=0; j < MATERIAL_MAX_TEXTURES; ++j ){
                        ITexture* tex = body->getMaterial(i).getTexture(j);
                        if( tex )
                            driver->removeTexture( tex );
                    }
                }
                mcache->removeMesh( body->getMesh() );
            }
        }
        std::cerr << "Releasing Scene: " << conf_.S("name") << "... done." << std::endl;
    }
}

//tell engine to release some of the cached resources.
Stage& Stage::releaseResource()
{
    need_release_ = true;
    return *this;
}

pStage Stage::init( std::string const& path )
{
    conf_ = Conf::i().config_of(path);
    scene_ = view::Scene::create( conf_.S("name") );
    scene_->setTo3DView( conf_.I("FoV") / 180.f * PI );
    utils::vector_any const& lists = conf_.V("all_items");

    BOOST_FOREACH(utils::any_type const& list, lists) {
        slists_.push_back( SceneObjList() );
        SceneObjList& slist = slists_.back();
        utils::vector_any const& items = boost::any_cast<utils::vector_any const>(list);

        AnimList anim_list;

        BOOST_FOREACH(utils::any_type const& it, items) {
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

            //put animation data into the cache:
            AnimSets anim_sets;
            BOOST_FOREACH(utils::pair_any const& it, item.M("anim")) {
                std::string const& k = boost::any_cast<std::string const>(it.first);
                utils::map_any const& m = boost::any_cast<utils::map_any const>(it.second);
                anim_sets[k].s = m.I("s");
                anim_sets[k].e = m.I("e");
                anim_sets[k].dur = m.I("duration");
                anim_sets[k].loop= m.I("loop");
            }
            anim_list.push_back( anim_sets );
        }
        anim_group_.push_back( anim_list );
    }

    music_path_a_ = conf_.S("music");
    music_path_b_ = conf_.S("music2");
    fmusic_path_a_= conf_.S("fast_music");
    fmusic_path_b_= conf_.S("fast_music2");

    return shared_from_this();
}

//Stage& Stage::loadBGM()
//{
//    try {
//        audio::Sound::i().loadBGM_AB( conf_.S("music"), conf_.S("music2") );
//    }
//    catch(boost::bad_any_cast&) { // GDC 2012 Mockup: We should really start to use Lua for everything scriptable....
//        audio::Sound::i().loadBGM( conf_.S("music") );
//    }
//
//    try {
//        audio::Sound::i().loadBGM_AB( conf_.S("fast_music"), conf_.S("fast_music2") );
//    }
//    catch(boost::bad_any_cast&) { // GDC 2012 Mockup: We should really start to use Lua for everything scriptable....
//        audio::Sound::i().loadBGM( conf_.S("music") );
//    }
//    return *this;
//}

Stage& Stage::hitGroup(int const& id)
{
    SceneObjList& slist = slists_[id];
    for( size_t i = 0; i < slist.size(); ++i ) {
        view::pAnimatedSceneObject& obj = slist[i];

        AnimParam hconf = anim_group_[id][i]["hit"];
        AnimParam rconf = anim_group_[id][i]["recover"];
        AnimParam iconf = anim_group_[id][i]["idle"];

        data::AnimatorParam<OBounce, Frame> hit;
        data::AnimatorParam<Linear, Frame> recover;
        data::AnimatorParam<Linear, Frame> idle;

        int fn = obj->get<Frame>(), s = fn < hconf.s ? hconf.s : fn;

        hit.start( s ).end( hconf.e ).duration( hconf.dur );
        recover.start( rconf.s ).end( rconf.e ).duration( rconf.dur );
        idle.start( iconf.s ).end( iconf.e ).duration( iconf.dur ).loop(-1);

        obj->clearAllTween().queue(hit).queue(recover).tween(idle);
    }
    return *this;
}

Stage& Stage::playBGM(time_t const& fade_t)
{
    audio::Sound::i().seek_and_playBGM_AB(music_path_a_, music_path_b_, 1000, fade_t);
    return *this;
}

Stage& Stage::playFastBGM(time_t const& fade_t)
{
    audio::Sound::i().playBGM("smb_warning.wav", 100);
    ctrl::EventDispatcher::i().get_timer_dispatcher("game")->subscribe(
        std::tr1::bind(&audio::Sound::seek_and_playBGM_AB, &audio::Sound::i(), fmusic_path_a_, fmusic_path_b_, 850, fade_t),
        shared_from_this(),
        2250);

    return *this;
}

Stage& Stage::playNormalBGM_without_leading(time_t const& lead_t, time_t const& fade_t)
{
    audio::Sound::i().seek_and_playBGM_AB(music_path_a_, music_path_b_, lead_t, fade_t);
    return *this;
}

void Stage::cycle()
{
    scene_->redraw();
}
