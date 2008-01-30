
#ifndef _SHOOTING_CUBES_APPLICATION_CLASS_
#define _SHOOTING_CUBES_APPLICATION_CLASS_

/* TODO:
   Inputs are temporarily declared in this scope,
   will be removed later. (when player class is out) */

#include "presenter/Transitioner.hpp"
#include "presenter/MainMenu.hpp"

namespace irr {
class ITimer;
}

namespace psc {

namespace ctrl {
class Input;
}

class App
{
public:
    static App& i() {
        static App app;
        return app;
    }

    int run();

    App& setLoading(int const& cent) { trans_->setLoadingBar(cent); return *this; }

    ~App();

private:
    App();
    App(App const&);  //no copy constructor

    bool update_block();

private:
    unsigned int framerate_;
    unsigned int last_timetick_;

    presenter::pTransitioner trans_;
    presenter::pObject master_presenter_;

    irr::ITimer* timer_;

    //these are temporarily here.
    ctrl::Input* input1_;
    ctrl::Input* input2_;
};

}  //psc

#endif // _SHOOTING_CUBES_APPLICATION_CLASS_