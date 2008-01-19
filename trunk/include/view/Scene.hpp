#ifndef _SHOOTING_CUBES_SCENE_VIEW_
#define _SHOOTING_CUBES_SCENE_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include <boost/tr1/memory.hpp>

/* TODO:
   Somehow add a node2view conversion here? Not so sure. */

namespace psc { namespace view {

class Scene : public Object, public std::tr1::enable_shared_from_this<Scene>
{
public:
    typedef std::tr1::shared_ptr< Scene > pointer_type;
    static pointer_type create(pObject const parent = pointer_type()) {
        pointer_type p = psc::ObjectPool< Scene >::create();
        p->init(parent);
        return p;
    }

    virtual Scene* clone() const;
    virtual Scene& setTo2DView(int w, int h);
    virtual Scene& setTo3DView(float FoV);
    virtual Scene& activate();
    virtual Scene& deactivate();
    virtual irr::scene::ICameraSceneNode* camera() const;
    virtual ~Scene();

protected:
    void init(pObject const& parent);

protected:
    irr::scene::ICameraSceneNode* camera_;
    irr::scene::ILightSceneNode* light_;
};

typedef Scene::pointer_type pScene;
typedef std::tr1::weak_ptr<Scene> wpScene;

} //view
} //psc

#endif
