#ifndef _SHOOTING_CUBES_OBJECT_VIEW_
#define _SHOOTING_CUBES_OBJECT_VIEW_

#include "boost/function.hpp"

class ObjectView
{
public:
    ObjectView(ObjectView const* parent = 0);

    virtual ObjectView* clone() const;

    virtual void moveTo(int,int,int);
    virtual void moveTo(int,int,int,int,boost::function<void()> cb = 0);

    virtual irr::scene::ISceneNode* body() const;

    virtual ~ObjectView();

protected:
    irr::scene::ISceneManager* smgr_;
    irr::scene::ISceneNode* body_;
};

#endif