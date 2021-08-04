/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QPushButton *openMaterialDialog;
    QPushButton *resetDB;
    QPushButton *importDB;
    QPushButton *exportDB;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(347, 200);
        MainWindow->setMouseTracking(true);
        MainWindow->setTabletTracking(true);
        MainWindow->setFocusPolicy(Qt::StrongFocus);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setCursor(QCursor(Qt::ArrowCursor));
        centralwidget->setMouseTracking(true);
        centralwidget->setTabletTracking(true);
        centralwidget->setFocusPolicy(Qt::StrongFocus);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 0, 0);
        openMaterialDialog = new QPushButton(centralwidget);
        openMaterialDialog->setObjectName(QString::fromUtf8("openMaterialDialog"));

        verticalLayout->addWidget(openMaterialDialog);

        resetDB = new QPushButton(centralwidget);
        resetDB->setObjectName(QString::fromUtf8("resetDB"));

        verticalLayout->addWidget(resetDB);

        importDB = new QPushButton(centralwidget);
        importDB->setObjectName(QString::fromUtf8("importDB"));

        verticalLayout->addWidget(importDB);

        exportDB = new QPushButton(centralwidget);
        exportDB->setObjectName(QString::fromUtf8("exportDB"));

        verticalLayout->addWidget(exportDB);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 347, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        openMaterialDialog->setText(QApplication::translate("MainWindow", "Open Material Dialog", nullptr));
        resetDB->setText(QApplication::translate("MainWindow", "Reset DB", nullptr));
        importDB->setText(QApplication::translate("MainWindow", "Import DB", nullptr));
        exportDB->setText(QApplication::translate("MainWindow", "Export DB", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
