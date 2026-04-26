QT       += core gui widgets
TARGET = WindyColourPicker
TEMPLATE = app

win32: LIBS += -lgdi32 -luser32

RESOURCES += res/resources.qrc
RC_FILE = res/app.rc

# Release 构建后自动清理中间文件和不必要依赖
win32:Release:QMAKE_POST_LINK = "$$shell_path($$PWD/clean_release.bat)"

SOURCES += \
    main.cpp \
    WindyColourPicker.cpp \
    ColorWheel.cpp \
    ColorTipWidget.cpp

HEADERS += \
    WindyColourPicker.h \
    ColorWheel.h \
    ColorTipWidget.h