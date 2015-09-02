
#include "App.hpp"
#include "Conf.hpp"
#include "utils/Logger.hpp"
#include <cstdlib>  // for srand
#include <ctime>    // for time, clock

#ifdef __APPLE__
    #import <Cocoa/Cocoa.h>
    #include <CoreFoundation/CoreFoundation.h>
    #include <unistd.h>
    #include <iostream>
#elif defined(_SHOOTING_CUBES_ANDROID_)
    #include "IrrDevice.hpp"
    #include "android/unpack_data.h"
	#include <android_native_app_glue.h>
#elif defined(_SHOOTING_CUBES_IPHONE_)
	#import <UIKit/UIKit.h>
	#import <Foundation/Foundation.h>
#else
    #include <windows.h>
#endif



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
    psc::Conf::i().init("", app->activity->internalDataPath, app->activity->externalDataPath);
    psc::App::i().init(app).launchOpening2().run();
    IrrDevice::i().~IrrDevice(); // manually destruct here, as IrrEx17 example?
#else

// Sticked setProcessDpiAware() function from SFML library (zlib license) to here
// SFML - Simple and Fast Multimedia Library
//   Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
// Full copyright notice please see under doc/ folder

    HINSTANCE shCoreDll = LoadLibrary("Shcore.dll");

    if (shCoreDll)
    {
        enum ProcessDpiAwareness
        {
            ProcessDpiUnaware         = 0,
            ProcessSystemDpiAware     = 1,
            ProcessPerMonitorDpiAware = 2
        };

        typedef HRESULT (WINAPI* SetProcessDpiAwarenessFuncType)(ProcessDpiAwareness);
        SetProcessDpiAwarenessFuncType SetProcessDpiAwarenessFunc = reinterpret_cast<SetProcessDpiAwarenessFuncType>(GetProcAddress(shCoreDll, "SetProcessDpiAwareness"));

        if (SetProcessDpiAwarenessFunc)
        {
            // We only check for E_INVALIDARG because we would get
            // E_ACCESSDENIED if the DPI was already set previously
            // and S_OK means the call was successful
            if (SetProcessDpiAwarenessFunc(ProcessSystemDpiAware) == E_INVALIDARG)
            {
                printf("Failed to set process DPI awareness\n");
            }
            else
            {
                FreeLibrary(shCoreDll);
            }
        }

        FreeLibrary(shCoreDll);
    }

    // Fall back to SetProcessDPIAware if SetProcessDpiAwareness
    // is not available on this system
    HINSTANCE user32Dll = LoadLibrary("user32.dll");

    if (user32Dll)
    {
        typedef BOOL (WINAPI* SetProcessDPIAwareFuncType)(void);
        SetProcessDPIAwareFuncType SetProcessDPIAwareFunc = reinterpret_cast<SetProcessDPIAwareFuncType>(GetProcAddress(user32Dll, "SetProcessDPIAware"));

        if (SetProcessDPIAwareFunc)
        {
            if (!SetProcessDPIAwareFunc())
                printf("Failed to set process DPI awareness\n");
        }

        FreeLibrary(user32Dll);
    }

    psc::Conf::i().init(""); // doesn't need to find working_path on win32, not sure about linux though
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    //return psc::App::i().init().launchOpening().run();
    return psc::App::i().init().launchOpening2().run();
#endif
}
