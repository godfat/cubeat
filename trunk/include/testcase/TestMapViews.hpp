
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

        map_ = presenter::Map::create();

        // map_->push_view( presenter::cube::ViewStdoutMaster::create() );
        map_->push_view( presenter::cube::ViewSpriteMaster::create(scene_) );
        map_->push_view( presenter::cube::ViewSpriteMaster::create(scene_,
            data::ViewSpriteSetting::create(50, 100, 400) ) );
        map_->push_view( presenter::cube::ViewSpriteMaster::create(scene_,
            data::ViewSpriteSetting::create(25, 450, 400) ) );

        map_->push_garbage_land(map_);
        map_->throw_garbage(10);

        // test_ = psc::view::Sprite::create("title", scene_, 100, 40);
        // test_->moveTo(100,40);
    }
    void cycle(){
        scene_->redraw();
        map_->redraw().cycle();
    }

private:
    view::pScene scene_;
    presenter::pMap map_;
};

#endif
