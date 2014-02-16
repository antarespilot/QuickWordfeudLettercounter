QT += qml quick gui

SOURCES += main.cpp \
           game.cpp \
    words.cpp \
    letters.cpp \
    gamemanager.cpp

HEADERS += \
    game.h \
    words.h \
    letters.h \
    gamemanager.h

RESOURCES += \
    main.qrc

OTHER_FILES += \
    main.qml \
    GameView.qml
