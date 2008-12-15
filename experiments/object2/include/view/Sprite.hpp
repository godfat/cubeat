#ifndef _SHOOTING_CUBES_SPRITE_VIEW_
#define _SHOOTING_CUBES_SPRITE_VIEW_

#include "view/Object.hpp"

namespace view {

class SceneView;

class Sprite : public Object
{
public:
    //Sprite(SceneView const* parent);
    virtual void init(Object const*);

    virtual Sprite* clone() const;
    virtual Sprite& moveTo(int,int);
    virtual Sprite& setDepth(float);
    virtual Sprite& moveTween(int,int,int,std::tr1::function<void()> cb = 0, int delay = 0);

    virtual ~Sprite();

protected:

};

} //view

#endif
