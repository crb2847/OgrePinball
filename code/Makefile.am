ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h Game.h Entity.h

bin_PROGRAMS= OgrePinball
OgrePinball_CPPFLAGS= -I$(top_srcdir)
OgrePinball_SOURCES= BaseApplication.cpp Game.cpp Entity.cpp
OgrePinball_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS)
OgrePinball_LDADD= $(OGRE_LIBS) $(OIS_LIBS)
OgrePinball_LDFLAGS= -lOgreOverlay -lboost_system

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
