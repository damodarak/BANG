QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ask.cpp \
    card.cpp \
    char_bart.cpp \
    char_blackj.cpp \
    char_calamity.cpp \
    char_carlson.cpp \
    char_jesse.cpp \
    char_jourd.cpp \
    char_ketchum.cpp \
    char_lucky.cpp \
    char_pedro.cpp \
    char_slab.cpp \
    char_suzy.cpp \
    game.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp

HEADERS += \
    ask.h \
    card.h \
    char_bart.h \
    char_blackj.h \
    char_calamity.h \
    char_carlson.h \
    char_cringo.h \
    char_jesse.h \
    char_jourd.h \
    char_ketchum.h \
    char_lucky.h \
    char_paul.h \
    char_pedro.h \
    char_rose.h \
    char_slab.h \
    char_suzy.h \
    char_vulture.h \
    char_willy.h \
    characters.h \
    game.h \
    mainwindow.h \
    player.h

FORMS += \
    ask.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
