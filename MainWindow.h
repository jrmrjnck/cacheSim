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

class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   MainWindow();
   ~MainWindow();

public slots:
   void openFile();

private:
   Processor* _proc;

   QMenu* _fileMenu;

   QTableView* _cacheView;
   QTableView* _memoryView;
};
