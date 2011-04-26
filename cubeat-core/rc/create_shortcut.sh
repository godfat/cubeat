#!/bin/bash

# Absolute path to this script. 
script=$(readlink -f $0)
# Absolute path this script is in. 
script_path=$(dirname $script)

echo "
[Desktop Entry]
Value=0.5
Type=Application
Name=CuBeat
Categories=Game
Exec=$script_path/../cubeat
Path=$script_path/../
Icon=$script_path/CuBeat.png
Name[en_US]=CuBeat
GenericName[en_US.utf8]=CuBeat 0.5alpha demo
" > CuBeat.desktop

# locate desktop
desktop_dir=$(xdg-user-dir DESKTOP)
if [ "$desktop_dir" == "" ] ; then
	# no xdg-user-dir found or it does not know where to find desktop
	# assume $HOME/Desktop
	desktop_dir=$HOME/Desktop
fi

cp CuBeat.desktop $desktop_dir
chmod 755 $desktop_dir/CuBeat.desktop


