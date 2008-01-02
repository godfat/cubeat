#ifndef _SHOOTING_CUBES_SCENE_VIEW_
#define _SHOOTING_CUBES_SCENE_VIEW_

#include "view/Object.hpp"

namespace view {

class Scene : public Object
{
public:
    //Scene(Object const* parent = 0);
    virtual void init(Object const* parent = 0);

    virtual Scene* clone() const;
    virtual Scene& setTo2DView(int w, int h);
    virtual Scene& setTo3DView(float FoV);
    virtual Scene& activate();
    virtual Scene& deactivate();
    virtual irr::scene::ICameraSceneNode* camera() const;
    virtual ~Scene();

private:
    irr::scene::ICameraSceneNode* camera_;
    irr::scene::ILightSceneNode* light_;
};

} //view

#endif
