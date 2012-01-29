#!/bin/sh

# you must have optipng installed to use this script

find -type f -name '*.png' -exec optipng -o4 {} \;
