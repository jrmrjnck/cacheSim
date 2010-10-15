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

// Binary Logarithm
// from http://en.wikipedia.org/wiki/Binary_logarithm
int floorLog2(unsigned int n) {
  if (n == 0)
    return -1;

  int pos = 0;
  if (n >= 1<<16) { n >>= 16; pos += 16; }
  if (n >= 1<< 8) { n >>=  8; pos +=  8; }
  if (n >= 1<< 4) { n >>=  4; pos +=  4; }
  if (n >= 1<< 2) { n >>=  2; pos +=  2; }
  if (n >= 1<< 1) {           pos +=  1; }
  return pos;
}
