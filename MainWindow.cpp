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
#include <QtGui/QToolBar>

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
   QMenu* fileMenu  = menuBar()->addMenu( "&File" );
   QMenu* runMenu   = menuBar()->addMenu( "&Run" );
   QMenu* cacheMenu = menuBar()->addMenu( "&Cache" );
   QMenu* helpMenu  = menuBar()->addMenu( "&Help" );

   // File Menu
   _loadAction   = fileMenu->addAction( QIcon("Folder.png"), "&Load...", 
                                        this, SLOT(loadFile()), QKeySequence("Ctrl+L") );
   _reloadAction = fileMenu->addAction( QIcon("Refresh.png"), "&Reload",
                                        this, SLOT(reloadFile()), QKeySequence("Ctrl+A") );
   fileMenu->addAction( "&Exit", this, SLOT(close()), QKeySequence("Ctrl+Q") );

   // Run Menu
   _runAction  = runMenu->addAction( QIcon("Play.png"), "Ru&n", 
                                     _processor, SLOT(run()), QKeySequence("Ctrl+R") );
   _stepAction = runMenu->addAction( QIcon("Next.png"), "&Step",
                                     _processor, SLOT(step()), QKeySequence("Ctrl+S") );
   connect( _processor, SIGNAL(enableGui(bool)), this, SLOT(enableRunGui(bool)) );
   enableRunGui( false );

   // Cache menu
   cacheMenu->addAction( "Block Size", this, SLOT(changeBlockSize()), QKeySequence("Ctrl+B") );
   cacheMenu->addAction( "Cache Size", this, SLOT(changeCacheSize()), QKeySequence("Ctrl+C") );

   // Help Menu
   helpMenu->addAction( "About", this, SLOT(showAbout()) );

   // Toolbar
   QToolBar* iconBar = addToolBar( "Icon Bar" );
   iconBar->addAction( _loadAction );
   iconBar->addAction( _reloadAction );
   iconBar->addSeparator();
   iconBar->addAction( _runAction );
   iconBar->addAction( _stepAction );

   // Dock the Cache widget
   QDockWidget* _cacheDock = new QDockWidget( "Cache" );
   _cacheDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   QTableView* _cacheView = new QTableView;
   _cacheView->setModel( _processor->cache() );
   _cacheView->setGridStyle( Qt::DashLine );
   _cacheView->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _cacheView->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _cacheDock->setWidget( _cacheView );
   addDockWidget( Qt::RightDockWidgetArea, _cacheDock );

   // Dock the Memory widget
   QDockWidget* _memDock = new QDockWidget( "Main Memory" );
   _memDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   QTableView* _memoryView = new QTableView;
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
void MainWindow::loadFile()
{
   _programFileName = QFileDialog::getOpenFileName( this, "Open Program Source",
                                                    QDir::currentPath(),
                                                    "All Files (*.*)" );
   if( _programFileName.isEmpty() )
      return;

   _processor->readFile( _programFileName );
}

// Reload the last used file
void MainWindow::reloadFile()
{
   if( _programFileName.isEmpty() )
      return;

   _processor->readFile( _programFileName );
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

// Show a dialog to configure the block size
void MainWindow::changeBlockSize()
{

}

// Show a dialog to configure the cache parameters
void MainWindow::changeCacheSize()
{

}

// Make the menu/toolbar options reflect the processor state
void MainWindow::enableRunGui( bool enabled )
{
   _runAction->setEnabled( enabled );
   _stepAction->setEnabled( enabled );
}
