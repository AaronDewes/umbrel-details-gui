QT += gui svg network
INCLUDEPATH += qrcode/

HEADERS += \
    window.h

SOURCES += main.cpp \
    window.cpp \
    qrcode/QrCode.cpp

RESOURCES += umbrel-details.qrc

TARGET = umbrel-details
INSTALLS += target
