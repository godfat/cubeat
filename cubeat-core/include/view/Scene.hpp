#ifndef _SHOOTING_CUBES_SCENE_VIEW_
#define _SHOOTING_CUBES_SCENE_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

#include <string>
#include <map>

namespace psc {

namespace ctrl {
class Button;
class Input;
class TimerDispatcher;
typedef std::tr1::shared_ptr<TimerDispatcher> pTimerDispatcher;
}

namespace view {

class Scene : public Object
{
    typedef std::list<view::wpObject>              ObjList;
    typedef std::map <ctrl::Input const*, ObjList> PickingMap;
public:
    typedef std::tr1::shared_ptr< Scene > pointer_type;
    static pointer_type create(std::string const& name = "unnamed_scene") {
        pointer_type p = utils::ObjectPool< Scene >::create();
        p->init(name);
        return p;
    }

    Scene();

    virtual Scene& setTo2DView();
    virtual Scene& setTo3DView(float FoV = PI/3.0f);
    virtual Scene& activate();
    virtual Scene& redraw();
    virtual Scene& deactivate();
    virtual Scene& allowPicking(bool);
    virtual Scene& enablePostProc();
    virtual irr::scene::ICameraSceneNode* camera() const;
    virtual irr::scene::ISceneCollisionManager* getCollisionMgr() const;
    virtual irr::video::ITexture* getPostProcTexture() const;
    virtual irr::scene::IParticleSystemSceneNode* addParticleNodeTo(view::pObject const& obj = view::pObject(),
                                                                    bool const& use_default = true) const;

    std::list<wpObject> const& pick(vec2 const&);
    std::list<wpObject> const& pick(vec2 const&,
                                    std::tr1::function<bool(vec2 const&, vec2 const&)> const&);

    void addPickMapping(irr::scene::ISceneNode*, wpObject const&);
    void removePickMapping(irr::scene::ISceneNode*);
    void enableGlobalHittingEvent();
    void update_focus_objs_by_input(ctrl::Input const*);   //this will be called inside EventDispatcher.

    std::string getName() const { return name_; }
    std::time_t getTime() const;

    virtual ~Scene();

protected:
    void init(std::string const&);
    void onPressEvent(int, int, ctrl::Button const*);
    void onDownEvent(int, int, ctrl::Button const*);
    void processHit(int, int, ctrl::Button const*);

protected:
    typedef std::pair<irr::scene::ISceneNode* const, wpObject> Node2ViewPair;
    bool allow_picking_;
    irr::scene::ILightSceneNode*  light_;
    irr::scene::ICameraSceneNode* camera_;
    irr::scene::ICameraSceneNode* rt_camera_;
    irr::scene::ISceneNode*       rt_plane_;
    irr::video::ITexture*         rt_;
    std::map<irr::scene::ISceneNode*, wpObject> node2view_;
    std::list<wpObject> picked_temporary_;   //this list will be wiped every time you call "pick"
                                             //so there must be a copy in the outside world
    PickingMap pickmap_, last_pickmap_;
    ctrl::pTimerDispatcher timer_;
};

} //view
} //psc

#endif
