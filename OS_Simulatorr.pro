#-------------------------------------------------
#
# Project created by QtCreator 2012-10-19T21:45:16
#
#-------------------------------------------------

QT       += core gui

TARGET = OS_Simulatorr
TEMPLATE = app


SOURCES += main.cpp\
        os_sim.cpp \
    fcfs.cpp \
    sjf.cpp \
    Sched.cpp \
    prority.cpp \
    rr.cpp

HEADERS  += os_sim.h \
    fcfs.h \
    Sched.h \
    sjf.h \
    prority.h \
    rr.h

FORMS    += os_sim.ui \
    fcfs.ui \
    sjf.ui \
    prority.ui \
    rr.ui
