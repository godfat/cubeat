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
    static pointer_type create(wpSoundSample const& sample, unsigned int const& fade = 0, bool const& loop = false) {
        return pointer_type(new SoundObject(sample, fade, loop));
    }
    ~SoundObject();

    SoundObject& pause();
    SoundObject& stop();
    SoundObject& resume();
    bool has_partB() const { return has_partB_; }
    SoundObject& partB_path(std::string const& path);
    std::string  partB_path() const { return partB_path_; }

    bool finished() const;

private:
    SoundObject(wpSoundStream const&, bool const&);
    SoundObject(wpSoundBuffer const&, bool const&);
    SoundObject(wpSoundSample const&, unsigned int const&, bool const&);
    void gen_source();  //probably not going to be used.
    ALuint source;      //probably not going to be used.

    //for ALmixer
    ALuint src_; //do we need this?
    ALint  ch_;  //do we need this?

    bool        has_partB_;
    std::string partB_path_;
};

typedef SoundObject::pointer_type pSoundObject;
typedef SoundObject::wpointer_type wpSoundObject;

} // audio
} // psc

#endif //_SHOOTING_CUBES_AUDIO_SOUND_OBJECT_


