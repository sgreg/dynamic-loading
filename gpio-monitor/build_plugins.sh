#!/bin/bash
#
# Hackaday - Dynamic Loading
# GPIO Monitor Plugin System
#
# Build script to compile all plugins listed in $PLUGINS below
#

PLUGINS_DIR="plugins/"
PLUGINS="dummy print logger"

CFLAGS="-shared -fPIC -Wall -Wextra -I.."

cd $PLUGINS_DIR
for plugin in $PLUGINS ; do
    echo "Building plugin $plugin"
    gcc $CFLAGS -o $plugin.plugin plugin-$plugin.c || break
done
cd - >/dev/null

