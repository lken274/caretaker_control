#!/bin/bash
export LD_LIBRARY_PATH=./build-ct4ui-output/lib

if [[ $(id -u) == 0 ]]; then
    # running as root.
    # pass special argument to ajust y-offset for menubar placement on window
    ./build-ct4ui-output/ct4ui --root
else
    ./build-ct4ui-output/ct4ui
fi
