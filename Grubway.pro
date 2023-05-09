QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    views/AdminDashboard/AdminArea/edituser.cpp \
    views/AdminDashboard/AdminArea/resetpass.cpp \
    views/AdminDashboard/AdminArea/vieworder.cpp \
    views/AdminDashboard/AdminArea/viewuser.cpp \
    views/CashierScreen/CashierScreen.cpp \
    views/Funcs/AddItem/additem.cpp \
    views/AdminDashboard/admindashboard.cpp \
    views/Funcs/HistoryDialog/historydialog.cpp \
    Database.cpp \
    views/CashierScreen/inventoryscreen.cpp \
    views/Funcs/ItemInputDialog/iteminputdialog.cpp \
    main.cpp \
    views/Login/login.cpp \
    views/Login/register.cpp

HEADERS += \
    views/AdminDashboard/AdminArea/edituser.h \
    views/AdminDashboard/AdminArea/resetpass.h \
    views/AdminDashboard/AdminArea/vieworder.h \
    views/AdminDashboard/AdminArea/viewuser.h \
    views/CashierScreen/CashierScreen.h \
    views/Funcs/AddItem/additem.h \
    views/AdminDashboard/admindashboard.h \
    views/Funcs/HistoryDialog/historydialog.h \
    enums/account.h \
    inc/common.h \
    Database.h \
    enums/Item.h \
    enums/logs.h \
    enums/orders.h \
    enums/receipts.h \
    views/CashierScreen/inventoryscreen.h \
    views/Login/login.h \
    views/Login/register.h \
    views/Funcs/ItemInputDialog/iteminputdialog.h \
    views/Funcs/Receipt/receipt.h

FORMS += \
    views/AdminDashboard/AdminArea/edituser.ui \
    views/AdminDashboard/AdminArea/resetpass.ui \
    views/AdminDashboard/AdminArea/vieworder.ui \
    views/AdminDashboard/AdminArea/viewuser.ui \
    views/CashierScreen/CashierScreen.ui \
    views/Funcs/AddItem/additem.ui \
    views/AdminDashboard/admindashboard.ui \
    views/Funcs/HistoryDialog/historydialog.ui \
    views/CashierScreen/inventoryscreen.ui \
    views/Login/login.ui \
    views/Login/register.ui

# Install python server app
win32: first.commands = $(COPY_DIR) $$shell_path($$PWD/web/) $$shell_path($$OUT_PWD/web)
unix:!macx: first.commands = $(COPY_DIR) $$PWD/web/ $$OUT_PWD/web
macx: first.commands = $(COPY_DIR) $$PWD/web/ $$OUT_PWD/Grubway.app/Contents/MacOS/web
export(first.commands)
QMAKE_EXTRA_TARGETS += first

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc \
   icons/windowsicon.qrc

DISTFILES += \
    inc/Account \
    inc/AddItem \
    inc/AdminDashboard \
    inc/CashierScreen \
    inc/Database \
    inc/EditUser \
    inc/HistoryDialog \
    inc/InventoryScreen \
    inc/Item \
    inc/ItemInputDialog \
    inc/Login \
    inc/Logs \
    inc/Orders \
    inc/Receipts \
    inc/Register \
    inc/ResetPass \
    inc/ViewUser \
    web/script.py \
    web/static/Grubway.png \
    web/templates/cart.html \
    web/templates/index.html \
    web/templates/orderDetails.html \
    web/templates/orders.html \
    web/templates/register.html \
    web/templates/settings.html \
    web/templates/store.html

INCLUDEPATH += $$PWD/inc

# Set the Mac application icon and copy the PNG icon to the app bundle
macx {
    ICON_SOURCE_PATH = $$PWD/icons/grubwayicon.icns
    ICON_TARGET_PATH = $$OUT_PWD/Grubway.app/Contents/Resources
    QMAKE_POST_LINK = mkdir -p $$ICON_TARGET_PATH && cp $$ICON_SOURCE_PATH $$ICON_TARGET_PATH

    PNG_SOURCE_PATH = $$PWD/icons/grubwayicon.png
    PNG_TARGET_PATH = $$OUT_PWD/Grubway.app/Contents/Resources
    QMAKE_POST_LINK += && cp $$PNG_SOURCE_PATH $$PNG_TARGET_PATH
}

# QMake Artifact variables
VERSION = 1.0.0.0
QMAKE_TARGET_DEVELOPER = "The Vanguard"
QMAKE_TARGET_PRODUCT = "Grubway"
QMAKE_TARGET_DESCRIPTION = "Get your Grub on(TM)"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2023 Grubway"


