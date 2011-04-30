#!/bin/sh

script=$(readlink -f $0)
script_path=$(dirname $script)

if [ ! -e rc/config.zzml ] || [ ! -d rc/config ] || [ ! -d rc/script ] || [ ! -d rc/fonts ] || [ ! -d rc/model ] || [ ! -d rc/sound ] || [ ! -d rc/texture ]
then
  echo "You are missing resource files! This distribution is not complete."
  exit 1
fi

march=$(uname -m)
if [ $march = x86_64 ]
then
   libs_path=./lib64
else
   libs_path=./lib32
fi

env LD_LIBRARY_PATH=$libs_path:$LD_LIBRARY_PATH ./bin/CuBeat
