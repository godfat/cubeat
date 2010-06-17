
== CuBeat
 all library remains original license, and
 all codes belongs to Cubeat licensed under Apache License 2.0

== website
 * http://cubeat.game.tw
 
== core team
 * Please refer to:
   http://wiki.github.com/godfat/cubeat/the-team-of-project-shooting-cubes
 
== special thanks to
 * All the open source library devs. (please check dependencies below)
 * ?
 * ?
 * ?
 * ?
 * There's too many people to thank. so I guess let's thank god.

== runtime requirement
 you need the following additional requirements to play the game:
 * OpenGL enabled box with win32, linux/X11 or MacOSX/X11
 * ruby 1.8+ (for generating puzzles on non-win32 platforms)
 * ruby gem ludy (for generating puzzles)

== devel requirement
 you need the following library to build the cubeat-core:
 * g++ 4.2+ are tested, g++ 4.5 is recommended.
 * Boost 1.40+ is recommended. system, thread and date_time binary required.
 * cubeat/irrlicht (already included, please see below)
 * manymouse (already included)
 * wiiyourself! 0.99+ (now working on newer version's compatibility)
 * IrrKlang 1.0+ 
 
 * ruby 1.8+ (for preprocessing)
 * ruby gem erubis (for automations)
 * ruby gem ludy (for rake tasks and puzzle generating)
 * CMake (for building automations, not necessary if you use IDEs like codeBlocks)
 * X11 (if building on non-windows platform. mac requires X11 too)
 
 you need the following library to build the cubeat/irrlicht:
 * OpenGL APIs.
 * Freetype 2.3+
 * cubeat/irrlicht is a customized irrlicht based on irrlicht pre-1.6 svn version.
   (now working on updating to mainstream irrlicht 1.7.1)

== build
 run
   $ rake build
 to build the entire project.
 
 if you are using IDEs like codeBlocks (a few overly simplified steps):

   1.build boost with: system, thread, date_time. (shared)
   2.build freetype statically.
   3.build cubeat/irrlicht (shared) with statically linked freetype library.
   4.build wiiyourself! statically (this feature is win32 only)
   5.build manymouse statically or include all files it in the cubeat project file.
   6.preprocess the cubeat-core with "rake preprocess" to make sure the files are generated.
   7.build cubeat and link against at least cubeat/irrlicht and boost binaries.
     manymouse and wiiyourself! are optional. it will only be compiled and linked
     with _USE_WIIMOTE_ and _USE_MANYMOUSE_ defines.
   8.if all above process are successful, put the shared libraries 
     (boost, irrlicht, irrklang, etc) with the executable and it should run. 
   9.send an email to us to request the resource files. 
     (graphics, textures, sounds and musics.)

== startup
 run
   $ bin/cubeat
 to play the game. (you'll need X11 under non-windows platform.)

== others
 run
   $ rake --tasks
 to see other tasks for automations.
