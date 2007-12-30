
#ifndef _SHOOTING_CUBES_MAP_VIEW_
#define _SHOOTING_CUBES_MAP_VIEW_

#include "view/ObjectView.hpp"

#include <map>
#include <vector>

namespace irr{
namespace scene{
class ISceneNode;
}
}

class CubeView;

class MapView : public ObjectView
{
public:
    MapView(int index):index_(index){}

    virtual MapView* clone() const;
    virtual MapView& init(ObjectView const*);
    virtual MapView& addCube(CubeView*);

protected:
    virtual void ownerHitCallback(int x, int y);
    virtual void enemyHitCallback(int x, int y);

protected:
    std::map< irr::scene::ISceneNode*, CubeView* > node2view_;
    std::vector< CubeView* > cubes_;
    int index_;
};

#endif