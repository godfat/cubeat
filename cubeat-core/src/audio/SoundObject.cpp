
#include "audio/SoundObject.hpp"
#include "Conf.hpp"

#include <iostream>

using namespace psc;
using namespace audio;

int const SoundStream::NUM_BUFS = 2;

SoundStream::SoundStream(std::string const& path, int const& chunk_length)
    :name_(Conf::i().expand(path)), ALstream_(0)
{
    buffer_ = new ALuint[NUM_BUFS];
    ALstream_ = alureCreateStreamFromFile(name_.c_str(), chunk_length, NUM_BUFS, buffer_);
    if(!ALstream_) {
        std::cerr << "OpenAL: Could not load" << name_ << ": " << alureGetErrorString() << std::endl;
        //even if the playing stream is not found, it should be tolerable. (just skip it.)
    }
}

SoundStream::~SoundStream()
{
    std::cerr << "SoundStream " << name_ << " killed." << std::endl;
    if(ALstream_)
        alureDestroyStream(ALstream_, NUM_BUFS, buffer_);
    delete [] buffer_;
}

/// ----------- SoundBuffer below ---------- ///

SoundBuffer::SoundBuffer(std::string const& path)
    :name_(Conf::i().expand(path)), ALbuffer_(0)
{
    ALbuffer_ = alureCreateBufferFromFile(name_.c_str());
    if(!ALbuffer_) {
        std::cerr << "OpenAL: Could not load" << name_ << ": " << alureGetErrorString() << std::endl;
        //even if the buffer is cannot be created, it should be tolerable. (just skip it.)
    }
}

SoundBuffer::~SoundBuffer()
{
    std::cerr << "SoundBuffer " << name_ << " killed." << std::endl;
    if(ALbuffer_)
        alDeleteBuffers(1, &ALbuffer_);
}

/// ----------- SoundObject below ---------- ///

SoundObject::SoundObject(wpSoundStream const& stream, bool const& loop)
    //:stream_(stream)
{
    gen_source();
    if( pSoundStream s = stream.lock() ) {
        std::cerr << "OpenAL: Trying to play " << s->name_ << std::endl;
        alureRewindStream(s->ALstream_);
        if( !alurePlaySourceStream(source, s->ALstream_, SoundStream::NUM_BUFS, loop?-1:0, NULL, NULL) ) {
            std::cerr << "OpenAL: Failed to play stream " << s->name_ << ": " << alureGetErrorString() << std::endl;
            stop();
            //even if the stream cannot be played, it should be tolerable. (just skip it.)
        }
    }
}

SoundObject::SoundObject(wpSoundBuffer const& buffer, bool const& loop)
{
    gen_source();
    if( pSoundBuffer s = buffer.lock() ) {
        std::cerr << "OpenAL: Trying to play " << s->name_ << std::endl;
        alSourcei(source, AL_BUFFER, s->ALbuffer_);
        alSourcei(source, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
        if( !alurePlaySource(source, NULL, NULL) ) {
            std::cerr << "OpenAL: Failed to play buffer " << s->name_ << ": " << alureGetErrorString() << std::endl;
            stop();
            //even if the stream cannot be played, it should be tolerable. (just skip it.)
        }
    }
}

void SoundObject::gen_source()
{
    alGenSources(1, &source);
    if(alGetError() != AL_NO_ERROR) {
        std::cerr << "OpenAL: Failed to create source." << std::endl;
        //even if the source cannot be allocated by OpenAL, it should be tolerable. (just skip it.)
    }
}

//SoundObject& SoundObject::play(bool const& loop)
//{
//    if( pSoundStream s = stream_.lock() ) {
//        std::cerr << "OpenAL: Trying to play " << s->name_ << std::endl;
//        if( !alurePlaySourceStream(source, s->ALstream_, SoundStream::NUM_BUFS, loop?-1:0, NULL, NULL) ) {
//            std::cerr << "OpenAL: Failed to play stream " << s->name_ << ": " << alureGetErrorString() << std::endl;
//            stop();
//            //even if the stream cannot be played, it should be tolerable. (just skip it.)
//        }
//    }
//    return *this;
//}

SoundObject& SoundObject::pause()
{
    if( !alurePauseSource(source) ) {
        std::cerr << "OpenAL: Failed to pause source " << this << ": " << alureGetErrorString() << std::endl;
        stop();
        //even if the stream cannot be paused, it should be tolerable. (just skip it.)
    }
    return *this;
}

SoundObject& SoundObject::stop()
{
    if( !alureStopSource(source, AL_FALSE) ) {
        std::cerr << "OpenAL: Failed to stop source " << this << ": " << alureGetErrorString() << std::endl;
        alSourcei(source, AL_SOURCE_STATE, AL_STOPPED);
        //even if the stream cannot be stopped, it should be tolerable. (just skip it.)
    }
    return *this;
}

SoundObject& SoundObject::resume()
{
    if( !alureResumeSource(source) ) {
        std::cerr << "OpenAL: Failed to resume source " << this << ": " << alureGetErrorString() << std::endl;
        stop();
        //even if the stream cannot be resumed, it should be tolerable. (just skip it.)
    }
    return *this;
}

bool SoundObject::finished() const
{
    ALenum state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED || state == AL_INITIAL;
}

SoundObject::~SoundObject()
{
    alDeleteSources(1, &source);
}
