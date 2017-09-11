TEMPLATE = app
TARGET = qtlauncher
QT += quick
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += link_pkgconfig

#static {
#    QT += svg
#    QTPLUGIN += qtvirtualkeyboardplugin
#}

HEADERS += \
    database.h \
    contracttypemodel.h \
    basecontract.h \
    contractfactory.h \
    hourlycontract.h \
    monthlycontract.h \
    salescontract.h \
    modeloperations.h \
    paymentinfomodel.h \
    employeemodel.h

SOURCES += \
    main.cpp \
    contracttypemodel.cpp \
    basecontract.cpp \
    contractfactory.cpp \
    hourlycontract.cpp \
    monthlycontract.cpp \
    salescontract.cpp \
    modeloperations.cpp \
    paymentinfomodel.cpp \
    employeemodel.cpp


#RESOURCES += \
#   $$files(*.qml)
RESOURCES = contactlist_modified.qrc

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
target.path = $$[INSTALL_ROOT]/usr/bin

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = $$PWD/designer

OTHER_FILES += \
    designer/Backend/*.qml

#target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols2/contactlist
INSTALLS += target

#DISTFILES +=

#DISTFILES += \
#    designer/Backend/PaymentListModel.qml

DISTFILES += \
    PaymentDialog.qml

CONFIG += disable-desktop

#disable-xcb {
#    message("The disable-xcb option has been deprecated. Please use disable-desktop instead.")
#    CONFIG += disable-desktop
#}

#disable-desktop|android-embedded|!isEmpty(CROSS_COMPILE)|qnx {
#    DEFINES += MAIN_QML=\\\"basic-b2qt.qml\\\"
#} else {
#    DEFINES += MAIN_QML=\\\"Basic.qml\\\"
#}
