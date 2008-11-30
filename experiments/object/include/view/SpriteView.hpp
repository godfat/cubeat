#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "ObjectView.hpp"

class GUIView;

class SpriteView : public ObjectView
{
public:
    //SpriteView(GUIView const* parent);

    virtual SpriteView* clone() const;
    virtual SpriteView& init(ObjectView const*);
    virtual SpriteView& moveTo(int,int);
    virtual SpriteView& setDepth(float);
    virtual SpriteView& moveTween(int,int,int,std::tr1::function<void()> cb = 0, int delay = 0);

    virtual ~SpriteView();

protected:

};

#endif
