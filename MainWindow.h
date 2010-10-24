/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include <QtGui/QMainWindow>
#include <QtGui/QTableView>

#include "Processor.h"
#include "AboutWindow.h"

// The highest level application object,
// handles menus, actions, etc...
class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   MainWindow();
   ~MainWindow();

public slots:
   void loadFile();
   void reloadFile();
   void showAbout();
   void closeAbout();
   void enableRunGui( bool enable );
   void changeCache();

private:
   Processor* _processor;
   QString    _programFileName;

   QTableView* _cacheView;

   AboutWindow* _aboutWindow;

   QAction* _loadAction;
   QAction* _reloadAction;
   QAction* _runAction;
   QAction* _stepAction;
};
