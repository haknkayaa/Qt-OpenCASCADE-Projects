//
// Created by sufuk on 22.02.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ViewerBox.h" resolved

#include "ViewerBox.h"
#include "ui_ViewerBox.h"


ViewerBox::ViewerBox(QWidget *parent) :
        QWidget(parent), ui(new Ui::ViewerBox) {
    ui->setupUi(this);
}

ViewerBox::~ViewerBox() {
    delete ui;
}

