/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

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

MainWindow::MainWindow() : QMainWindow()
{
   // Initialize main view widget
   _proc = new Processor();
   setCentralWidget( _proc );

   // Add menus
   _fileMenu = new QMenu( "&File" );
   _fileMenu->addAction( "&Open...", this, SLOT(openFile()) );
   _fileMenu->addAction( "E&xit", this, SLOT(close()) );
   menuBar()->addMenu( _fileMenu );

   // Dock the Cache widget
   QDockWidget* _cacheDock = new QDockWidget( "Cache" );
   _cacheDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   _cacheView = new QTableView;
   _cacheView->setModel( _proc->getCache() );
   _cacheView->setGridStyle( Qt::DashLine );
   _cacheView->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
   _cacheView->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _cacheDock->setWidget( _cacheView );
   addDockWidget( Qt::RightDockWidgetArea, _cacheDock );

   // Dock the Memory widget
   QDockWidget* _memDock = new QDockWidget( "Main Memory" );
   _memDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   _memoryView = new QTableView;
   _memoryView->setModel( _proc->getMemory() );
   _memoryView->setGridStyle( Qt::DashLine );
   _memoryView->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _memoryView->horizontalHeader()->setVisible( false );
   _memoryView->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _memDock->setWidget( _memoryView );
   addDockWidget( Qt::RightDockWidgetArea, _memDock );

   // Open file from command line if applicable
   QStringList args = qApp->arguments();
   if( args.size() > 1 )
      _proc->readFile( args.at(1) );
}

MainWindow::~MainWindow()
{
}

void MainWindow::openFile()
{
   QString filename = QFileDialog::getOpenFileName( this, "Open Program Source",
                                                    QDir::currentPath(),
                                                    "All Files (*.*)" );
   if( filename == "" )
      return;

   _proc->readFile( filename );
}
