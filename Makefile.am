INCLUDES = $(all_includes)

kcoloredit_SOURCES = kzcolorselector.cpp imageselection.cpp \
	texteditselection.cpp gradientselection.cpp colorselector.cpp \
	kxycolorselector.cpp paletteview.cpp paletteviewscrolledarea.cpp \
	editablestreamhistory.cpp color.cpp palette.cpp loadpalettedlg.cpp \
	kcoloreditview.cpp kcoloreditdoc.cpp kcoloredit.cpp main.cpp 
kcoloredit_METASOURCES = AUTO
kcoloredit_LDADD = $(LIB_KDEUI) $(LIB_KFILE)
kcoloredit_LDFLAGS = $(all_libraries) $(KDE_RPATH)

bin_PROGRAMS = kcoloredit kcolorchooser
kcolorchooser_SOURCES = kcolorchooser.cpp
kcolorchooser_LDADD = $(LIB_KDEUI)
kcolorchooser_LDFLAGS = $(all_libraries) $(KDE_RPATH)

lib_LTLIBRARIES = kcolorchooser.la
kcolorchooser_la_SOURCES = kcolorchooser.cpp
kcolorchooser_la_LIBADD = $(LIB_KDEUI)
kcolorchooser_la_LDFLAGS = $(all_libraries) -module -avoid-version

noinst_HEADERS = color.h colorselector.h editablestreamhistory.h \
	gradientselection.h imageselection.h kcoloredit.h kcoloreditdoc.h \
	kcoloreditview.h kxycolorselector.h kzcolorselector.h \
	loadpalettedlg.h main.h palette.h palettehistory.h paletteview.h \
	resource.h texteditselection.h

lnk_DATA = kcoloredit.desktop kcolorchooser.desktop
lnkdir = $(kde_appsdir)/Graphics

KDE_ICON = kcoloredit kcolorchooser

EXTRA_DIST = $(lnk_DATA)

messages:
	$(XGETTEXT) $(kcoloredit_SOURCES) -o $(podir)/kcoloredit.pot

