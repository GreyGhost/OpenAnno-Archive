#!/bin/sh
svn update
cmake .
make
cd ~
mkdir OpenAnno
cd OpenAnno
mkdir data
cp -r ~/trunk/src/openanno ~/OpenAnno/
cp -r ~/trunk/data/* ~/OpenAnno/data/
cp ~/trunk/icon.png ~/OpenAnno/
strip openanno
