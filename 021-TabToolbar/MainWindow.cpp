/*
    TabToolbar - a small utility library for Qt, providing tabbed toolbars
	Copyright (C) 2018 Oleksii Sierov
	
    TabToolbar is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TabToolbar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TabToolbar.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MainWindow.h"
#include "ui_MainWindow.h"

CMainWindow::CMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->statusBar->addPermanentWidget(new QLabel("Some fancy status."));

    tt::TabToolbar* mytab = new tt::TabToolbar(this, 75, 3);
    addToolBar(Qt::TopToolBarArea, mytab);

    QMenu* menu = new QMenu(this);
    menu->setObjectName("dummyMenu");
    menu->addActions({ui->actionDummy});

    mytab->AddPage("File");
    static bool kek = true;
    tt::Page* editPage = mytab->AddPage("Edit");

    tt::Page* viewPage = mytab->AddPage("View");
    tt::Group* g1 = viewPage->AddGroup("Group 1");
    tt::Group* g2 = viewPage->AddGroup("Group 2");
    tt::Group* g3 = viewPage->AddGroup("Group 3");
    g1->AddAction(QToolButton::DelayedPopup, ui->actionOpen);
    g1->AddSeparator();
    g1->AddAction(QToolButton::DelayedPopup, ui->actionSave);
    g1->AddAction(QToolButton::DelayedPopup, ui->actionSaveAs);
    g2->AddAction(QToolButton::InstantPopup, ui->actionPolypaint, menu);
    g2->AddAction(QToolButton::InstantPopup, ui->actionScale, menu);
    g2->AddSeparator();
    QTextEdit* te = new QTextEdit();
    g2->AddWidget(te);
    te->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    te->setMaximumWidth(100);
    tt::SubGroup* g2s = g2->AddSubGroup(tt::SubGroup::Align::Yes);
    g2s->AddAction(QToolButton::DelayedPopup, ui->actionUndo);
    g2s->AddAction(QToolButton::DelayedPopup, ui->actionRedo);
    g2s->AddAction(QToolButton::InstantPopup, ui->actionClose, menu);
    g3->AddAction(QToolButton::MenuButtonPopup, ui->actionSettings, menu);
    tt::SubGroup* g3s = g3->AddSubGroup(tt::SubGroup::Align::Yes);
    g3s->AddHorizontalButtons({{QToolButton::DelayedPopup, ui->actionSave},
                               {QToolButton::InstantPopup, ui->actionPolypaint, menu},
                               {QToolButton::MenuButtonPopup, ui->actionSettings, menu}});

    g3s->AddHorizontalButtons({{QToolButton::DelayedPopup, ui->actionUndo},
                               {QToolButton::DelayedPopup, ui->actionRedo},
                               {QToolButton::InstantPopup, ui->actionClose, menu}});
    QCheckBox* ch = new QCheckBox("Check 1");
    g3s->AddWidget(ch);

    g3->AddSeparator();
    tt::SubGroup* g3ss = g3->AddSubGroup(tt::SubGroup::Align::No);
    QPushButton* btn = new QPushButton("Edit");
    g3ss->AddWidget(btn);
    g3ss->AddAction(QToolButton::DelayedPopup, ui->actionSaveAs);
    QObject::connect(btn, &QPushButton::clicked, [editPage]()
    {
        if(kek)
            editPage->hide();
        else
            editPage->show();
        kek = !kek;
    });

    mytab->AddPage("Help");

    mytab->SetSpecialTabEnabled(true);
    mytab->AddCornerAction(ui->actionHelp);
    QObject::connect(mytab, &tt::TabToolbar::SpecialTabClicked, this, [this]()
    {
        QMessageBox::information(this, "Kek", "Cheburek");
    });

}

CMainWindow::~CMainWindow()
{
    delete ui;
}
