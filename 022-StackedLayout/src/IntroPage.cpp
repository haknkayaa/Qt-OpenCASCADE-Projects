/******************************************************************************
 *    IntroPage.cpp 
 ******************************************************************************
 *     author      : Hakan Kaya, mail@hakankaya.kim
 *     date        : 2/24/21.
 *     description :
 ******************************************************************************/

#include "IntroPage.h"

IntroPage::IntroPage(QWidget *parent) : QWidget(parent){

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);

    // LEFT SIDE CONTENT
    {
        QWidget *leftContentWidget = new QWidget(this);

        leftContentWidget->setStyleSheet("background-color:#0e1822;");

        QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
        spLeft.setHorizontalStretch(1);
        leftContentWidget->setSizePolicy(spLeft);

        QVBoxLayout *leftMenuLayout = new QVBoxLayout();
        leftMenuLayout->setAlignment(Qt::AlignHCenter);

        /** New Project Button Start */
        QToolButton *newProjectButton = new QToolButton(this);
        newProjectButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        newProjectButton->setStyleSheet("font-size: 20pt; color: #CABFEE;"); // Color is very light purple
        QPixmap newProjectPixmap(":/icons/new-project-icon.png");
        QIcon newProjectIcon(newProjectPixmap);
        newProjectButton->setIconSize(QSize(50, 50));
        newProjectButton->setIcon(newProjectIcon);
        newProjectButton->setText("New Project\n"); // Added a new line to keep close the text to icon
        newProjectButton->setFixedSize(200, 150);
        leftMenuLayout->addWidget(newProjectButton);
        /** New Project Button end */

        /** Open Project Button Start */
        QToolButton *openProjectButton = new QToolButton(this);
        openProjectButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        openProjectButton->setStyleSheet("font-size: 20pt; color: #CABFEE;");
        QPixmap openProjectPixmap(":/icons/open-project-icon.png");
        QIcon openProjectIcon(openProjectPixmap);
        openProjectButton->setIconSize(QSize(50, 50));
        openProjectButton->setIcon(openProjectIcon);
        openProjectButton->setText("Open Project\n");
        openProjectButton->setFixedSize(200, 150);
        leftMenuLayout->addWidget(openProjectButton);
        /** Open Project Button end */

        /** Settings Button Start */
        QToolButton *settingsButton = new QToolButton(this);
        settingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        settingsButton->setStyleSheet("font-size: 20pt; color: #CABFEE;");
        QPixmap settingsPixmap(":/icons/settings-icon.png");
        QIcon settingsIcon(settingsPixmap);
        settingsButton->setIconSize(QSize(50, 50));
        settingsButton->setIcon(settingsIcon);
        settingsButton->setText("Settings\n");
        settingsButton->setFixedSize(200, 150);
        leftMenuLayout->addWidget(settingsButton);
        /** Settings Button end */

        /** Examples Button Start */
        QToolButton *examplesButton = new QToolButton(this);
        examplesButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        examplesButton->setStyleSheet("font-size: 20pt; color: #CABFEE;");
        QPixmap examplesPixmap(":/icons/examples-icon.png");
        QIcon examplesIcon(examplesPixmap);
        examplesButton->setIconSize(QSize(50, 50));
        examplesButton->setIcon(examplesIcon);
        examplesButton->setText("Examples\n");
        examplesButton->setFixedSize(200, 150);
        leftMenuLayout->addWidget(examplesButton);
        /** Examples Button End */

        leftContentWidget->setLayout(leftMenuLayout);
        mainLayout->addWidget(leftContentWidget);

        /** Made connections of left menu buttons to respective slots */
        connect(newProjectButton, SIGNAL(clicked(bool)), this, SLOT(newProject_button()));
        connect(openProjectButton, SIGNAL(clicked(bool)), this, SLOT(openProject_button()));
        connect(settingsButton, SIGNAL(clicked(bool)), this, SLOT(settings_button()));
        connect(examplesButton, SIGNAL(clicked(bool)), this, SLOT(examples_button()));

    }
    // END LEFT SIDE CONTENT

    // RIGHT SIDE CONTENT
    {
        rightContentWidget = new QWidget(this);
        QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
        spRight.setHorizontalStretch(5);
        rightContentWidget->setSizePolicy(spRight);
        rightContentWidget->setMaximumHeight(600);

        QVBoxLayout *rightSideLayout = new QVBoxLayout(rightContentWidget);


        // Right Side Content Title
        {
            QLabel *titleLabel = new QLabel("Welcome, <br>"
                                            "<h1 style='margin-top:0px; margin-bottom:0px;'>MRADSIM</h1> <br>"
                                            "<h2 style='margin-top:0px; margin-bottom:0px;'>Matter Radiation Inreractions Simulator</h2>",
                                            this);

            rightSideLayout->addWidget(titleLabel);
        }
        // End Right Side Content Title



        rightSideLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum,QSizePolicy::Expanding));

        mainLayout->addWidget(rightContentWidget);
    }
    // END RIGHT SIDE CONTENT

    setLayout(mainLayout);
}
void IntroPage::newProject_button() {
    qDebug() << "New Project clicked";

}

void IntroPage::openProject_button() {
    qDebug() << "Open Project Clicked";
}

void IntroPage::settings_button() {
    qDebug() << "Settings clicked";

}

void IntroPage::examples_button() {
    qDebug() << "Examples clicked";
}