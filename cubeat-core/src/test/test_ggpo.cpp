
#include "view/Scene.hpp"
#include "view/Sprite.hpp"
#include "EventDispatcher.hpp"
#include "ctrl/TimerDispatcher.hpp"
#include "utils/dictionary.hpp"
#include "utils/Random.hpp"
#include "Conf.hpp"

using namespace psc;

class TestGGPO{
public:
    TestGGPO(): frame_(0), rollbacked_(false) {
        scene_ = view::Scene::create("game");
        scene_->setTo2DView();
        utils::Random2::i().init(12345);
    }

    void update_state(int f) {
        printf("Frame: %d\n", f);
        utils::Random2::i().set_frame(f);

        std::vector<double> chances;
        chances.push_back(40);
        chances.push_back(40);
        chances.push_back(20);

        printf("%d %d %d %d %d\n", utils::r_random(50),
                                   utils::r_random(100),
                                   utils::r_random(150),
                                   utils::distributed_random(chances),
                                   utils::r_random(25));
    }

    void cycle(){
        ++frame_;
        if( frame_ == 3 ) {
            printf("rollbacked!!\n");
            for( int i = 1; i < 3; ++i ) {
                update_state(i);
            }
        }
        if( frame_ == 5 ) {
            printf("rollbacked!! twice\n");
            for( int i = 2; i < 5; ++i ) {
                update_state(i);
            }
        }
        update_state( frame_ );
        scene_->redraw();
        utils::Random2::i().end_frame();
    }

private:
    view::pScene  scene_;
    view::pSprite sprite_;
    int frame_;
    bool rollbacked_;
};

#include "App.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#include <boost/tr1/functional.hpp> // for bind

int main(){
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    psc::Conf::i().init("");
    psc::App::i().init();
    TestGGPO tester;
    return psc::App::i().run(std::tr1::bind(&TestGGPO::cycle, &tester));
}
