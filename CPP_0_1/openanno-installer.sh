#!/bin/bash
# TODO: Support »the GNOME way«
INSTALLDIR="`kdialog --getexistingdirectory '~' --title 'Installations Ordner wählen'`"
if ($? -ne 0) ; then
exit
fi
cmake .
make
cd ..
mkdir -p $INSTALLDIR
cp -r src/openanno $INSTALLDIR
cp -r data/ $INSTALLDIR
cp icon.png $INSTALLDIR
