/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include "Memory.h"

Memory::Memory( int wordSize, int numWords )
{
   _mask = (1 << wordSize) - 1;
   _numWords = numWords;
   _data = new int[numWords];

   for( int i = 0; i < _numWords; ++i )
      _data[i] = 0;
}

Memory::~Memory()
{
   delete [] _data;
}

int Memory::maxAddress()
{
   return _numWords - 1;
}

void Memory::writeData( int address, int data )
{
   if( address > _numWords ){
      // TODO: Invalid address
   }

   _data[address] = data;

   QModelIndex cell = index( address, 0 );
   emit dataChanged( cell, cell );
}

int Memory::readData( int address )
{
   if( address > _numWords ) {
      // TODO: Invalid address
   }

   return _data[address];
}

int Memory::rowCount( const QModelIndex& parent ) const
{
   return _numWords;
}

int Memory::columnCount( const QModelIndex& parent ) const
{
   return 1;
}

QVariant Memory::data( const QModelIndex& index, int role ) const
{
   // Check validity of request
   if( !index.isValid() || role != Qt::DisplayRole ||
       index.column() != 0 )
      return QVariant();

   return QVariant( "0x" + QString::number(_data[index.row()],16) );
}

QVariant Memory::headerData( int section, Qt::Orientation orientation, int role ) const
{
   if( role != Qt::DisplayRole )
      return QVariant();

   if( orientation == Qt::Vertical ) {
      return QVariant( "0x" + QString::number(section,16) );
   }

   return QVariant();
}
