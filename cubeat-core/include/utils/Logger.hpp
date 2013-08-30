#ifndef _CUBEAT_UTILS_LOGGER_
#define _CUBEAT_UTILS_LOGGER_

#ifdef _SHOOTING_CUBES_ANDROID_
    #include <android/log.h> // for Android Logging defines

    #define LOG_TAG "CuBeat"
    #ifdef ANDROID
    # define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
    # define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
    # define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
    #else
    # define QUOTEME_(x) #x
    # define QUOTEME(x) QUOTEME_(x)
    # define LOGI(...) printf("I/" LOG_TAG " (" __FILE__ ":" QUOTEME(__LINE__) "): " __VA_ARGS__)
    # define LOGE(...) printf("E/" LOG_TAG "(" ")" __VA_ARGS__)
    #endif
#endif

//This is a draft of multi-threaded logger controlled by a static mutex.

#include <boost/thread/mutex.hpp>
#include <boost/thread/tss.hpp>
#include <iostream>

namespace psc {
namespace utils {

class Logger {
    typedef boost::thread_specific_ptr< std::ostringstream > pOSS;
    typedef boost::mutex::scoped_lock scoped_lock;

public:
    static Logger& i() {
        static Logger singleton;
        return singleton;
    }

    template<typename T>
    Logger& buf(T const& in) {
        if( oss_.get() == 0 )
            oss_.reset(new std::ostringstream);
        *oss_ << in;
        return *this;
    }

    template<typename T>
    Logger& buf(T const* in) {
        if( oss_.get() == 0 )
            oss_.reset(new std::ostringstream);
        *oss_ << in;
        return *this;
    }

    Logger& endl() {
        if( oss_.get() == 0 )
            oss_.reset(new std::ostringstream);
        scoped_lock l(io_mutex_);
        std::cout << oss_->str() << std::endl;
        oss_->str("");
        return *this;
    }

    Logger& out() {
        if( oss_.get() == 0 )
            oss_.reset(new std::ostringstream);
        scoped_lock l(io_mutex_);
        std::cout << oss_->str();
        oss_->str("");
        return *this;
    }

private:
    Logger(){}
    Logger(Logger const&);
    static boost::mutex io_mutex_;
    pOSS oss_;
};

} //utils
} //psc

#endif //_CUBEAT_UTILS_LOGGER_
