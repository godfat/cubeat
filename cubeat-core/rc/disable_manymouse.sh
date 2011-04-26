#!/bin/bash

# This script will undo ManyMouse support enabled by enable_input.sh

echo "
Removing ManyMouse support for this account:

 - it will remove the udev rule and remove the user from \"input\" group.
 - it won't remove the \"input\" user group. if you want to do this, simply:
   \"sudo groupdel input\" after you have run this script.

NOTICE: You have to re-login to make these rule changes to take effect.

Do you want to proceed? (y/n) "
read ans

if [ "$ans" == "y" ] ; then
  username=$(whoami)
  sudo rm /etc/udev/rules.d/85-cubeat.rules
  sudo gpasswd -d $username input
fi

