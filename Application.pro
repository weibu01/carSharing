#-------------------------------------------------
#
# Project created by QtCreator 2019-04-11T11:19:43
#
#-------------------------------------------------

QT       += core gui sql
QT += network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Application
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    createdb.cpp \
    carmanage.cpp \
    applicantmanage.cpp \
    addcardlg.cpp \
    addapplicantdlg.cpp \
    createdbone.cpp \
    checkwindow.cpp \
    obj.cpp

HEADERS  += mainwindow.h \
    createdb.h \
    carmanage.h \
    applicantmanage.h \
    addcardlg.h \
    addapplicantdlg.h \
    createdbone.h \
    checkwindow.h \
    obj.h

FORMS    += mainwindow.ui \
    carmanage.ui \
    applicantmanage.ui \
    addcardlg.ui \
    addapplicantdlg.ui \
    checkwindow.ui


