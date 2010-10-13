/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include "Memory.h"

Memory::Memory()
{
}

Memory::~Memory()
{
}

int Memory::rowCount( const QModelIndex& parent ) const
{
   return 0;
}

int Memory::columnCount( const QModelIndex& parent ) const
{
   return 0;
}

QVariant Memory::data( const QModelIndex& index, int role ) const
{
   return QVariant();
}

QVariant Memory::headerData( int section, Qt::Orientation orientation, int role ) const
{
   return QVariant();
}
