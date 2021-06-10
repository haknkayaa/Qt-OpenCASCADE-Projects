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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QSplitter *splitter;
    QWidget *widget_left;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *viewerlayout;
    QWidget *widget_right;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *ButtonLayout;
    QPushButton *importButton;
    QPushButton *exportButton;
    QCheckBox *check_show3DGrid;
    QCheckBox *check_showTrihedronCube;
    QCheckBox *check_showPerformanceStats;
    QComboBox *comboBox;
    QSpacerItem *ButtonSpacer;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QSlider *slider_ZPlane;
    QToolButton *button_YInvert;
    QSlider *slider_YPlane;
    QSlider *slider_XPlane;
    QCheckBox *check_YPlane;
    QCheckBox *check_ZPlane;
    QToolButton *button_ZInvert;
    QCheckBox *check_XPlane;
    QToolButton *button_XInvert;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *xpos_button;
    QPushButton *xneg_button;
    QPushButton *ypos_button;
    QPushButton *yneg_button;
    QPushButton *zpos_button;
    QPushButton *zneg_button;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
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
        MainWindow->resize(1054, 697);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setCursor(QCursor(Qt::ArrowCursor));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(3, 3, 0, 0);
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        widget_left = new QWidget(splitter);
        widget_left->setObjectName(QString::fromUtf8("widget_left"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_left->sizePolicy().hasHeightForWidth());
        widget_left->setSizePolicy(sizePolicy);
        widget_left->setMinimumSize(QSize(500, 0));
        horizontalLayout = new QHBoxLayout(widget_left);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        viewerlayout = new QVBoxLayout();
        viewerlayout->setObjectName(QString::fromUtf8("viewerlayout"));

        horizontalLayout->addLayout(viewerlayout);

        splitter->addWidget(widget_left);
        widget_right = new QWidget(splitter);
        widget_right->setObjectName(QString::fromUtf8("widget_right"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_right->sizePolicy().hasHeightForWidth());
        widget_right->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(widget_right);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        ButtonLayout = new QVBoxLayout();
        ButtonLayout->setSpacing(6);
        ButtonLayout->setObjectName(QString::fromUtf8("ButtonLayout"));
        ButtonLayout->setSizeConstraint(QLayout::SetNoConstraint);
        importButton = new QPushButton(widget_right);
        importButton->setObjectName(QString::fromUtf8("importButton"));
        importButton->setAutoRepeat(false);

        ButtonLayout->addWidget(importButton);

        exportButton = new QPushButton(widget_right);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));

        ButtonLayout->addWidget(exportButton);

        check_show3DGrid = new QCheckBox(widget_right);
        check_show3DGrid->setObjectName(QString::fromUtf8("check_show3DGrid"));

        ButtonLayout->addWidget(check_show3DGrid);

        check_showTrihedronCube = new QCheckBox(widget_right);
        check_showTrihedronCube->setObjectName(QString::fromUtf8("check_showTrihedronCube"));

        ButtonLayout->addWidget(check_showTrihedronCube);

        check_showPerformanceStats = new QCheckBox(widget_right);
        check_showPerformanceStats->setObjectName(QString::fromUtf8("check_showPerformanceStats"));

        ButtonLayout->addWidget(check_showPerformanceStats);

        comboBox = new QComboBox(widget_right);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        ButtonLayout->addWidget(comboBox);

        ButtonSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        ButtonLayout->addItem(ButtonSpacer);

        groupBox = new QGroupBox(widget_right);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, 1, 1, 1);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        slider_ZPlane = new QSlider(groupBox);
        slider_ZPlane->setObjectName(QString::fromUtf8("slider_ZPlane"));
        slider_ZPlane->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(slider_ZPlane, 2, 3, 1, 1);

        button_YInvert = new QToolButton(groupBox);
        button_YInvert->setObjectName(QString::fromUtf8("button_YInvert"));

        gridLayout->addWidget(button_YInvert, 1, 1, 1, 1);

        slider_YPlane = new QSlider(groupBox);
        slider_YPlane->setObjectName(QString::fromUtf8("slider_YPlane"));
        slider_YPlane->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(slider_YPlane, 1, 3, 1, 1);

        slider_XPlane = new QSlider(groupBox);
        slider_XPlane->setObjectName(QString::fromUtf8("slider_XPlane"));
        slider_XPlane->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(slider_XPlane, 0, 3, 1, 1);

        check_YPlane = new QCheckBox(groupBox);
        check_YPlane->setObjectName(QString::fromUtf8("check_YPlane"));

        gridLayout->addWidget(check_YPlane, 1, 0, 1, 1);

        check_ZPlane = new QCheckBox(groupBox);
        check_ZPlane->setObjectName(QString::fromUtf8("check_ZPlane"));

        gridLayout->addWidget(check_ZPlane, 2, 0, 1, 1);

        button_ZInvert = new QToolButton(groupBox);
        button_ZInvert->setObjectName(QString::fromUtf8("button_ZInvert"));

        gridLayout->addWidget(button_ZInvert, 2, 1, 1, 1);

        check_XPlane = new QCheckBox(groupBox);
        check_XPlane->setObjectName(QString::fromUtf8("check_XPlane"));

        gridLayout->addWidget(check_XPlane, 0, 0, 1, 1);

        button_XInvert = new QToolButton(groupBox);
        button_XInvert->setObjectName(QString::fromUtf8("button_XInvert"));

        gridLayout->addWidget(button_XInvert, 0, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);


        ButtonLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(widget_right);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setSizeConstraint(QLayout::SetNoConstraint);
        xpos_button = new QPushButton(groupBox_2);
        xpos_button->setObjectName(QString::fromUtf8("xpos_button"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(xpos_button->sizePolicy().hasHeightForWidth());
        xpos_button->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(xpos_button);

        xneg_button = new QPushButton(groupBox_2);
        xneg_button->setObjectName(QString::fromUtf8("xneg_button"));
        sizePolicy2.setHeightForWidth(xneg_button->sizePolicy().hasHeightForWidth());
        xneg_button->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(xneg_button);

        ypos_button = new QPushButton(groupBox_2);
        ypos_button->setObjectName(QString::fromUtf8("ypos_button"));
        sizePolicy2.setHeightForWidth(ypos_button->sizePolicy().hasHeightForWidth());
        ypos_button->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(ypos_button);

        yneg_button = new QPushButton(groupBox_2);
        yneg_button->setObjectName(QString::fromUtf8("yneg_button"));
        sizePolicy2.setHeightForWidth(yneg_button->sizePolicy().hasHeightForWidth());
        yneg_button->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(yneg_button);

        zpos_button = new QPushButton(groupBox_2);
        zpos_button->setObjectName(QString::fromUtf8("zpos_button"));
        sizePolicy2.setHeightForWidth(zpos_button->sizePolicy().hasHeightForWidth());
        zpos_button->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(zpos_button);

        zneg_button = new QPushButton(groupBox_2);
        zneg_button->setObjectName(QString::fromUtf8("zneg_button"));
        sizePolicy2.setHeightForWidth(zneg_button->sizePolicy().hasHeightForWidth());
        zneg_button->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(zneg_button);


        verticalLayout_3->addLayout(horizontalLayout_4);


        ButtonLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(widget_right);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetNoConstraint);
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        label_xPos = new QLabel(groupBox_3);
        label_xPos->setObjectName(QString::fromUtf8("label_xPos"));

        horizontalLayout_3->addWidget(label_xPos);

        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(label);

        label_yPos = new QLabel(groupBox_3);
        label_yPos->setObjectName(QString::fromUtf8("label_yPos"));

        horizontalLayout_3->addWidget(label_yPos);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        label_zPos = new QLabel(groupBox_3);
        label_zPos->setObjectName(QString::fromUtf8("label_zPos"));

        horizontalLayout_3->addWidget(label_zPos);


        verticalLayout_4->addLayout(horizontalLayout_3);


        ButtonLayout->addWidget(groupBox_3);


        verticalLayout->addLayout(ButtonLayout);

        splitter->addWidget(widget_right);

        horizontalLayout_2->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1054, 21));
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
        importButton->setText(QApplication::translate("MainWindow", "Import", nullptr));
        exportButton->setText(QApplication::translate("MainWindow", "Export", nullptr));
        check_show3DGrid->setText(QApplication::translate("MainWindow", "Show 3D Grid", nullptr));
        check_showTrihedronCube->setText(QApplication::translate("MainWindow", "Show Trihedron Cube", nullptr));
        check_showPerformanceStats->setText(QApplication::translate("MainWindow", "Show Performance Stats", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "Clip Plane:", nullptr));
        button_YInvert->setText(QApplication::translate("MainWindow", "+/-", nullptr));
        check_YPlane->setText(QApplication::translate("MainWindow", "Y Axis", nullptr));
        check_ZPlane->setText(QApplication::translate("MainWindow", "Z Axis", nullptr));
        button_ZInvert->setText(QApplication::translate("MainWindow", "+/-", nullptr));
        check_XPlane->setText(QApplication::translate("MainWindow", "X Axis", nullptr));
        button_XInvert->setText(QApplication::translate("MainWindow", "+/-", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "View Projection:", nullptr));
        xpos_button->setText(QApplication::translate("MainWindow", "X+", nullptr));
        xneg_button->setText(QApplication::translate("MainWindow", "X-", nullptr));
        ypos_button->setText(QApplication::translate("MainWindow", "Y+", nullptr));
        yneg_button->setText(QApplication::translate("MainWindow", "Y-", nullptr));
        zpos_button->setText(QApplication::translate("MainWindow", "Z+", nullptr));
        zneg_button->setText(QApplication::translate("MainWindow", "Z-", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Position:", nullptr));
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
