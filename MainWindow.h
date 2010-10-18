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
   void openFile();
   void showAbout();
   void closeAbout();

private:
   Processor* _processor;

   QTableView* _cacheView;
   QTableView* _memoryView;

   AboutWindow* _aboutWindow;
};
