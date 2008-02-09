

#include "private/AVIVideo.hpp"
#include "IrrDevice.hpp"

#include <iostream>

#ifdef WIN32

using namespace irr;
using namespace core;
using namespace video;

AVIVideo::AVIVideo(std::string const& path)
    :loop_(false), is_end_(false), paused_(false), updated_(false),
     file_ok_(false), frame_duration_(0), num_of_frames_(0), start_frame_(0),
     now_frame_(-1), width_(0), height_(0), last_tick_(0), texture_(0)
{
    timer_ = IrrDevice::i().d()->getTimer();
    if( !open(path) )
        std::cerr << "AVI file open failed.\n";
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

    unsigned char* pixels_in = (unsigned char*)
                               AVIStreamGetFrame( frame_obj_, now_frame_ ) +
                               sizeof(BITMAPINFOHEADER);

    int pitch_i = bitmap_.biWidth * 3;
    int pitch_o = texture_->getPitch();
    int yi      = (bitmap_.biHeight-1) * pitch_i; //bitmap format is upside-down.
    int yo      = 0;

    for(int h = 0; h < bitmap_.biHeight; ++h) {
        for(int w = 0, xi=0, xo=0; w < bitmap_.biWidth; ++w) {
            //this is rather dangerous, cuz we assume the irr texture is A8R8G8B8
            //and the AVI bitmap is set to 24bit color.
            pixels_out[yo + xo    ] = pixels_in[yi + xi    ];   //R
            pixels_out[yo + xo + 1] = pixels_in[yi + xi + 1];   //G
            pixels_out[yo + xo + 2] = pixels_in[yi + xi + 2];   //B
            pixels_out[yo + xo + 3] = 255;                      //A
            xi += 3;
            xo += 4;
        }
        yi -= pitch_i;
        yo += pitch_o;
    }

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
    AVIFILEINFO      avi_info;
    HRESULT          hr;
//    PAVISTREAM       astream;             //not used for non-sound streaming
//    AVISTREAMINFO*   stream_info;
//    LPWAVEFORMAT     audio_format;
//    LPPCMWAVEFORMAT  audio_format_PCM;

    AVIFileInit();
    hr = AVIFileOpen(&avi_file_, path.c_str(), OF_READ, NULL);
    if(hr != AVIERR_OK) return false;

    AVIFileInfo(avi_file_, &avi_info, sizeof(AVIFILEINFO));
    setFPS((int)((float)avi_info.dwRate / avi_info.dwScale + 0.5f));

    width_ = avi_info.dwWidth;
    height_= avi_info.dwHeight;

    hr = AVIFileGetStream(avi_file_, &vstream_, streamtypeVIDEO, 0);
    if(hr != AVIERR_OK) return false;

    start_frame_ = AVIStreamStart(vstream_);
    if(start_frame_ == -1) return false;

    num_of_frames_ = AVIStreamLength(vstream_);
    if(num_of_frames_ == -1) return false;

    initBitmapStruct(24);
    frame_obj_ = AVIStreamGetFrameOpen(vstream_, &bitmap_);
    if(bitmap_.biSizeImage == 0)
        bitmap_.biSizeImage = bitmap_.biHeight * bitmap_.biWidth * 3;

    u32 w,h;
    for(w = 1; w < avi_info.dwWidth; w <<= 1);
    for(h = 1; h < avi_info.dwHeight; h <<= 1);

    IVideoDriver* driver = IrrDevice::i().d()->getVideoDriver();
    texture_ = driver->addTexture(dimension2di(w,h), "AVI_Texture", ECF_A8R8G8B8);
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
    bitmap_.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_.biBitCount = bitsPerPixel;
    bitmap_.biClrImportant = 0;
    bitmap_.biClrUsed = 0;
    bitmap_.biCompression = BI_RGB;
    bitmap_.biPlanes = 1;
    bitmap_.biWidth = width_;
    bitmap_.biHeight = height_;
    bitmap_.biXPelsPerMeter = 0;
    bitmap_.biYPelsPerMeter = 0;
    bitmap_.biSizeImage = (((bitmap_.biWidth * 3) + 3) & 0xfffc) * bitmap_.biHeight;
    if (bitmap_.biBitCount > 24)      bitmap_.biBitCount = 32;
    else if (bitmap_.biBitCount > 16) bitmap_.biBitCount = 24;
    else if (bitmap_.biBitCount > 8)  bitmap_.biBitCount = 16;
    else if (bitmap_.biBitCount > 4)  bitmap_.biBitCount =  8;
    else if (bitmap_.biBitCount > 0)  bitmap_.biBitCount =  4;
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
    if( file_ok_ && frame_obj_ ) AVIStreamGetFrameClose( frame_obj_ );
    if( file_ok_ && vstream_ )   AVIStreamRelease( vstream_ );
    if( avi_file_ )              AVIFileRelease( avi_file_ );
    AVIFileExit();
}

#endif //WIN32
