
#ifndef _SHOOTING_CUBES_TEST_MAP_VIEWS_
#define _SHOOTING_CUBES_TEST_MAP_VIEWS_

#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "all_fwd.hpp"

#include "presenter/Map.hpp"
#include "data/MapSetting.hpp"
#include "presenter/cube/ViewSprite.hpp"
#include "presenter/cube/ViewStdout.hpp"

#include <tr1/functional>

using namespace psc;

class TestMapViews{
public:
    TestMapViews(){
        scene_ = psc::view::Scene::create("TestMapViewScene");
        scene_->setTo2DView();

        map_ = presenter::Map::create(data::pMapSetting(new data::MapSetting));

        map_->push_view( std::make_pair(
            &presenter::cube::ViewStdout::create,
            &presenter::cube::ViewStdout::redraw) );

        using namespace std::tr1::placeholders;
        map_->push_view( std::make_pair(
            std::tr1::bind(&presenter::cube::ViewSprite::create,
                _1, scene_, map_->setting()),
            &presenter::cube::ViewSprite::redraw) );

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
