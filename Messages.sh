#!bin/sh
$EXTRACTRC *.rc */*.rc >> rc.cpp
$XGETTEXT `find -name '*.h' -o -name '*.cpp'` -o $podir/kcoloredit.pot
