/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 5
 * October 28, 2010
 *
\****************************************************************************/
#ifndef CACHEPARAMETERDIALOG_H
#define CACHEPARAMETERDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QTabWidget>

class CacheParameterDialog : public QDialog
{
   Q_OBJECT;
public:
   CacheParameterDialog( int blockSize, int words, QWidget* parent = NULL );

   void setTab( int index );

   int blockSize();
   int words();

private slots:
   void _getData( int resultCode );

private:
   int _blockSize;
   int _words;

   QTabWidget* _optionTabs;
   QGroupBox*  _sizeGroup;
   QGroupBox*  _blockGroup;
};

#endif // CACHEPARAMETERDIALOG_H
