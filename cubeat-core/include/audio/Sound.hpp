
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

    Sound& loadStream(std::string const& path);
    Sound& loadBuffer(std::string const& path);
    Sound& loadSample(std::string const& path);

    //Sound& playStream(std::string const& path, bool const& loop = false);
    Sound& playBuffer(std::string const& path, bool const& loop = false);
    Sound& playSample(std::string const& path, time_t const& fade_t = 0, bool const& loop = false);

    //these are very important when you need to load 2 BGM at the same time and flip-flop between them
    Sound& loadBGM_AB(std::string const& path_a, std::string const& path_b);
    Sound& loadBGM(std::string const& path, bool const& loop = false);

    //just play this if you want to play a single music track
    Sound& playBGM_AB(std::string const& path_a, std::string const& path_b);
    Sound& playBGM(std::string const& path, bool const& loop = false);

    Sound& trackFlip(time_t const& fade_t = 0);

    //This may very well be extended to a multi-track system, abstracted from AL channels and sources that,
    //when you need arbitrary 3 or more BGM stand-by and somehow play them in-turn
    //However, what is the use case? I never bump into one.

    Sound& stopAll();
    Sound& pauseAll(bool const&);
    Sound& cycle();

    void init();
    ~Sound();

private:
    Sound();

    void exchange(pSoundObject const& before, pSoundObject const& after, time_t const& t);
    void internal_so_init(pSoundObject& s, std::string const& a, bool const& l, std::string const& b = std::string());

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
