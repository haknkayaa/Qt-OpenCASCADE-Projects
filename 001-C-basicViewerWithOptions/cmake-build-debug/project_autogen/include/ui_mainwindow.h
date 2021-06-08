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
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *ButtonLayout;
    QPushButton *pushButton;
    QCheckBox *check_show3DGrid;
    QCheckBox *check_showTrihedronCube;
    QCheckBox *check_showPerformanceStats;
    QSpacerItem *ButtonSpacer;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *xpos_button;
    QPushButton *xneg_button;
    QPushButton *ypos_button;
    QPushButton *yneg_button;
    QPushButton *zpos_button;
    QPushButton *zneg_button;
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
        MainWindow->resize(1020, 724);
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
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        ButtonLayout = new QVBoxLayout();
        ButtonLayout->setSpacing(0);
        ButtonLayout->setObjectName(QString::fromUtf8("ButtonLayout"));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setAutoRepeat(false);

        ButtonLayout->addWidget(pushButton);

        check_show3DGrid = new QCheckBox(widget);
        check_show3DGrid->setObjectName(QString::fromUtf8("check_show3DGrid"));

        ButtonLayout->addWidget(check_show3DGrid);

        check_showTrihedronCube = new QCheckBox(widget);
        check_showTrihedronCube->setObjectName(QString::fromUtf8("check_showTrihedronCube"));

        ButtonLayout->addWidget(check_showTrihedronCube);

        check_showPerformanceStats = new QCheckBox(widget);
        check_showPerformanceStats->setObjectName(QString::fromUtf8("check_showPerformanceStats"));

        ButtonLayout->addWidget(check_showPerformanceStats);

        ButtonSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        ButtonLayout->addItem(ButtonSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        xpos_button = new QPushButton(widget);
        xpos_button->setObjectName(QString::fromUtf8("xpos_button"));

        horizontalLayout_4->addWidget(xpos_button);

        xneg_button = new QPushButton(widget);
        xneg_button->setObjectName(QString::fromUtf8("xneg_button"));

        horizontalLayout_4->addWidget(xneg_button);

        ypos_button = new QPushButton(widget);
        ypos_button->setObjectName(QString::fromUtf8("ypos_button"));

        horizontalLayout_4->addWidget(ypos_button);

        yneg_button = new QPushButton(widget);
        yneg_button->setObjectName(QString::fromUtf8("yneg_button"));

        horizontalLayout_4->addWidget(yneg_button);

        zpos_button = new QPushButton(widget);
        zpos_button->setObjectName(QString::fromUtf8("zpos_button"));

        horizontalLayout_4->addWidget(zpos_button);

        zneg_button = new QPushButton(widget);
        zneg_button->setObjectName(QString::fromUtf8("zneg_button"));

        horizontalLayout_4->addWidget(zneg_button);


        ButtonLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        label_xPos = new QLabel(widget);
        label_xPos->setObjectName(QString::fromUtf8("label_xPos"));

        horizontalLayout_3->addWidget(label_xPos);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        label_yPos = new QLabel(widget);
        label_yPos->setObjectName(QString::fromUtf8("label_yPos"));

        horizontalLayout_3->addWidget(label_yPos);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        label_zPos = new QLabel(widget);
        label_zPos->setObjectName(QString::fromUtf8("label_zPos"));

        horizontalLayout_3->addWidget(label_zPos);


        ButtonLayout->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(ButtonLayout);


        horizontalLayout_2->addWidget(widget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1020, 21));
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
        pushButton->setText(QApplication::translate("MainWindow", "Import STEP", nullptr));
        check_show3DGrid->setText(QApplication::translate("MainWindow", "Show 3D Grid", nullptr));
        check_showTrihedronCube->setText(QApplication::translate("MainWindow", "Show Trihedron Cube", nullptr));
        check_showPerformanceStats->setText(QApplication::translate("MainWindow", "Show Performance Stats", nullptr));
        xpos_button->setText(QApplication::translate("MainWindow", "X+", nullptr));
        xneg_button->setText(QApplication::translate("MainWindow", "X-", nullptr));
        ypos_button->setText(QApplication::translate("MainWindow", "Y+", nullptr));
        yneg_button->setText(QApplication::translate("MainWindow", "Y-", nullptr));
        zpos_button->setText(QApplication::translate("MainWindow", "Z+", nullptr));
        zneg_button->setText(QApplication::translate("MainWindow", "Z-", nullptr));
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
