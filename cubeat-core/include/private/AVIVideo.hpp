
#ifndef _SHOOTING_CUBES_PRIVATE_AVI_VIDEO_WIN32_ONLY_
#define _SHOOTING_CUBES_PRIVATE_AVI_VIDEO_WIN32_ONLY_

#ifdef WIN32
#include <windows.h>
#include <vfw.h>
#endif

#include <string>

namespace irr {
namespace core {
template<class T>
class dimension2d;
typedef dimension2d<int> dimension2di;
}
namespace video {
class ITexture;
}
class ITimer;
}

class AVIVideo
{
public:
    AVIVideo(std::string const& path);
    ~AVIVideo();

    void redraw();
    void setFPS(int fps);
    void setLoop(bool loop);
    void setCurrentFrame(int frame);
    void stop();
    void play();
    void restart();
    bool isEnd() const;
    int  getCurrentFrame() const;
    irr::video::ITexture*   getTexture() const;
    irr::core::dimension2di getSize() const;

private:
    bool open(std::string const& path);
    void close();
    bool nextFrame();
    void initBitmapStruct(int bitsPerPixel);

private:
    bool loop_;
    bool is_end_;
    bool paused_;
    bool updated_;
    bool file_ok_;

    float        frame_duration_;
    int          num_of_frames_;
    int          start_frame_;
    int          now_frame_;
    int          width_, height_;
    unsigned int fps_, last_tick_;

    irr::video::ITexture* texture_;
    irr::ITimer* timer_;

#ifdef WIN32
    // 2011.04.18: code removed due to possibly unresolved copyright issue.
#endif // WIN32
};


#endif // _SHOOTING_CUBES_PRIVATE_AVI_VIDEO_WIN32_ONLY_
