project(kcoloredit)
find_package (KDE4 REQUIRED)
include(KDE4Defaults)
include_directories ( ${KDE4_INCLUDES} )

set(kcoloredit_SRCS kzcolorselector.cpp imageselection.cpp texteditselection.cpp gradientselection.cpp colorselector.cpp kxycolorselector.cpp paletteview.cpp paletteviewscrolledarea.cpp editablestreamhistory.cpp color.cpp palette.cpp loadpalettedlg.cpp kcoloreditview.cpp kcoloreditdoc.cpp kcoloredit.cpp main.cpp)

kde4_add_executable(kcoloredit ${kcoloredit_SRCS})

target_link_libraries(kcoloredit ${KDE4_KIO_LIBS})

install(TARGETS kcoloredit  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES kcoloredit.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kcoloreditui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kcoloredit )

kde4_install_icons( ${ICON_INSTALL_DIR}   )

