#ifndef _SHOOTING_CUBES_GUI_VIEW_
#define _SHOOTING_CUBES_GUI_VIEW_

#include "ObjectView.hpp"

class GUIView : public ObjectView
{
public:
    GUIView(ObjectView const* parent = 0);

    virtual GUIView* clone() const;

    virtual irr::scene::ICameraSceneNode* camera() const;
    virtual ~GUIView();

private:
    irr::scene::ICameraSceneNode* camera_;
};

#endif