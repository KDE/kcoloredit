install ( FILES kcoloredit.desktop DESTINATION  ${KDE_INSTALL_APPDIR} )
install ( FILES kcoloreditui.rc  DESTINATION  ${KXMLGUI_INSTALL_DIR}/kcoloredit )
install ( FILES text-input.png controls-input.png DESTINATION  ${KDE_INSTALL_DATAROOTDIR}/kcoloredit/pics )

ecm_install_icons( ${ICON_INSTALL_DIR} )
