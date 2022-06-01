QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# for correct output in console - testing
CONFIG += console

# non-compilable libs like ASIO lie here
INCLUDEPATH += C:/boost/boost_1_79_0

# 4 lines to find all Boost dependencies
win32:LIBS += -lsetupapi
win32:LIBS += -lwsock32
win32:LIBS += -lws2_32
win32:DEFINES += _WIN32_WINNT=0x0501

SOURCES += \
        log.cpp \
        main.cpp \
        messenger.cpp \
        session.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    log.h \
    messenger.h \
    session.h
