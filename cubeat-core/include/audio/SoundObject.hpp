#ifndef _SHOOTING_CUBES_AUDIO_SOUND_OBJECT_
#define _SHOOTING_CUBES_AUDIO_SOUND_OBJECT_

#include <AL/alure.h> //Do we need yet another interface abstraction to separate this?
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
                               int const& chunk_length = 8192)
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
    ~SoundObject();

    SoundObject& pause();
    SoundObject& stop();
    SoundObject& resume();

    bool finished() const;

private:
    SoundObject(wpSoundStream const&, bool const&);
    SoundObject(wpSoundBuffer const&, bool const&);
    void gen_source();
    ALuint source;
};

typedef SoundObject::pointer_type pSoundObject;
typedef SoundObject::wpointer_type wpSoundObject;

} // audio
} // psc

#endif //_SHOOTING_CUBES_AUDIO_SOUND_OBJECT_


