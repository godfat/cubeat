
#include "private/AVIVideo.hpp"
#include "IrrDevice.hpp"

#include <iostream>

using namespace irr;
using namespace core;
using namespace video;

AVIVideo::AVIVideo(std::string const& path)
    :loop_(false), is_end_(false), paused_(false), updated_(false),
     file_ok_(false), frame_duration_(0), num_of_frames_(0), start_frame_(0),
     now_frame_(-1), width_(0), height_(0), last_tick_(0), texture_(0)
{
    timer_ = IrrDevice::i().d()->getTimer();
    if( !open(path) ) {
        std::cerr << "AVI file open failed.\n";
        is_end_ = true;
    }
}

AVIVideo::~AVIVideo()
{
    close();
}

void AVIVideo::redraw()
{
    if( !file_ok_ || paused_ ) return;

    is_end_ = nextFrame();
    if( is_end_ || !updated_ ) return;

    unsigned char* pixels_out = (unsigned char*)texture_->lock();
    if( !pixels_out ) { texture_->unlock(); return; }

#ifdef WIN32
    // 2011.04.18: code removed due to possibly unresolved copyright issue.
#endif //WIN32

    texture_->unlock();
}

void AVIVideo::setFPS(int fps)
{
    fps_  = fps;
    frame_duration_ = 1000.0f / fps;
}

void AVIVideo::setLoop(bool loop)
{
    loop_ = loop;
}

int AVIVideo::getCurrentFrame() const
{
    return now_frame_;
}

void AVIVideo::setCurrentFrame(int frame)
{
    if( frame < start_frame_ ) return;
    if( frame >= (start_frame_ + num_of_frames_) ) return;
    now_frame_ = frame;
}

void AVIVideo::stop()
{
    paused_ = true;
}

void AVIVideo::play()
{
    paused_ = false;
    last_tick_ = timer_->getTime();
}

void AVIVideo::restart()
{
    now_frame_ = start_frame_;
    updated_ = true;
    last_tick_ = timer_->getTime();
}

bool AVIVideo::isEnd() const
{
    return is_end_;
}

ITexture *AVIVideo::getTexture() const
{
    return texture_;
}

dimension2di AVIVideo::getSize() const
{
    return dimension2di(width_, height_);
}

//////////////////////// private below ///////////////////////////////

bool AVIVideo::open(std::string const& path)
{
#ifdef WIN32
    // 2011.04.18: code removed due to possibly unresolved copyright issue.
#endif //WIN32
    u32 w=1,h=1;
#ifdef WIN32
    // 2011.04.18: code removed due to possibly unresolved copyright issue.
#endif

    IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
    texture_ = driver->addTexture(dimension2d<u32>(w,h), "AVI_Texture", ECF_A8R8G8B8);
    if( !texture_ ) {
        printf("Error creating Irrlicht texture.\n");
        return false;
    }
    file_ok_ = true;
    is_end_ = nextFrame();
    return true;
}

void AVIVideo::initBitmapStruct(int bitsPerPixel)
{
#ifdef WIN32
    // 2011.04.18: code removed due to possibly unresolved copyright issue.
#endif //WIN32
}

bool AVIVideo::nextFrame()
{
    updated_ = false;
    if( last_tick_ + frame_duration_ <= timer_->getTime() ) {
        if( now_frame_ < start_frame_ ) restart();
        else {
            ++now_frame_;
            updated_ = true;
        }
        if( now_frame_ >= num_of_frames_ ) {
            if( loop_ ) restart();
            else return true;
        }
        last_tick_ += frame_duration_;
    }
    return false;
}

void AVIVideo::close()
{
#ifdef WIN32
    // 2011.04.18: code removed due to possibly unresolved copyright issue.
#endif //WIN32
}
