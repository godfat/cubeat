
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
class SoundSample;
class SoundObject;
typedef std::tr1::shared_ptr<SoundStream> pSoundStream;
typedef std::tr1::weak_ptr<SoundStream> wpSoundStream;
typedef std::tr1::shared_ptr<SoundBuffer> pSoundBuffer;
typedef std::tr1::weak_ptr<SoundBuffer> wpSoundBuffer;
typedef std::tr1::shared_ptr<SoundSample> pSoundSample;
typedef std::tr1::weak_ptr<SoundSample> wpSoundSample;
typedef std::tr1::shared_ptr<SoundObject> pSoundObject;
typedef std::tr1::weak_ptr<SoundObject> wpSoundObject;

class Sound
{
    typedef std::map  <std::string, pSoundStream>        SoundStreamMap;
    typedef std::pair <std::string const, pSoundStream>  SoundStreamMapPair;
    typedef std::map  <std::string, pSoundBuffer>        SoundBufferMap;
    typedef std::pair <std::string const, pSoundBuffer>  SoundBufferMapPair;
    typedef std::map  <std::string, pSoundSample>        SoundSampleMap;
    typedef std::pair <std::string const, pSoundSample>  SoundSampleMapPair;
    typedef std::list <pSoundObject>                     SoundList;
    typedef std::list <SoundList::iterator>              SoundListRemoval;
public:
    static Sound& i() {
        static Sound singleton;
        return singleton;
    }

    //Sound& playStream(std::string const& path, bool const& loop = false);
    Sound& playBuffer(std::string const& path, bool const& loop = false);
    Sound& playSample(std::string const& path, time_t const& fade_t = 0, int const& loop = 0);

    //just play this if you want to play a single music track
    Sound& playBGM_AB(std::string const& path_a, std::string const& path_b, time_t const& fade_t = 0);
    Sound& playBGM(std::string const& path, time_t const& fade_t = 0, int const& loop = 0);

    Sound& seek_and_playBGM_AB(std::string const& pa, std::string const& pb, time_t const& seekms, time_t const& fade_t = 0);
    Sound& seek_and_playBGM(std::string const& p, time_t const& seekms, time_t const& fade_t = 0, int const& loop = 0);

    Sound& stopAll();
    Sound& pauseAll(bool const&);
    Sound& cycle();

    void check_sound_volumes();

    void init();
    ~Sound();

private:
    Sound();

    Sound& loadStream(std::string const& path);
    Sound& loadBuffer(std::string const& path);
    Sound& loadSample(std::string const& path);

    //load is internal. you essentially just play. it returns "next" main_track_.
    int loadBGM_AB(std::string const& path_a, std::string const& path_b);
    int loadBGM(std::string const& path);

    void trackFlip(time_t const& fade_t = 0, int const& loop = 0, int const& seek = 0);
    //This may very well be extended to a multi-track system, abstracted from AL channels and sources that,
    //when you need arbitrary 3 or more BGM stand-by and somehow play them in-turn
    //However, what is the use case? I never bump into one.

    void exchange(pSoundObject const& before, pSoundObject const& after, time_t const& t, int const& loop, int const& seek = 0);
    void internal_so_init(pSoundObject& s, std::string const& a, std::string const& b = std::string());

private:
    std::string base_path_;
    bool inited_;
    int main_track_;

    SoundStreamMap sound_streams_;
    //SoundBufferMap sound_buffers_;
    SoundSampleMap sound_buffers_;
    SoundSampleMap sound_samples_;
    SoundList      sound_list_;
    SoundListRemoval sound_to_be_cleared_;

    pSoundObject bgm_[2];
};

} //audio
} //psc

#endif
