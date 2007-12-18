#ifndef _SHOOTING_CUBES_OBJECT_VIEW_
#define _SHOOTING_CUBES_OBJECT_VIEW_

namespace irr{
namespace scene{
class ISceneManager;
class ISceneNode;
}
}

class ObjectView
{
public:
    ObjectView(ObjectView const* parent = 0);

    virtual ObjectView* clone() const;

    virtual void moveTo(int,int,int);

    virtual irr::scene::ISceneNode* body() const;

    virtual ~ObjectView();

protected:
    irr::scene::ISceneManager* smgr_;
    irr::scene::ISceneNode* body_;
};

#endif