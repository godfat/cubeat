
#ifndef _SHOOTING_CUBES_SOUND_
#define _SHOOTING_CUBES_SOUND_

#include <string>
#include <map>

namespace psc {
namespace audio {

class SoundObject;

class Sound
{
    typedef std::map  <std::string, SoundObject*>        SoundMap;
    typedef std::pair <std::string const, SoundObject*>  SoundMapPair;
public:
    static Sound& i() {
        static Sound singleton;
        return singleton;
    }

    Sound& load(std::string const& path);
    Sound& play(std::string const& path, bool const& loop = false);
    Sound& stopAll();
    Sound& pauseAll(bool const&);
    Sound& cycle();

    void init();
    ~Sound();

private:
    Sound();

private:
    std::string base_path_;
    bool inited_;

    SoundMap sound_base_;
};

} //audio
} //psc

#endif
