
SET (CMAKE_CXX_FLAGS "-std=c++98 -pedantic -Wall --expensive-optimizations -O3 -Os")
SET (SOURCES "test2.cpp")
SET (HEADERS )
SET(CMAKE_VERBOSE_MAKEFILE on)

PROJECT (shooting-cubes)

FIND_PACKAGE (boost REQUIRED)
# ADD_DEFINITIONS (${BOOST_DEFINITIONS})

# INCLUDE_DIRECTORIES (${BOOST_INCLUDE_DIR})
INCLUDE_DIRECTORIES ("/opt/local/include")

# TARGET_LINK_LIBRARIES(foo ${Boost_filesystem_LIBRARY} ${Boost_program_options_LIBRARY})

ADD_EXECUTABLE (bin/Hello ${SOURCES})

# ADD_LIBRARY (Hello2 STATIC ${SOURCES})
# ADD_LIBRARY (Hello3 SHARED ${SOURCES})
