install ( FILES kcoloredit.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install ( FILES kcoloreditui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kcoloredit )

kde4_install_icons( ${ICON_INSTALL_DIR} )
