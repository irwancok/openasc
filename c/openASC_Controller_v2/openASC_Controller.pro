TEMPLATE = app
QT = gui \
    core \
    network
CONFIG += qt \
    warn_on \
    console \
    debug
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui \
    ui/rotatorwindow.ui \
    ui/settingsdialog.ui \
    ui/terminaldialog.ui \
		ui/errordialog.ui
HEADERS = src/mainwindowimpl.h \
    src/generic.h \
    src/main.h \
    src/rotatordialog.h \
    src/settingsdialog.h \
    src/commands.h \
    src/terminaldialog.h \
		src/errordialog.h \
		src/tcpclass.h
SOURCES = src/mainwindowimpl.cpp \
    src/main.cpp \
    src/generic.cpp \
    src/rotatordialog.cpp \
    src/settingsdialog.cpp \
    src/terminaldialog.cpp \
		src/errordialog.cpp \
		src/tcpclass.cpp
