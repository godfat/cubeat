#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "ObjectView.hpp"

class GUIView;

class SpriteView : public ObjectView
{
public:
    SpriteView(GUIView const* parent);

    virtual SpriteView* clone() const;

    //temp
    void step1();
    void step2();
    void step3();
    void step4();

    virtual ~SpriteView();

protected:
    irr::video::SMaterial mat_;
};

#endif