#ifndef _SHOOTING_CUBES_AUDIO_SOUND_OBJECT_
#define _SHOOTING_CUBES_AUDIO_SOUND_OBJECT_

#if !defined(ALC_VERSION_0_1) || !defined(AL_VERSION_1_0)
#if defined(__APPLE__)
#include <OpenAL/alure.h>
#else
#include <AL/alure.h>
#endif
#endif

#include <ALmixer.h>

#include <string>
#include <boost/tr1/memory.hpp>

namespace psc {
namespace audio {

class SoundObject;

class SoundStream
{
public:
    typedef std::tr1::shared_ptr<SoundStream> pointer_type;
    typedef std::tr1::weak_ptr<SoundStream> wpointer_type;

    static pointer_type create(std::string const& path,
                               int const& chunk_length = 250000)
    {
        return pointer_type(new SoundStream(path, chunk_length));
    }
    ~SoundStream();

private:
    SoundStream(std::string const&, int const&);

    static int const NUM_BUFS;
    std::string name_;
    alureStream* ALstream_;
    ALuint* buffer_;

    friend class SoundObject;
};

typedef SoundStream::pointer_type pSoundStream;
typedef SoundStream::wpointer_type wpSoundStream;

class SoundBuffer
{
public:
    typedef std::tr1::shared_ptr<SoundBuffer> pointer_type;
    typedef std::tr1::weak_ptr<SoundBuffer> wpointer_type;

    static pointer_type create(std::string const& path)
    {
        return pointer_type(new SoundBuffer(path));
    }
    ~SoundBuffer();

private:
    SoundBuffer(std::string const&);

    std::string name_;
    ALuint ALbuffer_;

    friend class SoundObject;
};

typedef SoundBuffer::pointer_type pSoundBuffer;
typedef SoundBuffer::wpointer_type wpSoundBuffer;

class SoundSample
{
public:
    typedef std::tr1::shared_ptr<SoundSample> pointer_type;
    typedef std::tr1::weak_ptr<SoundSample> wpointer_type;

    static pointer_type create(std::string const& path, bool const& streamed = false)
    {
        return pointer_type(new SoundSample(path, streamed));
    }
    ~SoundSample();

private:
    SoundSample(std::string const&, bool const&);

    void init(bool const&);

    std::string name_;
    ALmixer_Data* data_;

    friend class SoundObject;
};

typedef SoundSample::pointer_type pSoundSample;
typedef SoundSample::wpointer_type wpSoundSample;

class SoundObject
{
public:
    typedef std::tr1::shared_ptr<SoundObject> pointer_type;
    typedef std::tr1::weak_ptr<SoundObject> wpointer_type;

    static pointer_type create(wpSoundStream const& stream, bool const& loop = false) {
        return pointer_type(new SoundObject(stream, loop));
    }
    static pointer_type create(wpSoundBuffer const& buffer, bool const& loop = false) {
        return pointer_type(new SoundObject(buffer, loop));
    }
    static pointer_type create(wpSoundSample const& sample) {
        return pointer_type(new SoundObject(sample));
    }
    ~SoundObject();

    SoundObject& play(time_t const& fade_t = 0, int const& loop = 0);
    SoundObject& rewind();
    SoundObject& pause();
    SoundObject& stop();
    SoundObject& resume();
    SoundObject& fade_volume(double const&, time_t const&);
    SoundObject& volume(double const&);

    bool is_active() const;
    bool is_playing() const;
    bool is_paused() const;
    double volume() const;

    void partB(wpSoundSample const& partB);
    void cycle();

private:
    SoundObject(wpSoundStream const&, bool const&);
    SoundObject(wpSoundBuffer const&, bool const&);
    SoundObject(wpSoundSample const&);
    void gen_source();  //probably not going to be used after ALmixer used.
    ALuint source;      //probably not going to be used after ALmixer used.

    //for ALmixer
    ALuint src_;
    ALint  ch_;

    wpSoundSample sampleA_;
    wpSoundSample sampleB_;
};

typedef SoundObject::pointer_type pSoundObject;
typedef SoundObject::wpointer_type wpSoundObject;

} // audio
} // psc

#endif //_SHOOTING_CUBES_AUDIO_SOUND_OBJECT_


