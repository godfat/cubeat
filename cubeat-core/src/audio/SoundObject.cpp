
#include "audio/SoundObject.hpp"
#include "Conf.hpp"

#include <iostream>

using namespace psc;
using namespace audio;

int const SoundStream::NUM_BUFS = 3;

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
    std::cout << "SoundStream " << name_ << " killed." << std::endl;
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
    std::cout << "SoundBuffer " << name_ << " killed." << std::endl;
    if(ALbuffer_)
        alDeleteBuffers(1, &ALbuffer_);
}

/// ----------- SoundSample below ---------- ///

SoundSample::SoundSample(std::string const& path, bool const& streamed)
    :name_(Conf::i().expand(path)), data_(0)
{
    if ( streamed ) {
        data_ = ALmixer_LoadStream( name_.c_str(),
                                    ALMIXER_DEFAULT_BUFFERSIZE,
                                    ALMIXER_DEFAULT_QUEUE_BUFFERS,
                                    ALMIXER_DEFAULT_STARTUP_BUFFERS,
                                    ALMIXER_DEFAULT_BUFFERS_TO_QUEUE_PER_UPDATE_PASS,
                                    AL_FALSE );
    }
    else {
        data_ = ALmixer_LoadAll( name_.c_str(), AL_FALSE );
    }
    if( !data_ ) {
        std::cerr << "OpenAL (ALmixer): Could not load " << name_ << ": " << ALmixer_GetError() << std::endl;
        //even if the buffer is cannot be created, it should be tolerable. (just skip it.)
    }
}

SoundSample::~SoundSample()
{
    std::cout << "SoundSample " << name_ << " killed." << std::endl;
    if( data_ )
        ALmixer_FreeData( data_ );
}

/// ----------- SoundObject below ---------- ///

SoundObject::SoundObject(wpSoundStream const& stream, bool const& loop) : has_partB_(false)
{
    gen_source();
    if( pSoundStream s = stream.lock() ) {
        //std::cerr << "OpenAL: Trying to play " << s->name_ << std::endl;
        alureRewindStream(s->ALstream_);
        if( !alurePlaySourceStream(source, s->ALstream_, SoundStream::NUM_BUFS, loop?-1:0, NULL, NULL) ) {
            std::cerr << "OpenAL: Failed to play stream " << s->name_ << ": " << alureGetErrorString() << std::endl;
            stop();
            //even if the stream cannot be played, it should be tolerable. (just skip it.)
        }
    }
}

SoundObject::SoundObject(wpSoundBuffer const& buffer, bool const& loop) : has_partB_(false)
{
    gen_source();
    if( pSoundBuffer s = buffer.lock() ) {
        //std::cerr << "OpenAL: Trying to play " << s->name_ << std::endl;
        alSourcei(source, AL_BUFFER, s->ALbuffer_);
        alSourcei(source, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
        if( !alurePlaySource(source, NULL, NULL) ) {
            std::cerr << "OpenAL: Failed to play buffer " << s->name_ << ": " << alureGetErrorString() << std::endl;
            stop();
            //even if the stream cannot be played, it should be tolerable. (just skip it.)
        }
    }
}

SoundObject::SoundObject(wpSoundSample const& sample, unsigned int const& fade, bool const& loop)
    :src_(0), ch_(0), has_partB_(false)
{
    if( pSoundSample s = sample.lock() ) {
        if( fade > 0 ) { ch_ = ALmixer_FadeInChannel(-1, s->data_, loop?-1:0, fade); }
        else           { ch_ = ALmixer_PlayChannel(-1, s->data_, loop?-1:0); }

        src_ = ALmixer_GetSource(ch_); // just for future reference. will be useful to determine if a source stopped.

        if( ch_ == -1 ) {
            std::cerr << "OpenAL (ALmixer): Failed to play sample " << s->name_ << ": " << ALmixer_GetError() << std::endl;
            stop();
            //even if the stream cannot be played, it should be tolerable. (just skip it.)
        }
    }
}

void SoundObject::gen_source()
{   //this is internal and won't be called when using ALmixer
    alGenSources(1, &source);
    if(alGetError() != AL_NO_ERROR) {
        std::cerr << "OpenAL: Failed to create source." << std::endl;
        //even if the source cannot be allocated by OpenAL, it should be tolerable. (just skip it.)
    }
}

SoundObject& SoundObject::pause()
{
//    if( !alurePauseSource(source) ) {
    if( ALmixer_PauseChannel(ch_) == -1 ) { // PauseChannel is -1 on error
//        std::cerr << "OpenAL: Failed to pause source " << this << ": " << alureGetErrorString() << std::endl;
        std::cerr << "OpenAL (ALmixer): Failed to pause sound " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
        stop();
        //even if the stream cannot be paused, it should be tolerable. (just skip it.)
    }
    return *this;
}

SoundObject& SoundObject::stop()
{
//    if( !alureStopSource(source, AL_FALSE) ) {
    if( ALmixer_HaltChannel(ch_) == -1 ) { // HaltChannel is -1 on error
//        std::cerr << "OpenAL: Failed to stop source " << this << ": " << alureGetErrorString() << std::endl;
        std::cerr << "OpenAL (ALmixer): Failed to stop sound " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
//        alSourcei(source, AL_SOURCE_STATE, AL_STOPPED);
        //even if the stream cannot be stopped, it should be tolerable. (just skip it.)
    }
    ch_ = -1; //otherwise we have no idea if this is stopped.
    return *this;
}

SoundObject& SoundObject::resume()
{
//    if( !alureResumeSource(source) ) {
    if( ALmixer_ResumeChannel(ch_) == -1 ) { // ResumeChannel is -1 on error
        //std::cerr << "OpenAL: Failed to resume source " << this << ": " << alureGetErrorString() << std::endl;
        std::cerr << "OpenAL (ALmixer): Failed to resume sound " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
        stop();
        //even if the stream cannot be resumed, it should be tolerable. (just skip it.)
    }
    return *this;
}

SoundObject& SoundObject::partB_path(std::string const& path)
{
    partB_path_ = path;
    has_partB_ = true;
    return *this;
}

bool SoundObject::finished() const
{
//    ALenum state;
//    alGetSourcei(source, AL_SOURCE_STATE, &state);
//    return state == AL_STOPPED;
    bool active = ALmixer_IsActiveChannel(ch_);
    return !active;
}

SoundObject::~SoundObject()
{
    //alDeleteSources(1, &source);
}
