QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/../common/common.pri)

INCLUDEPATH += $$PWD/../common

SOURCE_FILES = $$files($$PWD/*.cpp, false)
HEADER_FILES = $$files($$PWD/*.h, false)
RESOURCE_FILES = $$files($$PWD/*.qrc, false)

SOURCES += $${SOURCE_FILES}

HEADERS += $${HEADER_FILES}

RESOURCES += $${RESOURCE_FILES}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
