#!/bin/sh

# This will undo what setup.sh has done.

echo "
Removing the desktop shortcut..."
chmod 755 rc/remove_shortcut.sh
echo "
Done."
chmod 755 rc/disable_manymouse.sh

./rc/remove_shortcut.sh
./rc/disable_manymouse.sh
