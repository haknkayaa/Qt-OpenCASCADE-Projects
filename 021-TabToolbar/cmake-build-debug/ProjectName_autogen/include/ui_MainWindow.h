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
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSaveAs;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionScale;
    QAction *actionPolypaint;
    QAction *actionSettings;
    QAction *actionDummy;
    QAction *actionRedo;
    QAction *actionUndo;
    QAction *actionClose;
    QAction *actionHelp;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(810, 511);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/stupid_logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/48px-Document-save-as.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveAs->setIcon(icon1);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/48px-Document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon2);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/48px-Document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon3);
        actionScale = new QAction(MainWindow);
        actionScale->setObjectName(QString::fromUtf8("actionScale"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/Scale.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionScale->setIcon(icon4);
        actionPolypaint = new QAction(MainWindow);
        actionPolypaint->setObjectName(QString::fromUtf8("actionPolypaint"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/icons/48px-Polypaint.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPolypaint->setIcon(icon5);
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/icons/48px-Preferences-system.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon6);
        actionDummy = new QAction(MainWindow);
        actionDummy->setObjectName(QString::fromUtf8("actionDummy"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/icons/48px-Edit-redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon7);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionUndo->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/icons/48px-Edit-undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon8);
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/icons/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon9);
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/icons/Help-browser.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHelp->setIcon(icon10);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMaximumSize(QSize(99999, 99999));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "TabToolbar Test", nullptr));
        actionSaveAs->setText(QApplication::translate("MainWindow", "Save As", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", nullptr));
        actionSave->setText(QApplication::translate("MainWindow", "Save", nullptr));
        actionScale->setText(QApplication::translate("MainWindow", "Scale", nullptr));
        actionPolypaint->setText(QApplication::translate("MainWindow", "Polypaint", nullptr));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", nullptr));
        actionDummy->setText(QApplication::translate("MainWindow", "dummy", nullptr));
        actionRedo->setText(QApplication::translate("MainWindow", "redo", nullptr));
        actionUndo->setText(QApplication::translate("MainWindow", "undo", nullptr));
        actionClose->setText(QApplication::translate("MainWindow", "close", nullptr));
        actionHelp->setText(QApplication::translate("MainWindow", "Help", nullptr));
#ifndef QT_NO_STATUSTIP
        statusBar->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
