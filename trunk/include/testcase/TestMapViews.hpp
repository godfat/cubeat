
#ifndef _SHOOTING_CUBES_TEST_MAP_VIEWS_
#define _SHOOTING_CUBES_TEST_MAP_VIEWS_

#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "all_fwd.hpp"

class TestMapViews{
public:
    TestMapViews(){
        scene_ = psc::view::Scene::create("TestMapViewScene");
        scene_->setTo2DView();
        
        test_ = psc::view::Sprite::create("title", scene_, 100, 40);
        test_->moveTo(100,40);
    }
    void cycle(){
        scene_->redraw();
    }
private:
    psc::view::pScene scene_;
    psc::view::pSprite test_;
};

#endif
