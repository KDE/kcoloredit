cmake_minimum_required(VERSION 2.8.12)
project ( kcoloredit )

set(QT_MIN_VERSION "5.7.0")
find_package(ECM 1.6.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
include(KDEInstallDirs)
include(KDECMakeSettings NO_POLICY_SCOPE)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui Widgets)
find_package(KF5 REQUIRED COMPONENTS I18n Config DocTools KDELibs4Support)

ADD_SUBDIRECTORY ( src )
ADD_SUBDIRECTORY ( desktop )
ADD_SUBDIRECTORY ( doc )
