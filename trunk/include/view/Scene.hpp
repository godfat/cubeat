#ifndef _SHOOTING_CUBES_SCENE_VIEW_
#define _SHOOTING_CUBES_SCENE_VIEW_

#include "view/Object.hpp"
#include "utils/ObjectPool.hpp"
#include "all_fwd.hpp"

/* TODO:
   Somehow add a node2view conversion here? Not so sure. */

namespace psc { namespace view {

class Scene : public Object, public std::tr1::enable_shared_from_this<Scene>
{
public:
    typedef std::tr1::shared_ptr< Scene > pointer_type;
    static pointer_type create(pObject const parent = pointer_type()) {
        pointer_type p = utils::ObjectPool< Scene >::create();
        p->init(parent);
        return p;
    }

    virtual Scene* clone() const;
    virtual Scene& setTo2DView();
    virtual Scene& setTo3DView(float FoV);
    virtual Scene& activate();
    virtual Scene& redraw();
    virtual Scene& deactivate();
    virtual irr::scene::ICameraSceneNode* camera() const;
    virtual ~Scene();

protected:
    void init(pObject const& parent);

protected:
    irr::scene::ICameraSceneNode* camera_;
    irr::scene::ILightSceneNode* light_;
};

} //view
} //psc

#endif
