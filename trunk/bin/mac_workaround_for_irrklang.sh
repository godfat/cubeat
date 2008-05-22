#!/bin/sh

install_name_tool -change /usr/local/lib/libirrklang.dylib libirrklang.dylib bin/libcubeat.dylib

install_name_tool -change /usr/local/lib/libirrklang.dylib libirrklang.dylib bin/cubeat
