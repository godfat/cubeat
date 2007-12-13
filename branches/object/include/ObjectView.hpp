#ifndef _SHOOTING_CUBES_OBJECT_VIEW_
#define _SHOOTING_CUBES_OBJECT_VIEW_

#include "boost/function.hpp"

class ObjectView
{
public:
    ObjectView(ObjectView const* parent = 0);

    virtual ObjectView* clone() const;
/*
    void hit_reaction( something );
*/
    virtual void moveTo(int,int,int);
    virtual void moveTo(int,int,int,int,boost::function<void()> cb = NULL);

    virtual irr::scene::ISceneNode* body() const;
/*
    void step1();
    void step2();
    void step3();
    void step4();
*/
    virtual ~ObjectView();

protected:
    irr::scene::ISceneManager* smgr_;
    irr::scene::ISceneNode* body_;
//    irr::video::SMaterial mat_;
};

#endif