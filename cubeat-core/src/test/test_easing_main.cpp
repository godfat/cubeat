
#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "Conf.hpp"

#include "EasingEquations.hpp"
#include "Accessors.hpp"

using namespace psc;
using namespace easing;
using namespace accessor;

class TestStage {

public:
    TestStage(){
        scene_ = view::Scene::create("game");
        scene_->setTo2DView();

        sprites_.push_back( view::Sprite::create("cubes/cube-b-1", scene_, 64, 64, true) );
        sprites_[0]->tween<Linear, Pos2D>(vec2(50, 50), vec2(1100, 50), 3000u, -1);

        sprites_.push_back( view::Sprite::create("cubes/cube-g-1", scene_, 64, 64, true) );
        sprites_[1]->tween<IQuad, Pos2D>(vec2(50, 130), vec2(1100, 130), 3000u, -1);

        sprites_.push_back( view::Sprite::create("cubes/cube-r-1", scene_, 64, 64, true) );
        sprites_[2]->tween<ICubic, Pos2D>(vec2(50, 210), vec2(1100, 210), 3000u, -1);

        sprites_.push_back( view::Sprite::create("cubes/cube-y-1", scene_, 64, 64, true) );
        sprites_[3]->tween<IQuart, Pos2D>(vec2(50, 290), vec2(1100, 290), 3000u, -1);

        sprites_.push_back( view::Sprite::create("cubes/cube-b-1", scene_, 64, 64, true) );
        sprites_[4]->tween<IOExpo, Pos2D>(vec2(50, 370), vec2(1100, 370), 3000u, -1);

        sprites_.push_back( view::Sprite::create("cubes/cube-g-1", scene_, 64, 64, true) );
        sprites_[5]->tween<SineCirc, Pos2D>(vec2(50, 450), vec2(1100, 450), 3000u, -1);

        sprites_.push_back( view::Sprite::create("cubes/cube-r-1", scene_, 64, 64, true) );
        sprites_[6]->tween<Linear, Pos2D>(vec2(50, 530), vec2(1100, 530), 3000u, -1);

        sprites_.push_back( view::Sprite::create("cubes/cube-y-1", scene_, 64, 64, true) );
        sprites_[7]->tween<OElastic, Pos2D>(vec2(50, 610), vec2(1100, 610), 3000u, -1);

        sprites_.push_back( view::Sprite::create("cubes/cube-b-1", scene_, 64, 64, true) );
        sprites_[8]->tween<OBack, Pos2D>(vec2(50, 680), vec2(1100, 680), 3000u, -1);

        for( size_t i = 0 ; i < sprites_.size(); ++i ) {
            sprites_[i]->tween<Linear, Rotation>(vec3(0, 0, 0), vec3(0, 0, 360), 3000u, -1);
        }
    }

    void cycle(){
        scene_->redraw();
    }

private:
    view::pScene scene_;
    std::vector<view::pSprite> sprites_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i().init();
    TestStage tester;
    return psc::App::i().run(std::tr1::bind(&TestStage::cycle, &tester));
}
