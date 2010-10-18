/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include <QtGui/QMessageBox>
#include <QtGui/QTableView>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QFileDialog>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QApplication>

#include "MainWindow.h"
#include "Exceptions.h"

// Constructor
MainWindow::MainWindow() : QMainWindow()
{
   // Initialize main view widget
   _processor = new Processor();
   setCentralWidget( _processor );

   _aboutWindow = NULL; 

   // Add menus
   QMenu* fileMenu = new QMenu( "&File" );
   fileMenu->addAction( "&Open...", this, SLOT(openFile()), QKeySequence::Open );
   fileMenu->addAction( "E&xit", this, SLOT(close()) );
   menuBar()->addMenu( fileMenu );

   QMenu* helpMenu = new QMenu( "&Help" );
   helpMenu->addAction( "&About", this, SLOT(showAbout()) );
   menuBar()->addMenu( helpMenu );

   // Dock the Cache widget
   QDockWidget* _cacheDock = new QDockWidget( "Cache" );
   _cacheDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   _cacheView = new QTableView;
   _cacheView->setModel( _processor->cache() );
   _cacheView->setGridStyle( Qt::DashLine );
   _cacheView->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _cacheView->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _cacheDock->setWidget( _cacheView );
   addDockWidget( Qt::RightDockWidgetArea, _cacheDock );

   // Dock the Memory widget
   QDockWidget* _memDock = new QDockWidget( "Main Memory" );
   _memDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   _memoryView = new QTableView;
   _memoryView->setModel( _processor->memory() );
   _memoryView->setGridStyle( Qt::DashLine );
   _memoryView->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
   _memoryView->horizontalHeader()->setVisible( false );
   _memoryView->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _memDock->setWidget( _memoryView );
   addDockWidget( Qt::RightDockWidgetArea, _memDock );

   // Open file from command line if applicable
   QStringList args = qApp->arguments();
   if( args.size() > 1 ) {
      _processor->readFile( args.at(1) );
   }
}

// Destructor
MainWindow::~MainWindow()
{
   delete _processor;
   delete _aboutWindow;
}

// Slot called to show an open file dialog
void MainWindow::openFile()
{
   QString filename = QFileDialog::getOpenFileName( this, "Open Program Source",
                                                    QDir::currentPath(),
                                                    "All Files (*.*)" );
   if( filename == "" )
      return;

   _processor->readFile( filename );
}

// Show the about dialog
void MainWindow::showAbout()
{
   if( _aboutWindow != NULL )
      return;

   _aboutWindow = new AboutWindow( this );
   _aboutWindow->show();
   connect( _aboutWindow, SIGNAL(dialogClosed()), this, SLOT(closeAbout()) );
}

// Close and delete the about window
void MainWindow::closeAbout()
{
   _aboutWindow->hide();
   delete _aboutWindow;
   _aboutWindow = NULL;
}
