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

#include "MainWindow.h"

MainWindow::MainWindow() : QMainWindow()
{
   _proc = new Processor();
   setCentralWidget( _proc );

   _fileMenu = new QMenu( "&File" );
   _fileMenu->addAction( "&Open...", this, SLOT(openFile()) );
   _fileMenu->addAction( "E&xit", this, SLOT(close()) );
   menuBar()->addMenu( _fileMenu );

   QDockWidget* _cacheDock = new QDockWidget( "Cache" );
   _cacheDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   _cacheView = new QTableView;
   _cacheView->setModel( _proc->getCache() );
   _cacheView->setGridStyle( Qt::DashLine );
   _cacheView->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
   _cacheView->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _cacheDock->setWidget( _cacheView );
   addDockWidget( Qt::RightDockWidgetArea, _cacheDock );

   QDockWidget* _memDock = new QDockWidget( "Main Memory" );
   _memDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   _memoryView = new QTableView;
   _memoryView->setModel( _proc->getMemory() );
   _memoryView->setGridStyle( Qt::DashLine );
   _memoryView->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _memoryView->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _memDock->setWidget( _memoryView );
   addDockWidget( Qt::RightDockWidgetArea, _memDock );
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
