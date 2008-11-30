
== Cubeat
 all library remains original license, and
 all codes belongs to Cubeat licensed under Apache License 2.0

== Contribution
 * ...
 * ...
 * ...
 * ...
 * ...
 * ...

== special thanks to
 * ?
 * ?
 * ?
 * ?
 * ?
 * ?

== runtime requirement
 you need the following library to play the game:
 * Irrklang (for sound effects)
 * Ruby (for generating puzzles)
 * gem ludy (for generating puzzles)

== building requirement
 you need the following library to build the game:
 * a C++ compiler (g++ 4.3 is recommended (tested))
 * C++ TR1
 * Boost
 * modified Irrlicht (for graphics)
 * Freetype (for font graphics)
 * OpenGL (for graphics)
 * Irrklang (for sound effects)
 * Ruby (for preprocessing and puzzle generating)
 * gem rake (for automations)
 * gem ludy (for rake tasks and puzzle generating)
 * CMake (for building automations)
 * X11 (if building on non-windows platform. mac requires X11 too)

== build
 run
   $ rake build
 to build the entire project.

== startup
 run
   $ bin/cubeat
 to play the game. (you'll need X11 under non-windows platform.)

== others
 run
   $ rake --tasks
 to see other tasks for automations.
