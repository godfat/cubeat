#!/bin/bash

# This script will setup an "input" user group and add current user to that group, 
# and then setup a specific udev rule, in order to enable multiple mice polling 
# from /dev/input. (Otherwise only system mouse can be used. However, it is still
# possible for the second player on the same machine to play versus mode with key-
# board, just it's quite unfair.)

echo "
ManyMouse support: 

This will setup an \"input\" user group and add current user to that group, 
and then setup a specific udev rule, in order to enable multiple mice polling 
from /dev/input. (Otherwise only system mouse can be used. However, it's still
possible for the second player on the same machine to play versus mode with 
keyboard, just it's quite unfair.)

NOTICE: You have to re-login to make these rule changes to take effect.

Do you want to proceed? (y/n) "
read ans

if [ "$ans" == "y" ] ; then
  # Absolute path to this script. 
  script=$(readlink -f $0)
  # Absolute path this script is in. 
  script_path=$(dirname $script)
  username=$(whoami)
  sudo cp $script_path/85-cubeat.rules /etc/udev/rules.d
  sudo groupadd -f input
  sudo gpasswd -a $username input
fi

