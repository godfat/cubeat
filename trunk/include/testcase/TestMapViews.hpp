
#ifndef _SHOOTING_CUBES_TEST_MAP_VIEWS_
#define _SHOOTING_CUBES_TEST_MAP_VIEWS_

#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "all_fwd.hpp"

#include "presenter/Map.hpp"
#include "presenter/cube/ViewSprite.hpp"
#include "presenter/cube/ViewStdout.hpp"

using namespace psc;

class TestMapViews{
public:
    TestMapViews(){
        scene_ = psc::view::Scene::create("TestMapViewScene");
        scene_->setTo2DView();

        data::pViewSpriteSetting s;

        // setup map0
        map0_ = presenter::Map::create();
        s = data::ViewSpriteSetting::create(50, 500, 50);
        s->push_ally(1).push_enemy(0);
        map0_->push_view( presenter::cube::ViewSpriteMaster::create(scene_, s) );

        map0_->push_view( presenter::cube::ViewStdoutMaster::create() );
        // map0_->push_view( presenter::cube::ViewSpriteMaster::create(scene_) );

        // setup map1
        map1_ = presenter::Map::create();
        s = data::ViewSpriteSetting::create(450, 500, 50);
        s->push_ally(0).push_enemy(1);
        map1_->push_view( presenter::cube::ViewSpriteMaster::create(scene_, s) );

        // mini view
        map1_->push_view( presenter::cube::ViewSpriteMaster::create(scene_,
            data::ViewSpriteSetting::create(800, 300, 25) ) );

        // setup garbage land
        map0_->push_garbage_land(map1_);
        map1_->push_garbage_land(map0_);
    }
    void cycle(){
        scene_->redraw();
        map0_->redraw().cycle();
        map1_->redraw().cycle();
    }

private:
    view::pScene scene_;
    presenter::pMap map0_;
    presenter::pMap map1_;
};

#endif
