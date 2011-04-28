#!/bin/bash

# remove shortcut from menu
# locate desktop
desktop_dir=$(xdg-user-dir DESKTOP)
if [ "$desktop_dir" == "" ] ; then
  # no xdg-user-dir found or it does not know where to find desktop
  # assume $HOME/Desktop
  desktop_dir=$HOME/Desktop
fi

# remove shortcut from desktop if exists
if [ -e $desktop_dir/CuBeat.desktop ] ; then
  rm $desktop_dir/CuBeat.desktop
fi
