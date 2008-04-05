
#include "view/Scene.hpp"
#include "view/Sprite.hpp"

#include "presenter/Map.hpp"
#include "presenter/cube/ViewSprite.hpp"
#include "presenter/cube/ViewStdout.hpp"

#include "EventDispatcher.hpp"

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
        map0_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s) );

        // map0_->push_view( presenter::cube::ViewStdoutMaster::create() );
        // map0_->push_view( presenter::cube::ViewSpriteMaster::create(scene_) );

        // setup map1
        data::pMapSetting set = data::MapSetting::create();
        // set->cube_dropping_duration(2000);
        map1_ = presenter::Map::create(set);
        s = data::ViewSpriteSetting::create(450, 500, 50);
        s->push_ally(0).push_enemy(1);
        map1_->set_view_master( presenter::cube::ViewSpriteMaster::create(scene_, s) );

        // mini view
        map1_->push_view_slave( presenter::cube::ViewSpriteMaster::create(scene_,
            data::ViewSpriteSetting::create(800, 300, 25) ) );

        // setup garbage land
        map0_->push_garbage_land(map1_);
        map1_->push_garbage_land(map0_);

//        // model cycle for 10 fps
//        ctrl::EventDispatcher::i().subscribe_timer(
//            std::tr1::bind(&presenter::Map::cycle_slow, map0_.get()), map0_, 100, -1);
//
//        // model cycle for 10 fps
//        ctrl::EventDispatcher::i().subscribe_timer(
//            std::tr1::bind(&presenter::Map::cycle_slow, map1_.get()), map1_, 100, -1);

        // for testing garbage
        map1_->throw_garbage(15);

        ctrl::EventDispatcher::i().subscribe_timer(
            std::tr1::bind(&presenter::Map::throw_garbage, map1_.get(), 30), map1_, 10000);
    }
    void cycle(){
        scene_->redraw();
//        map0_->redraw().cycle_fast();
//        map1_->redraw().cycle_fast();
        map0_->redraw().cycle();
        map1_->redraw().cycle();
    }

private:
    view::pScene scene_;
    presenter::pMap map0_;
    presenter::pMap map1_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <tr1/functional> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::App::i();
    TestMapViews tester;
    return psc::App::i().run(std::tr1::bind(&TestMapViews::cycle, &tester));
}
