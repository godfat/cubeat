
#ifndef _SHOOTING_CUBES_CUBE_VIEW_
#define _SHOOTING_CUBES_CUBE_VIEW_

#include "view/SceneObjectView.hpp"

class CubeView : public SceneObjectView
{
public:
    CubeView():SceneObjectView("cube"){}

    virtual CubeView* clone() const;
    virtual CubeView& init(ObjectView const*);

    virtual void ownerHit();
    virtual void enemyHit();

    virtual ~CubeView(){}

private:

};

#endif