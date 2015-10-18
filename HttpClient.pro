TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    httpclient.cpp \
    headertypes.cpp \
    httpmessages.cpp \
    requesttypes.cpp

OTHER_FILES += \
    changelog.txt

HEADERS += \
    httpclient.h \
    headertypes.h \
    clientactions.h \
    requesttypes.h \
    httpmessages.h

