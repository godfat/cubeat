
#ifndef _SHOOTING_CUBES_APPLICATION_CLASS_
#define _SHOOTING_CUBES_APPLICATION_CLASS_

/* TODO:
   Inputs are temporarily declared in this scope,
   will be removed later. (when player class is out) */

#include "all_fwd.hpp"
#include <tr1/functional>

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

    int run(std::tr1::function<void()> = 0);

    void setLoading(int const& cent);
    void launchMainMenu();
    void launchMultiplayer(std::string const&, std::string const&, std::string const&);
    void launchPuzzle     (std::string const&, std::string const&, int);
    void pause();
    void resume();

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
    presenter::pObject temp_presenter_;

    irr::ITimer* timer_;

    //these are temporarily here.
    ctrl::Input* input1_;
    ctrl::Input* input2_;
};

}  //psc

// #include "detail/member_template/App_run.hpp"

#endif // _SHOOTING_CUBES_APPLICATION_CLASS_
