TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    spec_driver.cpp \
    specparser.cpp \
    speclexer.cpp \
    ../SharedClasses/component.cpp \
    ../SharedClasses/componentmodel.cpp \
    ../SharedClasses/nettransition.cpp \
    ../SharedClasses/networkmodel.cpp \
    ../SharedClasses/ostream_util.cpp \
    ../SharedClasses/outputterminal.cpp \
    ../SharedClasses/problem.cpp \
    ../SharedClasses/problemnode.cpp \
    ../SharedClasses/statedata.cpp \
    ../SharedClasses/system.cpp \
    ../SharedClasses/systemnode.cpp \
    ../SharedClasses/systransition.cpp \
    ../SharedClasses/terminal.cpp \
    ../SharedClasses/transition.cpp

OTHER_FILES += \
    specparser.yy \
    speclexer.ll

HEADERS += \
    spec_driver.h \
    specparser.hpp \
    grail_lib.h \
    ../SharedClasses/component.h \
    ../SharedClasses/componentmodel.h \
    ../SharedClasses/nettransition.h \
    ../SharedClasses/networkmodel.h \
    ../SharedClasses/ostream_util.h \
    ../SharedClasses/outputterminal.h \
    ../SharedClasses/problem.h \
    ../SharedClasses/problemnode.h \
    ../SharedClasses/serialize.h \
    ../SharedClasses/statedata.h \
    ../SharedClasses/system.h \
    ../SharedClasses/systemnode.h \
    ../SharedClasses/systransition.h \
    ../SharedClasses/terminal.h \
    ../SharedClasses/transition.h \
    ../SharedClasses/utils.h

INCLUDEPATH += "../Libraries/astl/tags/2.0/include" \
               "./Libraries/boost_1_59_0" \
               "../Libraries/Grail/"\
               "../SharedClasses/"

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
