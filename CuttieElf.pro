QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    analyze.cpp \
    analyzeelf.cpp \
    binarysection.cpp \
    dataencoder.cpp \
    excludelist.cpp \
    filterconfig.cpp \
    hashcalculator.cpp \
    main.cpp \
    mainwindow.cpp

#  resources.cpp

HEADERS += \
    analyze.h \
    analyzeelf.h \
    binarysection.h \
    config.h \
    dataencoder.h \
    excludelist.h \
    filterconfig.h \
    hashcalculator.h \
    mainwindow.h


FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    CuttieElf_en_US.ts
CONFIG += lrelease

CONFIG += embed_translations

CONFIG += static

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS+= -lelf
LIBS+= -lz
LIBS += -lfuzzy
LIBS += -lssl -lcrypto

DISTFILES += \
    todo

RESOURCES += \
    resources.qrc

QMAKE_LFLAGS += -fopenmp
QMAKE_CXXFLAGS += -fopenmp
QMAKE_CXXFLAGS += -fno-lto
QMAKE_CFLAGS   += -fno-lto
QMAKE_LFLAGS   += -fno-lto
LIBS  += -fopenmp
