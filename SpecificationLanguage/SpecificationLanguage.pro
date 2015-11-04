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

INCLUDEPATH += "/home/giulio/astl/tags/2.0/include" \
               "/home/giulio/boost_1_59_0/" \
               "/home/giulio/Grail_int/"
