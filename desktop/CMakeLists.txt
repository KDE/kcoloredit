install ( PROGRAMS kcoloredit.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install ( FILES kcoloreditui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kcoloredit )
install ( FILES text-input.png controls-input.png DESTINATION  ${DATA_INSTALL_DIR}/kcoloredit/pics )

kde4_install_icons( ${ICON_INSTALL_DIR} )
