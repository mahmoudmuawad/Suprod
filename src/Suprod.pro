QT       += core gui sql network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = suprod

VERSION = 0.0
TEMPLATE = app


SOURCES += \
    dayhabit.cpp \
    dayplan.cpp \
    days.cpp \
    habit.cpp \
    main.cpp \
    mainwindow.cpp \
    otask.cpp \
    qhpushbutton.cpp \
    task.cpp

HEADERS += \
    dayhabit.h \
    dayplan.h \
    days.h \
    habit.h \
    mainwindow.h \
    otask.h \
    qhpushbutton.h \
    task.h

FORMS += \
    dayhabit.ui \
    dayplan.ui \
    days.ui \
    habit.ui \
    mainwindow.ui \
# note : deleting ui you need to remove \
    otask.ui \
    task.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    suprod.qrc

INSTALLS += target

isEmpty(PREFIX) {
 PREFIX = /opt/$(TARGET)
}
target.path = $$PREFIX/
