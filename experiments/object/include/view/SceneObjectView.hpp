
#ifndef _SHOOTING_CUBES_SCENE_OBJECT_VIEW_
#define _SHOOTING_CUBES_SCENE_OBJECT_VIEW_

#include "view/ObjectView.hpp"

#include <string>

class SceneObjectView : public ObjectView
{
public:
    SceneObjectView(std::string path):path_(path){}

    virtual SceneObjectView* clone() const;
    virtual SceneObjectView& init(ObjectView const*);

    virtual ~SceneObjectView(){}

private:
    std::string path_;
};

#endif