QT += gui waylandcompositor svg
INCLUDEPATH += qrcode/

HEADERS += \
    window.h \
    compositor.h

SOURCES += main.cpp \
    window.cpp \
    compositor.cpp \
    qrcode/QrCode.cpp

TARGET = umbrel-details
INSTALLS += target
