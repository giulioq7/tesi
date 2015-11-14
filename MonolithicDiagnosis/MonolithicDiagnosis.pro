TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../Classes/component.cpp \
    ../Classes/componentmodel.cpp \
    ../Classes/nettransition.cpp \
    ../Classes/networkmodel.cpp \
    ../Classes/ostream_util.cpp \
    ../Classes/problem.cpp \
    ../Classes/problemnode.cpp \
    ../Classes/statedata.cpp \
    ../Classes/system.cpp \
    ../Classes/systemnode.cpp \
    ../Classes/transition.cpp \
    ../Classes/behaviorstate.cpp \
    ../Classes/systransition.cpp

HEADERS += \
    ../Classes/component.h \
    ../Classes/componentmodel.h \
    ../Classes/nettransition.h \
    ../Classes/networkmodel.h \
    ../Classes/ostream_util.h \
    ../Classes/problem.h \
    ../Classes/problemnode.h \
    ../Classes/serialize.h \
    ../Classes/statedata.h \
    ../Classes/system.h \
    ../Classes/systemnode.h \
    ../Classes/transition.h \
    ../Classes/utils.h \
    ../Classes/behaviorstate.h \
    ../Classes/systransition.h

INCLUDEPATH += "../Libraries/astl/tags/2.0/include" \
               "../Libraries/boost_1_59_0" \
               "../Classes/"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Libraries/boost_1_59_0/lib/release/ -lboost_serialization
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Libraries/boost_1_59_0/lib/debug/ -lboost_serialization
else:unix: LIBS += -L$$PWD/../Libraries/boost_1_59_0/lib/ -lboost_serialization

INCLUDEPATH += $$PWD/../Libraries/boost_1_59_0
DEPENDPATH += $$PWD/../Libraries/boost_1_59_0

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Libraries/boost_1_59_0/lib/release/libboost_serialization.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Libraries/boost_1_59_0/lib/debug/libboost_serialization.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Libraries/boost_1_59_0/lib/release/boost_serialization.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Libraries/boost_1_59_0/lib/debug/boost_serialization.lib
else:unix: PRE_TARGETDEPS += $$PWD/../Libraries/boost_1_59_0/lib/libboost_serialization.a
