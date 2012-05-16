
#ifndef _SHOOTING_CUBES_SOUND_
#define _SHOOTING_CUBES_SOUND_

#include <string>
#include <list>
#include <map>
#include <boost/tr1/memory.hpp>

namespace psc {
namespace audio {

class SoundStream;
class SoundBuffer;
class SoundObject;
typedef std::tr1::shared_ptr<SoundStream> pSoundStream;
typedef std::tr1::weak_ptr<SoundStream> wpSoundStream;
typedef std::tr1::shared_ptr<SoundBuffer> pSoundBuffer;
typedef std::tr1::weak_ptr<SoundBuffer> wpSoundBuffer;
typedef std::tr1::shared_ptr<SoundObject> pSoundObject;
typedef std::tr1::weak_ptr<SoundObject> wpSoundObject;

class Sound
{
    typedef std::map  <std::string, pSoundStream>        SoundStreamMap;
    typedef std::pair <std::string const, pSoundStream>  SoundStreamMapPair;
    typedef std::map  <std::string, pSoundBuffer>        SoundBufferMap;
    typedef std::pair <std::string const, pSoundBuffer>  SoundBufferMapPair;
    typedef std::list <pSoundObject>                     SoundList;
    typedef std::list <SoundList::iterator>              SoundListRemoval;
public:
    static Sound& i() {
        static Sound singleton;
        return singleton;
    }

    Sound& loadStream(std::string const& path);
    Sound& loadBuffer(std::string const& path);
    Sound& playStream(std::string const& path, bool const& loop = false);
    Sound& playBuffer(std::string const& path, bool const& loop = false);
    Sound& playABStream(std::string const& path_a, std::string const& path_b);
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

    SoundStreamMap sound_streams_;
    SoundBufferMap sound_buffers_;
    SoundList      sound_list_;
    SoundListRemoval sound_to_be_cleared_;
};

} //audio
} //psc

#endif
