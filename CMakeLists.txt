project(kcoloredit)




########### next target ###############

set(kcoloredit_SRCS kzcolorselector.cpp imageselection.cpp texteditselection.cpp gradientselection.cpp colorselector.cpp kxycolorselector.cpp paletteview.cpp paletteviewscrolledarea.cpp editablestreamhistory.cpp color.cpp palette.cpp loadpalettedlg.cpp kcoloreditview.cpp kcoloreditdoc.cpp kcoloredit.cpp main.cpp)

kde4_automoc(${kcoloredit_SRCS})

kde4_add_executable(kcoloredit ${kcoloredit_SRCS})

target_link_libraries(kcoloredit ${KDE4_KIO_LIBS})

install(TARGETS kcoloredit  DESTINATION ${BIN_INSTALL_DIR} )


########### next target ###############

set(kcolorchooser_SRCS kcolorchooser.cpp )

kde4_automoc(${kcolorchooser_SRCS})

kde4_add_executable(kcolorchooser ${kcolorchooser_SRCS})

target_link_libraries(kcolorchooser ${KDE4_KDEUI_LIBS})

install(TARGETS kcolorchooser  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kcoloredit.desktop kcolorchooser.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kcoloreditui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kcoloredit )

kde4_install_icons( ${ICON_INSTALL_DIR}   )

