#!/bin/sh

install_name_tool -change /usr/local/lib/libirrklang.dylib libirrklang.dylib bin/libshooting-cubes.dylib

install_name_tool -change /usr/local/lib/libirrklang.dylib libirrklang.dylib bin/test-game-main
install_name_tool -change /usr/local/lib/libirrklang.dylib libirrklang.dylib bin/test-map-cubes
install_name_tool -change /usr/local/lib/libirrklang.dylib libirrklang.dylib bin/test-map-views
install_name_tool -change /usr/local/lib/libirrklang.dylib libirrklang.dylib bin/puzzle
