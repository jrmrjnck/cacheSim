/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include <QtGui/QApplication>

#include "MainWindow.h"

int main( int argc, char* argv[] )
{
   QApplication cacheSim( argc, argv );

   MainWindow w;
   w.show();
   
   return cacheSim.exec();
}

