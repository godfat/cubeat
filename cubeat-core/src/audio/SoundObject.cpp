
#include "audio/SoundObject.hpp"
#include "Conf.hpp"

#include <iostream>

using namespace psc;
using namespace audio;

int const SoundStream::NUM_BUFS = 3;

SoundStream::SoundStream(std::string const& path, int const& chunk_length)
    :name_(Conf::i().expand(path))
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    ,ALstream_(0)
    #endif
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    buffer_ = new ALuint[NUM_BUFS];
    ALstream_ = alureCreateStreamFromFile(name_.c_str(), chunk_length, NUM_BUFS, buffer_);
    if(!ALstream_) {
        std::cerr << "OpenAL: Could not load" << name_ << ": " << alureGetErrorString() << std::endl;
        //even if the playing stream is not found, it should be tolerable. (just skip it.)
    }
    #endif
}

SoundStream::~SoundStream()
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    std::cout << "SoundStream " << name_ << " killed." << std::endl;
    if(ALstream_)
        alureDestroyStream(ALstream_, NUM_BUFS, buffer_);
    delete [] buffer_;
    #endif
}

/// ----------- SoundBuffer below ---------- ///

SoundBuffer::SoundBuffer(std::string const& path)
    :name_(Conf::i().expand(path))
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    ,ALbuffer_(0)
    #endif
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    ALbuffer_ = alureCreateBufferFromFile(name_.c_str());
    if(!ALbuffer_) {
        std::cerr << "OpenAL: Could not load" << name_ << ": " << alureGetErrorString() << std::endl;
        //even if the buffer is cannot be created, it should be tolerable. (just skip it.)
    }
    #endif
}

SoundBuffer::~SoundBuffer()
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    std::cout << "SoundBuffer " << name_ << " killed." << std::endl;
    if(ALbuffer_)
        alDeleteBuffers(1, &ALbuffer_);
    #endif
}

/// ----------- SoundSample below ---------- ///

SoundSample::SoundSample(std::string const& path, bool const& streamed)
    :name_(Conf::i().expand(path))
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    ,data_(0)
    #endif
{
    init(streamed);
}

void SoundSample::init(bool const& streamed)
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    if ( streamed ) {
        data_ = ALmixer_LoadStream( name_.c_str(),
                                    16384,      //ALMIXER_DEFAULT_BUFFERSIZE = 8192
                                    4,          //ALMIXER_DEFAULT_QUEUE_BUFFERS = 12 <--- why soo many???
                                    2,          //ALMIXER_DEFAULT_STARTUP_BUFFERS = 4 <--- what does it do?
                                    2,          //ALMIXER_DEFAULT_BUFFERS_TO_QUEUE_PER_UPDATE_PASS = 2 <-- huh?
                                    AL_FALSE );
    }
    else {
        data_ = ALmixer_LoadAll( name_.c_str(), AL_FALSE );
    }
    if( !data_ ) {
        std::cerr << "OpenAL (ALmixer): Could not load " << name_ << ": " << ALmixer_GetError() << std::endl;
        //even if the buffer is cannot be created, it should be tolerable. (just skip it.)
    }
    #endif
}

SoundSample::~SoundSample()
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    std::cout << "SoundSample " << name_ << " killed." << std::endl;
    if( data_ )
        ALmixer_FreeData( data_ );
    #endif
}

/// ----------- SoundObject below ---------- ///

SoundObject::SoundObject(wpSoundStream const& stream, bool const& loop)
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
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
    #endif
}

SoundObject::SoundObject(wpSoundBuffer const& buffer, bool const& loop)
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
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
    #endif
}

SoundObject::SoundObject(wpSoundSample const& sample)
    :
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    src_(0), ch_(-1),
    #endif
    sampleA_(sample), sampleB_(pSoundSample()), loaded_into_channel_(false)
{   //un-init ALsource is 0, but un-init Channel is -1
}

SoundObject& SoundObject::play(time_t const& fade_t, int const& loop)
{
    if( pSoundSample s = sampleA_.lock() ) {
        #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
        rewind(); // rewind here to ensure before this playback,
                  // the actual audio stream is not half way through.
        if( fade_t > 0 ) { ch_ = ALmixer_FadeInChannel(-1, s->data_, loop, fade_t); }
        else             { ch_ = ALmixer_PlayChannel(-1, s->data_, loop); }

        src_ = ALmixer_GetSource(ch_); // just for future reference. will be useful to determine if a source stopped.

        if( ch_ == -1 ) {
            std::cerr << "OpenAL (ALmixer): Failed to play sample " << s->name_ << ": " << ALmixer_GetError() << std::endl;
            //even if the stream cannot be played, it should be tolerable. (just skip it.)
            return *this;
        }
        #endif
        loaded_into_channel_ = true;
    }
    return *this;
}

void SoundObject::gen_source()
{   //this is internal and won't be called when using ALmixer
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    alGenSources(1, &source);
    if(alGetError() != AL_NO_ERROR) {
        std::cerr << "OpenAL: Failed to create source." << std::endl;
        //even if the source cannot be allocated by OpenAL, it should be tolerable. (just skip it.)
    }
    #endif
}

SoundObject& SoundObject::pause()
{
    if( !is_playing() ) return *this;
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
//    if( !alurePauseSource(source) ) {
    if( ALmixer_PauseChannel(ch_) == -1 ) { // PauseChannel is -1 on error
//        std::cerr << "OpenAL: Failed to pause source " << this << ": " << alureGetErrorString() << std::endl;
        std::cerr << "OpenAL (ALmixer): Failed to pause sound " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
        stop();
        //even if the stream cannot be paused, it should be tolerable. (just skip it.)
    }
    #endif
    std::cout << this << " is paused." << std::endl;
    return *this;
}

SoundObject& SoundObject::stop()
{
    if( !is_active() ) return *this;
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
//    if( !alureStopSource(source, AL_FALSE) ) {
    if( ALmixer_HaltChannel(ch_) == -1 ) { // HaltChannel is -1 on error
//        std::cerr << "OpenAL: Failed to stop source " << this << ": " << alureGetErrorString() << std::endl;
        std::cerr << "OpenAL (ALmixer): Failed to stop sound " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
//        alSourcei(source, AL_SOURCE_STATE, AL_STOPPED);
        //even if the stream cannot be stopped, it should be tolerable. (just skip it.)
    }
    ch_ = -1; //otherwise we have no idea if this is stopped.
    #endif
    return *this;
}

SoundObject& SoundObject::rewind()
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    if( pSoundSample s2 = sampleB_.lock() ) { // AB-loop tracks are special cases
        pSoundSample s1 = sampleA_.lock();
        if( ALmixer_SeekData( s1->data_, 0 ) == -1 ) {
            std::cerr << "OpenAL (ALmixer): Failed to rewind data " << s1->name_ << ": " << ALmixer_GetError() << std::endl;
            return *this;
        }
        if( ALmixer_SeekData( s2->data_, 0 ) == -1 ) {
            std::cerr << "OpenAL (ALmixer): Failed to rewind data " << s2->name_ << ": " << ALmixer_GetError() << std::endl;
            return *this;
        }
    }
    else {
        if( ch_ != -1 ) { // You will never use this API to rewind ALL CHANNELS. -1 is for un-initialized object.
            if( ALmixer_RewindChannel(ch_) == -1 ) {
                std::cerr << "OpenAL (ALmixer): Failed to rewind sound/stream " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
                stop();
            }
        }
    }
    #endif
    return *this;
}

SoundObject& SoundObject::seek(time_t const& ms)
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    if( pSoundSample s2 = sampleB_.lock() ) { // AB-loop tracks are special cases
        pSoundSample s1 = sampleA_.lock();
        time_t t1 = ALmixer_GetTotalTime(s1->data_);
        if( ms < t1 ) {
            ALmixer_HaltChannel(ch_);
            ALmixer_PlayChannel(ch_, s1->data_, 0);
            ALmixer_SeekChannel(ch_, ms);
        } else {
            int ms_into_part_B = ms - t1;
            std::cout << "Seeking to " << ms << " and it's " << ms_into_part_B << " into part B of this AB loop track. " << "(" << s2->name_ <<  ")" << std::endl;
            ALmixer_HaltChannel(ch_);
            ALmixer_PlayChannel(ch_, s2->data_, -1);
            ALmixer_SeekChannel(ch_, ms_into_part_B);
        }
        src_ = ALmixer_GetSource(ch_); // for future reference.
    }
    else {
        if( ALmixer_SeekChannel(ch_, ms) == -1 ) {
            std::cerr << "OpenAL (ALmixer): Failed to seek sound/stream " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
            stop();
        }
    }
    #endif
    return *this;
}

SoundObject& SoundObject::resume()
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
//    if( !alureResumeSource(source) ) {
    if( ALmixer_ResumeChannel(ch_) == -1 ) { // ResumeChannel is -1 on error
        //std::cerr << "OpenAL: Failed to resume source " << this << ": " << alureGetErrorString() << std::endl;
        std::cerr << "OpenAL (ALmixer): Failed to resume sound " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
        stop();
        //even if the stream cannot be resumed, it should be tolerable. (just skip it.)
    }
    #endif
    return *this;
}

void SoundObject::partB(wpSoundSample const& partB)
{
    sampleB_ = partB;
}

bool SoundObject::is_loaded() const
{
    return loaded_into_channel_;
}

bool SoundObject::is_active() const
{
//    ALenum state;
//    alGetSourcei(source, AL_SOURCE_STATE, &state);
//    return state == AL_STOPPED; //this was finished(), beware
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    if( ch_ == -1 ) return false;
    return static_cast<bool>(ALmixer_IsActiveChannel(ch_));
    #else
    return false;
    #endif
}

bool SoundObject::is_paused() const
{
    if( !is_active() ) return false;
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    return static_cast<bool>(ALmixer_IsPausedChannel(ch_));
    #else
    return false;
    #endif
}

bool SoundObject::is_playing() const
{
    if( !is_active() ) return false;
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    return static_cast<bool>(ALmixer_IsPlayingChannel(ch_));
    #else
    return false;
    #endif
}

double SoundObject::volume() const
{
    if( !is_active() ) return 0;
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    return ALmixer_GetVolumeChannel(ch_);
    #else
    return 0;
    #endif
}

SoundObject& SoundObject::volume(double const& v)
{
    if( !is_active() ) return *this;
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    if( ALmixer_SetVolumeChannel(ch_, v) == -1 ) { // SetVolumeChannel is -1 on error
        std::cerr << "OpenAL (ALmixer): Failed to adjust volume of " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
        stop();
        //even if the stream cannot be resumed, it should be tolerable. (just skip it.)
    }
    #endif
    return *this;
}

SoundObject& SoundObject::fade_volume(double const& v, time_t const& t)
{
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    std::cout << "Sound: going to fade " << sampleA_.lock()->name_ << " from " << volume() << " to " << v << ", time = " << t << std::endl;
    if( ALmixer_FadeChannel(ch_, t, v) == -1 ) { // FadeChannel is -1 on error
        std::cerr << "OpenAL (ALmixer): Failed to fade volume of " << this << " on channel " << ch_ << ": " << ALmixer_GetError() << std::endl;
        stop();
        //even if the stream cannot be resumed, it should be tolerable. (just skip it.)
    }
    #endif
    return *this;
}

void SoundObject::cycle()
{
    if( !loaded_into_channel_ ) return;
    // is_active() will be called twice. if it is finished the first time in the cycle(), then
    // it should check again for partB, if still not, then the second call to finished in Sound::cycle()
    // will find out it is really finished, otherwise it will init a new sample here and hence not finished.
    #ifdef _SHOOTING_CUBES_ENABLE_SOUND_
    if( !is_active() ) {
        if( pSoundSample s = sampleB_.lock() ) {
            ALmixer_PlayChannel(ch_, s->data_, -1); //loop it on top of the old channel as part A.
            src_ = ALmixer_GetSource(ch_); // for future reference.
        }
    }
    #endif
}

SoundObject::~SoundObject()
{
    //alDeleteSources(1, &source);
}
