#!/bin/bash

# This script will help you to create desktop shortcut to CuBeat.
# And it will prompt for ManyMouse support.

echo "
Creating the desktop shortcut..."
chmod 755 rc/create_shortcut.sh
echo "
Done."
chmod 755 rc/enable_manymouse.sh

rc/create_shortcut.sh
rc/enable_manymouse.sh

