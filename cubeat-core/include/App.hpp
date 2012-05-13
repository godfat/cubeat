
#ifndef _SHOOTING_CUBES_APPLICATION_CLASS_
#define _SHOOTING_CUBES_APPLICATION_CLASS_

/* TODO:
   Inputs are temporarily declared in this scope,
   will be removed later. (when player class is out) */

#include "all_fwd.hpp"
#include <boost/tr1/functional.hpp>

namespace psc {

namespace ctrl {
class Input;
class TimerDispatcher;
typedef std::tr1::weak_ptr<TimerDispatcher> wpTimerDispatcher;
}

class App
{
public:
    static App& i() {
        static App app;
        return app;
    }

    int run(std::tr1::function<void()> = 0);

    App& init();

    App& setLoading(int const& cent);
    App& launchOpening();
    App& launchOpening2();
    App& launchDemo();
    App& launchMainMenu();
    App& launchMultiplayer(std::string const&, std::string const&, std::string const&, int, int);
    App& launchPuzzle     (std::string const&, std::string const&, int);
    App& pause();
    App& resume();
    App& quit();

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

    bool quit_;

    ctrl::wpTimerDispatcher global_timer_;
};

}  //psc

// #include "detail/member_template/App_run.hpp"

#endif // _SHOOTING_CUBES_APPLICATION_CLASS_
