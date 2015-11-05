TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    spec_driver.cpp \
    specparser.cpp \
    speclexer.cpp \
    componentmodel.cpp \
    transition.cpp \
    component.cpp \
    networkmodel.cpp \
    ostream_util.cpp \
    system.cpp \
    systemnode.cpp \
    problemnode.cpp \
    problem.cpp \
    statedata.cpp \
    nettransition.cpp



OTHER_FILES += \
    specparser.yy \
    speclexer.ll

HEADERS += \
    spec_driver.h \
    specparser.hpp \
    componentmodel.h \
    transition.h \
    component.h \
    networkmodel.h \
    ostream_util.h \
    system.h \
    systemnode.h \
    problemnode.h \
    problem.h \
    utils.h \
    statedata.h \
    grail_lib.h \
    nettransition.h \
    serialize.h

INCLUDEPATH += "../Libraries/astl/tags/2.0/include" \
               "./Libraries/boost_1_59_0" \
               "../Libraries/Grail/"

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
