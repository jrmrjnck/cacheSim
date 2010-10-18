/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/
#ifndef MEMORY_H
#define MEMORY_H

#include <QAbstractTableModel>

#include "Exceptions.h"

exceptionClass( AccessViolation );

// Data model of a simple memory bank
class Memory : public QAbstractTableModel
{
   Q_OBJECT

public:
   Memory( int wordSize, int numWords );
   ~Memory();

   int maxAddress();
   int wordSize();
   void clearMemory();
   void writeData( int address, int data ) throw(AccessViolation);
   int readData( int address ) throw(AccessViolation);

   // Virtual Implementations from QAbstractTableModel
   int rowCount( const QModelIndex& parent = QModelIndex() ) const;
   int columnCount( const QModelIndex& parent = QModelIndex() ) const;
   QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
   QVariant headerData( int section, Qt::Orientation o, int role = Qt::DisplayRole ) const;

private:
   int* _data;
   int  _numWords;
   int  _wordSize;
};

#endif // MEMORY_H
