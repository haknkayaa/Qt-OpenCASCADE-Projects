// Qt Libraries
#include <QApplication>
// User Libraries
#include "plot.h"
#include <QMainWindow>
#include <qapplication.h>
#include <qmainwindow.h>
#include <qcombobox.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include "plot.h"


class MainWindow: public QMainWindow
{
public:
    MainWindow( QWidget * = NULL );

private:
    Plot *d_plot;
};

MainWindow::MainWindow( QWidget *parent ):
        QMainWindow( parent )
{
    d_plot = new Plot( this );
    setCentralWidget( d_plot );

    QToolBar *toolBar = new QToolBar( this );

    QComboBox *typeBox = new QComboBox( toolBar );
    typeBox->addItem( "Linear" );
    typeBox->addItem( "Logarithmic" );
    typeBox->setCurrentIndex( 0 );
    typeBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    toolBar->addWidget( typeBox );
    addToolBar( toolBar );

    d_plot->setMode( typeBox->currentIndex() );
    connect( typeBox, SIGNAL( currentIndexChanged( int ) ),
             d_plot, SLOT( setMode( int ) ) );
}

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    MainWindow w;
    w.setObjectName( "MainWindow" );
    w.resize( 600, 400 );
    w.show();

    return a.exec();
}
