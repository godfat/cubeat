
/*2007.11.9
  Deleted things coupled with Virtools.
  This item cannot compile unless further changes made.

  This class is now more or less like an interface.
*/

#ifndef _SHOOTING_CUBES_SOUND_
#define _SHOOTING_CUBES_SOUND_

#include <string>

namespace irrklang {
class ISoundEngine;
}

namespace psc {

class Sound
{
public:
    //There will be an interface change.
    static Sound& i() {
        static Sound singleton;
        return singleton;
    }

/*    virtual void pistol()          = 0;
    virtual void heavypistol()     = 0;
    virtual void machinegun()      = 0;
    virtual void heavymachinegun() = 0;
    virtual void rocket()          = 0;
    virtual void out_of_ammo()     = 0;

    virtual void normal_cube()     = 0;
    virtual void rock_cube()       = 0;
    virtual void broken_cube()     = 0;
    virtual void item_box()        = 0;
    virtual void reload()          = 0;
    virtual void change_wep()      = 0;

    virtual void cube_create()     = 0;
    virtual void cube_dropped()    = 0;
    virtual void combo(int num)    = 0;

    virtual void game_start()      = 0;
    virtual void game_pause()      = 0;
    virtual void game_win()        = 0;
    virtual void game_lose()       = 0;

    virtual void countdown()       = 0;
    virtual void emergency()       = 0;
    virtual void sp_event()        = 0;

    virtual void get_focus()       = 0;
    virtual void proceed()         = 0;
    virtual void cancel()          = 0;
    virtual void quit()            = 0; */

    Sound& play(std::string const& path, bool loop = false);
    Sound& stopAll();
    Sound& pauseAll(bool);

    void init();
    ~Sound();

private:
    Sound();

private:
    irrklang::ISoundEngine* engine_;
    std::string base_path_;
};

} //psc

#endif
