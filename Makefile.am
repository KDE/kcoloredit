INCLUDES = $(all_includes)
LDFLAGS = $(all_libraries) $(KDE_RPATH)
LDADD =   $(LIB_KDEUI) $(LIB_KFILE)

kcoloredit_SOURCES = kzcolorselector.cpp imageselection.cpp \
	texteditselection.cpp gradientselection.cpp colorselector.cpp \
	kxycolorselector.cpp paletteview.cpp editablestreamhistory.cpp \
	color.cpp palette.cpp loadpalettedlg.cpp kcoloreditview.cpp \
	kcoloreditdoc.cpp kcoloredit.cpp main.cpp 
kcoloredit_METASOURCES = AUTO
bin_PROGRAMS = kcoloredit

noinst_HEADERS = color.h colorselector.h editablestreamhistory.h \
	gradientselection.h imageselection.h kcoloredit.h kcoloreditdoc.h \
	kcoloreditview.h kxycolorselector.h kzcolorselector.h \
	loadpalettedlg.h main.h palette.h palettehistory.h paletteview.h \
	resource.h texteditselection.h

lnk_DATA = kcoloredit.desktop
lnkdir = $(kde_appsdir)/Graphics

KDE_ICON = kcoloredit

EXTRA_DIST = $(lnk_DATA)

messages:
	$(XGETTEXT) $(kcoloredit_SOURCES) -o $(podir)/kcoloredit.pot

