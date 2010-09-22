QT       += core gui
TARGET    = DTBViewer
TEMPLATE  = app

SOURCES += main.cpp \
           DTFile.cpp \
           DataTable.cpp \
           DTColumn.cpp \
           DTXor.cpp \
           DTRow.cpp \
           wndMain.cpp

HEADERS += DTFile.h \
           DataTable.h \
           DT.h \
           DTColumn.h \
           DTXor.h \
           DTRow.h \
           wndMain.h

FORMS   += wndMain.ui
