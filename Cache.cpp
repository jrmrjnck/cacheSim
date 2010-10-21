/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include <QtCore/QVariant>
#include <QtCore/QDebug>

#include "Cache.h"

int floorLog2( unsigned int );

// Constructor
Cache::Cache( int wordsPerBlock, int lines ) : QAbstractTableModel()
{
   _cacheLines = lines;
   _wordsPerBlock = wordsPerBlock;

   for( int i = 0; i < _cacheLines; ++i ) {
      _data.append( new CacheLine(_wordsPerBlock) );
   }

   clearCache();
}

// Destructor
Cache::~Cache()
{
   while( !_data.isEmpty() ) {
      delete _data.takeFirst();
   }
}

// Write back anything that has changed in cache
void Cache::flushCache()
{
   for( int i = 0; i < _cacheLines; ++i ) {
      if( _data[i]->dirty ) {
         _writeToMemory( i );
         _data[i]->dirty = false;
      }
      _updateRow( i );
   }
}

// Zero out all the bits
void Cache::clearCache()
{
   for( int i = 0; i < _cacheLines; ++i ) {
      _data[i]->dirty = false;
      _data[i]->tag   = 0;
      for( int j = 0; j < _wordsPerBlock; ++j ) {
         _data[i]->words[j] = 0;
      }
      _updateRow( i );
   }
}

// Get the value residing at address
int Cache::readData( int address )
{
   // Seperate the parts of the address
   int offset = address & (_wordsPerBlock-1);
   int tag = address >> floorLog2(_wordsPerBlock);
   int line = tag & (_cacheLines-1);
   tag >>= floorLog2(_cacheLines);

   if( _data[line]->tag != tag )
      _loadFromMemory( tag, line );

   _updateRow( line );

   return _data[line]->words[offset];
}

// Write value to address
void Cache::writeData( int address, int value )
{
   int offset = address & (_wordsPerBlock-1);
   int tag = address >> floorLog2(_wordsPerBlock);
   int line = tag & (_cacheLines-1);
   tag >>= floorLog2( _cacheLines );

   if( _data[line]->tag != tag )
      _loadFromMemory( tag, line );

   _data[line]->words[offset] = value;
   _data[line]->dirty = true;

   // Update the view
   _updateRow( line );
}

// Retrieve a block from memory and store in the cache
void Cache::_loadFromMemory( int tag, int line )
{
   if( _data[line]->dirty )
      _writeToMemory( line );

   qDebug() << QString("Loading line %1 from memory").arg(line);

   // Clear dirty bit and set tag
   _data[line]->dirty = false;
   _data[line]->tag = tag;

   tag <<= floorLog2( _cacheLines );
   tag |= line;
   int baseAddress = tag << floorLog2( _wordsPerBlock );

   for( int i = 0; i < _wordsPerBlock; ++i ) {
      _data[line]->words[i] = _mainMem->readData( baseAddress+i );
   }

}

// Write a block back to main memory
void Cache::_writeToMemory( int line )
{
   qDebug() << QString("Writing line %1 back to memory").arg(line);
   int tag = _data[line]->tag;
   tag <<= floorLog2(_cacheLines);
   tag |= line;
   int baseAddress = tag << floorLog2(_wordsPerBlock);

   for( int i = 0; i < _wordsPerBlock; ++i ) {
      _mainMem->writeData( baseAddress+i, _data[line]->words[i] );
   }
}

void Cache::setMemory( Memory *mem )
{
   _mainMem = mem;
}

// Return the number of rows to show in the table
int Cache::rowCount( const QModelIndex& parent ) const
{
   if( parent.isValid() )
      return 0;

   return _data.size();
}

// Return the number of table columns to be shown
int Cache::columnCount( const QModelIndex& parent ) const
{
   if( parent.isValid() )
      return 0;

   return (_wordsPerBlock + 2);
}

// Table interface to the cache data
QVariant Cache::data( const QModelIndex& index, int role ) const
{
   // Check validity of request
   if( !index.isValid() || role != Qt::DisplayRole )
      return QVariant();

   int line = index.row();
   int column = index.column();

   // Dirty bit
   if( column == 0 ) {
      return (_data[line]->dirty ? QVariant("1") : QVariant("0"));
   }
   // Tag
   else if( column == 1 ) {
      int tag = _data[line]->tag;
      return QVariant( "0x" + QString::number(tag,16));
   }
   // Data
   else if( column >= 2 && column < _wordsPerBlock+2 ) {
      int value = _data[line]->words[column-2];
      return QVariant( "0x" + QString::number(value,16).right(_mainMem->wordSize()/4) );
   }

   return QVariant();
}

// Define the labels to show in the table headers
QVariant Cache::headerData( int section, Qt::Orientation orientation, int role ) const
{
   if( role != Qt::DisplayRole )
      return QVariant();

   if( orientation == Qt::Vertical ) {
      return QVariant( "0x" + QString::number(section,16) );
   }

   if( section == 0 )
      return QVariant( "Dirty" );
   else if( section == 1 )
      return QVariant( "Tag" );
   else
      return QVariant( section - 2 );
}

// Send dataChanged signal for all cells in a certain row
void Cache::_updateRow( int line )
{
   QModelIndex start = index( line, 0 );
   QModelIndex end = index( line, _wordsPerBlock+2 );
   emit dataChanged( start, end );
}
