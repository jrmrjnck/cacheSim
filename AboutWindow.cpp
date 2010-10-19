/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include <QtGui/QMovie>
#include <QtGui/QGridLayout>
#include <QtGui/QFont>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtCore/QTimer>

#include "AboutWindow.h"

const char HELP_TEXT[] = \
"This program simulates the operation of a direct mapped CPU \
cache for a particularly simple computer architecture.";

// Constructor
AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent)
{
   QGridLayout* layout = new QGridLayout( this );

   // Make the graphics view fixed size
   QGraphicsView* graphicsView = new QGraphicsView;
   graphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
   graphicsView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
   graphicsView->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

   // Make the version text bold
   QLabel* appVersion = new QLabel( "Cache Simulator, Version 0.0.1" );
   QFont font;
   font.setPointSize( 10 );
   font.setBold( true );
   font.setWeight( 75 );
   appVersion->setFont( font );

   QLabel* descrip = new QLabel( HELP_TEXT );
   descrip->setWordWrap( true );

   QSpacerItem* vSpace = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );

   // Add widgets to the layout
   layout->addWidget( graphicsView, 0, 0, 2, 1 );
   layout->addWidget( appVersion, 0, 2, 1, 1 );
   layout->addWidget( descrip, 1, 2, 1, 1 );
   layout->addItem( vSpace, 1, 1, 1, 1 );

   // Position a picture of Dr. Plassmann's head over his shoulders
   _plassHead = new QGraphicsPixmapItem;
   _plassHead->setPixmap( QPixmap("PlassHead.png") );
   _plassHead->setTransformOriginPoint( _plassHead->pixmap().width()/2,
                                        _plassHead->pixmap().height()/2 );
   _plassHead->setPos( 30, 8 );
   _angle = 0;

   // Tell the view which scene to show
   QGraphicsScene* scene = new QGraphicsScene;
   scene->addPixmap( QPixmap("PlassGround.png") );
   scene->addItem( _plassHead );
   graphicsView->setScene( scene );

   // Set a timer to rotate Dr. Plassmann's head every 20 ms
   _rotateTimer = new QTimer( this );
   connect( _rotateTimer, SIGNAL(timeout()), this, SLOT(rotateHead()) );
   _angle = 720;
   _rotateTimer->start( 20 );

   // Do not allow resizing
   setFixedSize( this->sizeHint() );
}

// Rotate the picture on a timer
void AboutWindow::rotateHead()
{
   _plassHead->setRotation( _angle );

   static bool direction = false;

   // Stop after two revolutions
   if( _angle == 720 || _angle == -720 ) {
      _angle = 0;
      direction = !direction;
      _rotateTimer->stop();

      // Wait for a while before starting again
      QTimer::singleShot( 4000, _rotateTimer, SLOT(start()) );
   }
   // Rotate Dr. Plassmann's head in a manner inconsistent with
   // realistic anatomical range of motion.
   else
      _angle += direction ? 30 : -30;
}

// Catch the close event to emit a signal
void AboutWindow::closeEvent( QCloseEvent* e )
{
   e->accept();
   emit dialogClosed();
}
