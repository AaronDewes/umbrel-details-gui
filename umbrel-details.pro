QT += gui waylandcompositor svg
INCLUDEPATH += qrcode/

HEADERS += \
    window.h

SOURCES += main.cpp \
    window.cpp \
    qrcode/QrCode.cpp

TARGET = umbrel-details
INSTALLS += target
