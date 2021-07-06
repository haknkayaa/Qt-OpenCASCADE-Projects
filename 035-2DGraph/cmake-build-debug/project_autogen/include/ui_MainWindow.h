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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *addFile;
    QPushButton *removeFile;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout_2;
    QCustomPlot *customPlot;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;
    QPushButton *fitAllButton;
    QPushButton *redrawButton;
    QPushButton *saveImageButton;
    QPushButton *savePDFButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1214, 410);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        addFile = new QPushButton(centralwidget);
        addFile->setObjectName(QString::fromUtf8("addFile"));

        horizontalLayout_2->addWidget(addFile);

        removeFile = new QPushButton(centralwidget);
        removeFile->setObjectName(QString::fromUtf8("removeFile"));

        horizontalLayout_2->addWidget(removeFile);


        verticalLayout->addLayout(horizontalLayout_2);

        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy1);
        listWidget->setMaximumSize(QSize(200, 16777215));

        verticalLayout->addWidget(listWidget);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        customPlot = new QCustomPlot(centralwidget);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));

        verticalLayout_2->addWidget(customPlot);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        zoomInButton = new QPushButton(centralwidget);
        zoomInButton->setObjectName(QString::fromUtf8("zoomInButton"));

        horizontalLayout_3->addWidget(zoomInButton);

        zoomOutButton = new QPushButton(centralwidget);
        zoomOutButton->setObjectName(QString::fromUtf8("zoomOutButton"));

        horizontalLayout_3->addWidget(zoomOutButton);

        fitAllButton = new QPushButton(centralwidget);
        fitAllButton->setObjectName(QString::fromUtf8("fitAllButton"));

        horizontalLayout_3->addWidget(fitAllButton);

        redrawButton = new QPushButton(centralwidget);
        redrawButton->setObjectName(QString::fromUtf8("redrawButton"));

        horizontalLayout_3->addWidget(redrawButton);

        saveImageButton = new QPushButton(centralwidget);
        saveImageButton->setObjectName(QString::fromUtf8("saveImageButton"));

        horizontalLayout_3->addWidget(saveImageButton);

        savePDFButton = new QPushButton(centralwidget);
        savePDFButton->setObjectName(QString::fromUtf8("savePDFButton"));

        horizontalLayout_3->addWidget(savePDFButton);


        verticalLayout_2->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Data Graphs", nullptr));
        addFile->setText(QApplication::translate("MainWindow", "Add File", nullptr));
        removeFile->setText(QApplication::translate("MainWindow", "Remove File", nullptr));
        zoomInButton->setText(QApplication::translate("MainWindow", "Zoom In", nullptr));
        zoomOutButton->setText(QApplication::translate("MainWindow", "Zoom Out", nullptr));
        fitAllButton->setText(QApplication::translate("MainWindow", "Fit All", nullptr));
        redrawButton->setText(QApplication::translate("MainWindow", "Redraw Graph", nullptr));
        saveImageButton->setText(QApplication::translate("MainWindow", "Save Image", nullptr));
        savePDFButton->setText(QApplication::translate("MainWindow", "Save PDF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
