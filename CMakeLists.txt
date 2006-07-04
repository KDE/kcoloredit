project(kcoloredit)




########### next target ###############

set(kcoloredit_SRCS kzcolorselector.cpp imageselection.cpp texteditselection.cpp gradientselection.cpp colorselector.cpp kxycolorselector.cpp paletteview.cpp paletteviewscrolledarea.cpp editablestreamhistory.cpp color.cpp palette.cpp loadpalettedlg.cpp kcoloreditview.cpp kcoloreditdoc.cpp kcoloredit.cpp main.cpp)

kde4_automoc(${kcoloredit_SRCS})

kde4_add_executable(kcoloredit ${kcoloredit_SRCS})

target_link_libraries(kcoloredit  ${KDE4_KDE3SUPPORT_LIBS} )

install_targets(/bin kcoloredit )


########### next target ###############

set(kcolorchooser_SRCS kcolorchooser.cpp )

kde4_automoc(${kcolorchooser_SRCS})

kde4_add_executable(kcolorchooser ${kcolorchooser_SRCS})

target_link_libraries(kcolorchooser  ${KDE4_KIO_LIBS} )

install_targets(/bin kcolorchooser )


########### install files ###############

install( FILES kcoloredit.desktop kcolorchooser.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kcoloreditui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kcoloredit )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )




#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#
#kcoloredit_SOURCES = kzcolorselector.cpp imageselection.cpp \
#	texteditselection.cpp gradientselection.cpp colorselector.cpp \
#	kxycolorselector.cpp paletteview.cpp paletteviewscrolledarea.cpp \
#	editablestreamhistory.cpp color.cpp palette.cpp loadpalettedlg.cpp \
#	kcoloreditview.cpp kcoloreditdoc.cpp kcoloredit.cpp main.cpp 
#kcoloredit_METASOURCES = AUTO
#kcoloredit_LDADD = $(LIB_KDEUI) $(LIB_KFILE) $(LIB_KDE3SUPPORT)
#kcoloredit_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#
#rcdir = $(kde_datadir)/kcoloredit
#rc_DATA = kcoloreditui.rc
#
#bin_PROGRAMS = kcoloredit kcolorchooser
#kcolorchooser_SOURCES = kcolorchooser.cpp
#kcolorchooser_LDADD = $(LIB_KDEUI)
#kcolorchooser_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#
#noinst_HEADERS = color.h colorselector.h editablestreamhistory.h \
#	gradientselection.h imageselection.h kcoloredit.h kcoloreditdoc.h \
#	kcoloreditview.h kxycolorselector.h kzcolorselector.h \
#	loadpalettedlg.h main.h palette.h palettehistory.h paletteview.h \
#	resource.h texteditselection.h
#
#xdg_apps_DATA = kcoloredit.desktop kcolorchooser.desktop
#
#KDE_ICON = kcoloredit kcolorchooser
#
#EXTRA_DIST = $(xdg_apps_DATA)
#
#messages: rc.cpp
#	$(XGETTEXT) $(kcoloredit_SOURCES) resource.h -o $(podir)/kcoloredit.pot
#
