#ifndef _SHOOTING_CUBES_BUTTON_VIEW_
#define _SHOOTING_CUBES_BUTTON_VIEW_

#include "SpriteView.hpp"

class GUIView;

class ButtonView : public SpriteView
{
public:
    //ButtonView(GUIView const* parent);

    virtual ButtonView* clone() const;

    void onPress();

    virtual ~ButtonView();

private:
};

#endif
