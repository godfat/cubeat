
#include "App.hpp"
#include "Conf.hpp"
#include "utils/Logger.hpp"
#include <cstdlib>  // for srand
#include <ctime>    // for time, clock

#if defined(_SHOOTING_CUBES_ANDROID_)
    #include "IrrDevice.hpp"
    #include "android/unpack_data.h"
	#include <android_native_app_glue.h>
#elif defined(_SHOOTING_CUBES_IPHONE_)
	#import <UIKit/UIKit.h>
	#import <Foundation/Foundation.h>
#endif

#ifdef __APPLE__
  #import <Cocoa/Cocoa.h>
  #include <CoreFoundation/CoreFoundation.h>
  #include <unistd.h>
  #include <iostream>
#endif //__APPLE__

#ifdef _SHOOTING_CUBES_ANDROID_
void android_main(android_app* app)
#else
int main(int argc, char *argv[])
#endif
{
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
        std::cerr << "PATH ERROR " << __FILE__ << ": " << __LINE__ << std::endl;
    }
    CFRelease(resourcesURL);
    chdir(path); // cd in
    psc::Conf::i().init(path);
#elif _SHOOTING_CUBES_ANDROID_
    app_dummy();
    char buf[256] = {0};
    getcwd(buf, 256);
    chdir(app->activity->internalDataPath);
    LOGI("cwd: %s", buf);
    unpack_data(app);
    psc::Conf::i().init("", app->activity->internalDataPath);
    psc::App::i().init(app).launchOpening2().run();
    IrrDevice::i().~IrrDevice(); // manually destruct here, as IrrEx17 example?
#else
    psc::Conf::i().init(""); // doesn't need to find working_path on win32, not sure about linux though
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    //return psc::App::i().init().launchOpening().run();
    return psc::App::i().init().launchOpening2().run();
#endif
}
