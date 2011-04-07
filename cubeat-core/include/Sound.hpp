
/*2007.11.9
  Deleted things coupled with Virtools.
  This item cannot compile unless further changes made.

  This class is now more or less like an interface.
*/

#ifndef _SHOOTING_CUBES_SOUND_
#define _SHOOTING_CUBES_SOUND_

#include <string>
#include <map>

namespace psc {
namespace audio {

class SoundObject;

class SoundMgr
{
    typedef std::map  <std::string, SoundObject*>        SoundMap;
    typedef std::pair <std::string const, SoundObject*>  SoundMapPair;
public:
    static SoundMgr& i() {
        static SoundMgr singleton;
        return singleton;
    }

    SoundMgr& play(std::string const& path, bool loop = false);
    SoundMgr& stopAll();
    SoundMgr& pauseAll(bool);
    SoundMgr& cycle();

    void init();
    ~SoundMgr();

private:
    SoundMgr();

private:
    std::string base_path_;
    bool inited_;

    SoundMap sound_base_;
};

} //audio
} //psc

#endif
