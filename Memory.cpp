/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include "Memory.h"

// Constructor
Memory::Memory( int wordSize, int numWords )
{
   _wordSize = wordSize;
   _numWords = numWords;
   _data = new int[numWords];

   clearMemory();
}

// Destructor
Memory::~Memory()
{
   delete [] _data;
}

// Return the address bounds
int Memory::maxAddress()
{
   return _numWords - 1;
}

// Return the word size in bits
int Memory::wordSize()
{
   return _wordSize;
}

// Clear all the words to zero
void Memory::clearMemory()
{
   for( int i = 0; i < _numWords; ++i )
      _data[i] = 0;
}

// Write data to address
void Memory::writeData( int address, int data ) throw(AccessViolation)
{
   if( address >= _numWords ){
      throw AccessViolation( "Couldn't write at " +
                             QString::number(address,16).toStdString() );
   }

   _data[address] = data;

   // Tell the view to reload values
   QModelIndex cell = index( address, 0 );
   emit dataChanged( cell, cell );
}

// Return data located at address
int Memory::readData( int address ) throw(AccessViolation)
{
   if( address >= _numWords ) {
      throw AccessViolation( "Couldn't read at " +
                            QString::number(address,16).toStdString() );
   }

   return _data[address];
}

// Return the number of rows to show in the table
int Memory::rowCount( const QModelIndex& parent ) const
{
   // Every word gets its own row
   return _numWords;
}

// Return the number of columns to show in the table
int Memory::columnCount( const QModelIndex& parent ) const
{
   return 1;
}

// Outside interface to the memory data
QVariant Memory::data( const QModelIndex& index, int role ) const
{
   // Check validity of request
   if( !index.isValid() || role != Qt::DisplayRole ||
       index.column() != 0 )
      return QVariant();

   // Only show the word size
   QString number = QString::number( _data[index.row()],16 );
   return QVariant( "0x" + number.right(_wordSize/4) );
}

// Define the labels to show in the table headers
QVariant Memory::headerData( int section, Qt::Orientation orientation, int role ) const
{
   if( role != Qt::DisplayRole || orientation != Qt::Vertical )
      return QVariant();

   return QVariant( "0x" + QString::number(section,16) );
}
