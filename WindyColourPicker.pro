QT       += core gui widgets
TARGET = WindyColourPicker
TEMPLATE = app

win32: LIBS += -lgdi32 -luser32

RESOURCES += res/resources.qrc
RC_FILE = res/app.rc

SOURCES += \
    main.cpp \
    WindyColourPicker.cpp \
    ColorWheel.cpp \
    ColorTipWidget.cpp

HEADERS += \
    WindyColourPicker.h \
    ColorWheel.h \
    ColorTipWidget.h