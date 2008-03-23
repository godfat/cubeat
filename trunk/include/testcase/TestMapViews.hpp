
#ifndef _SHOOTING_CUBES_TEST_MAP_VIEWS_
#define _SHOOTING_CUBES_TEST_MAP_VIEWS_

#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "all_fwd.hpp"

#include "presenter/Map.hpp"
#include "data/MapSetting.hpp"
#include "presenter/cube/ViewSprite.hpp"
#include "presenter/cube/ViewStdout.hpp"

using namespace psc;

class TestMapViews{
public:
    TestMapViews(){
        scene_ = psc::view::Scene::create("TestMapViewScene");
        scene_->setTo2DView();

        data::pMapSetting setting(new data::MapSetting);
        setting->y_offset(500);
        map_ = presenter::Map::create(setting);

        map_->push_view( presenter::cube::ViewStdoutMaster::create() );
        map_->push_view( presenter::cube::ViewSpriteMaster::create(scene_, setting) );

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
