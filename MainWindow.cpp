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
   setWindowIcon( QIcon("Chip.png") );
   setWindowTitle( "CacheSim" );

   // Add menus
   QMenuBar* menus = menuBar();
   QMenu* fileMenu  = menus->addMenu( "&File" );
   QMenu* runMenu   = menus->addMenu( "&Run" );
   QMenu* cacheMenu = menus->addMenu( "&Cache" );
   QMenu* helpMenu  = menus->addMenu( "&Help" );

   // File Menu
   _loadAction   = fileMenu->addAction( QIcon("Folder.png"), "&Load...", 
                                        this, SLOT(loadFile()), QKeySequence("Ctrl+L") );
   _reloadAction = fileMenu->addAction( QIcon("Refresh.png"), "&Reload",
                                        this, SLOT(reloadFile()), QKeySequence("Ctrl+A") );
   _reloadAction->setEnabled( false );
   fileMenu->addSeparator();
   fileMenu->addAction( "&Exit", this, SLOT(close()), QKeySequence("Ctrl+Q") );

   // Run Menu
   _runAction  = runMenu->addAction( QIcon("Play.png"), "Ru&n", 
                                     _processor, SLOT(run()), QKeySequence("Ctrl+R") );
   _stepAction = runMenu->addAction( QIcon("Next.png"), "&Step",
                                     _processor, SLOT(step()), QKeySequence("Ctrl+S") );
   connect( _processor, SIGNAL(enableGui(bool)), this, SLOT(enableRunGui(bool)) );
   enableRunGui( false );

   // Cache menu
   cacheMenu->addAction( "Block Size", _processor, SLOT(changeBlockSize()),
                         QKeySequence("Ctrl+B") );
   cacheMenu->addAction( "Cache Size", _processor, SLOT(changeCacheSize()),
                         QKeySequence("Ctrl+C") );

   // Help Menu
   helpMenu->addAction( "About", this, SLOT(showAbout()) );

   // Toolbar
   QToolBar* iconBar = addToolBar( "Icon Bar" );
   iconBar->setMovable( false );
   iconBar->addAction( _loadAction );
   iconBar->addAction( _reloadAction );
   iconBar->addSeparator();
   iconBar->addAction( _runAction );
   iconBar->addAction( _stepAction );

   // Status Bar
   statusBar();
   _loadAction->setStatusTip( "Load a new program file." );
   _reloadAction->setStatusTip( "Reset the program to its beginning." );
   _runAction->setStatusTip( "Run to program to its end" );
   _stepAction->setStatusTip( "Execute the next instruction" );

   // Dock the Cache widget
   QDockWidget* cacheDock = new QDockWidget( "Cache" );
   cacheDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   cacheDock->setFeatures( QDockWidget::DockWidgetMovable | 
                            QDockWidget::DockWidgetFloatable );
   _cacheView = new QTableView;
   _cacheView->setModel( _processor->cache() );
   _cacheView->setGridStyle( Qt::DashLine );
   _cacheView->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   _cacheView->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   cacheDock->setWidget( _cacheView );
   addDockWidget( Qt::RightDockWidgetArea, cacheDock );
   connect( _processor, SIGNAL(cacheChanged()), this, SLOT(changeCache()) );

   // Dock the Memory widget
   QDockWidget* memDock = new QDockWidget( "Main Memory" );
   memDock->setAllowedAreas( Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea );
   memDock->setFeatures( QDockWidget::DockWidgetMovable | 
                            QDockWidget::DockWidgetFloatable );
   QTableView* memoryView = new QTableView;
   memoryView->setModel( _processor->memory() );
   memoryView->setGridStyle( Qt::DashLine );
   memoryView->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
   memoryView->horizontalHeader()->setVisible( false );
   memoryView->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
   memDock->setWidget( memoryView );
   addDockWidget( Qt::RightDockWidgetArea, memDock );

   // Open file from command line if applicable
   QStringList args = qApp->arguments();
   if( args.size() > 1 ) {
      _programFileName = args.at( 1 );
      reloadFile();
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
   reloadFile();
}

// Reload the last used file
void MainWindow::reloadFile()
{
   if( _programFileName.isEmpty() )
      return;

   _processor->readFile( _programFileName );
   _reloadAction->setEnabled( true );
}


// Show the about dialog
void MainWindow::showAbout()
{
   if( _aboutWindow == NULL ) {
      _aboutWindow = new AboutWindow( this );
      connect( _aboutWindow, SIGNAL(dialogClosed()), this, SLOT(closeAbout()) );
   }

   _aboutWindow->show();
}

// Close and delete the about window
void MainWindow::closeAbout()
{
   delete _aboutWindow;
   _aboutWindow = NULL;
}

// Make the menu/toolbar options reflect the processor state
void MainWindow::enableRunGui( bool enabled )
{
   _runAction->setEnabled( enabled );
   _stepAction->setEnabled( enabled );
}

// Reset the model for the cache view
void MainWindow::changeCache()
{
   _cacheView->setModel( _processor->cache() );
}
