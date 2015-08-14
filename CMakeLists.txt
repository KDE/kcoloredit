cmake_minimum_required(VERSION 2.8.9)
project ( kcoloredit )

find_package ( KDE4 REQUIRED )
include ( KDE4Defaults )
include ( MacroLibrary )

ADD_SUBDIRECTORY ( src )
ADD_SUBDIRECTORY ( desktop )
ADD_SUBDIRECTORY ( doc )
