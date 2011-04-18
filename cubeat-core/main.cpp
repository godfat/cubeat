
#include "App.hpp"
#include "Conf.hpp"
#include <cstdlib> // for srand
#include <ctime> // for time, clock

#ifdef __APPLE__
  #import <Cocoa/Cocoa.h>
  #include <CoreFoundation/CoreFoundation.h>
  #include <unistd.h>
  #include <iostream>
#endif //__APPLE__

int main()
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
#else
    psc::Conf::i().init(""); // doesn't need to find working_path on win32, not sure about linux though
#endif
    std::srand(std::time(0)^std::clock()); //  init srand for global rand...
    return psc::App::i().launchOpening().run();
}
