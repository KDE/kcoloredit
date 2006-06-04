INCLUDES = $(all_includes)

kcoloredit_SOURCES = kzcolorselector.cpp imageselection.cpp \
	texteditselection.cpp gradientselection.cpp colorselector.cpp \
	kxycolorselector.cpp paletteview.cpp paletteviewscrolledarea.cpp \
	editablestreamhistory.cpp color.cpp palette.cpp loadpalettedlg.cpp \
	kcoloreditview.cpp kcoloreditdoc.cpp kcoloredit.cpp main.cpp
kcoloredit_METASOURCES = AUTO
kcoloredit_LDADD = $(LIB_KDEUI) $(LIB_KFILE)
kcoloredit_LDFLAGS = $(all_libraries) $(KDE_RPATH)

rcdir = $(kde_datadir)/kcoloredit
rc_DATA = kcoloreditui.rc

bin_PROGRAMS = kcoloredit kcolorchooser
kcolorchooser_SOURCES = kcolorchooser.cpp
kcolorchooser_LDADD = $(LIB_KDEUI)
kcolorchooser_LDFLAGS = $(all_libraries) $(KDE_RPATH)

noinst_HEADERS = color.h colorselector.h editablestreamhistory.h \
	gradientselection.h imageselection.h kcoloredit.h kcoloreditdoc.h \
	kcoloreditview.h kxycolorselector.h kzcolorselector.h \
	loadpalettedlg.h main.h palette.h palettehistory.h paletteview.h \
	resource.h texteditselection.h

xdg_apps_DATA = kcoloredit.desktop kcolorchooser.desktop

KDE_ICON = kcoloredit kcolorchooser

EXTRA_DIST = $(xdg_apps_DATA)

messages: rc.cpp
	$(EXTRACTRC) *.rc > rc.cpp
	$(XGETTEXT) $(kcoloredit_SOURCES) rc.cpp resource.h -o $(podir)/kcoloredit.pot

