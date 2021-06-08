/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *viewerlayout;
    QWidget *widget;
    QWidget *layoutWidget;
    QVBoxLayout *ButtonLayout;
    QPushButton *pushButton;
    QCheckBox *check_show3DGrid;
    QCheckBox *check_showTrihedronCube;
    QCheckBox *check_showPerformanceStats;
    QSpacerItem *ButtonSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_5;
    QLabel *label_xPos;
    QLabel *label;
    QLabel *label_yPos;
    QLabel *label_3;
    QLabel *label_zPos;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1038, 795);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setCursor(QCursor(Qt::ArrowCursor));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        viewerlayout = new QVBoxLayout();
        viewerlayout->setObjectName(QString::fromUtf8("viewerlayout"));

        horizontalLayout_2->addLayout(viewerlayout);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMaximumSize(QSize(502, 16777215));
        layoutWidget = new QWidget(widget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 191, 731));
        ButtonLayout = new QVBoxLayout(layoutWidget);
        ButtonLayout->setObjectName(QString::fromUtf8("ButtonLayout"));
        ButtonLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setAutoRepeat(false);

        ButtonLayout->addWidget(pushButton);

        check_show3DGrid = new QCheckBox(layoutWidget);
        check_show3DGrid->setObjectName(QString::fromUtf8("check_show3DGrid"));

        ButtonLayout->addWidget(check_show3DGrid);

        check_showTrihedronCube = new QCheckBox(layoutWidget);
        check_showTrihedronCube->setObjectName(QString::fromUtf8("check_showTrihedronCube"));

        ButtonLayout->addWidget(check_showTrihedronCube);

        check_showPerformanceStats = new QCheckBox(layoutWidget);
        check_showPerformanceStats->setObjectName(QString::fromUtf8("check_showPerformanceStats"));

        ButtonLayout->addWidget(check_showPerformanceStats);

        ButtonSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        ButtonLayout->addItem(ButtonSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        label_xPos = new QLabel(layoutWidget);
        label_xPos->setObjectName(QString::fromUtf8("label_xPos"));

        horizontalLayout_3->addWidget(label_xPos);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        label_yPos = new QLabel(layoutWidget);
        label_yPos->setObjectName(QString::fromUtf8("label_yPos"));

        horizontalLayout_3->addWidget(label_yPos);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        label_zPos = new QLabel(layoutWidget);
        label_zPos->setObjectName(QString::fromUtf8("label_zPos"));

        horizontalLayout_3->addWidget(label_zPos);


        ButtonLayout->addLayout(horizontalLayout_3);


        horizontalLayout_2->addWidget(widget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1038, 21));
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
        pushButton->setText(QApplication::translate("MainWindow", "Push Button", nullptr));
        check_show3DGrid->setText(QApplication::translate("MainWindow", "Show 3D Grid", nullptr));
        check_showTrihedronCube->setText(QApplication::translate("MainWindow", "Show Trihedron Cube", nullptr));
        check_showPerformanceStats->setText(QApplication::translate("MainWindow", "Show Performance Stats", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "X:", nullptr));
        label_xPos->setText(QString());
        label->setText(QApplication::translate("MainWindow", "Y:", nullptr));
        label_yPos->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Z:", nullptr));
        label_zPos->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
