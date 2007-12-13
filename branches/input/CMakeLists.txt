
PROJECT (shooting-cubes)

SET (SOURCES main.cpp
             src/Button.cpp
             src/Crosshair.cpp
             src/EventDispatcher.cpp
             src/Input.cpp
             src/IrrDevice.cpp
             src/private/MastEventReceiver.cpp)

SET (CMAKE_VERBOSE_MAKEFILE on)

FIND_PACKAGE (boost REQUIRED)

INCLUDE_DIRECTORIES (/opt/local/include
                     /home/godfat/projects/irr/irrlicht-1.4/include
                     ./include)

LINK_DIRECTORIES (/home/godfat/projects/irr/irrlicht-1.4/lib/Linux
                  /usr/X11R6/lib)

LINK_LIBRARIES (Irrlicht GL X11 Xext Xxf86vm freetype)

ADD_EXECUTABLE (bin/Hello ${SOURCES})

SET (CMAKE_CXX_FLAGS "-std=c++98 -pedantic -Wall --expensive-optimizations -O3 -Os --input-charset=utf-8 --fast-math")

SET (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -framework AppKit -framework Foundation")
