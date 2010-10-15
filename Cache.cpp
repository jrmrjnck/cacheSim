/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include <QtCore/QVariant>

#include "Cache.h"
int floorLog2( unsigned int );

Cache::Cache( int wordsPerBlock, int lines ) : QAbstractTableModel()
{
   _cacheLines = lines;
   _wordsPerBlock = wordsPerBlock;

   for( int i = 0; i < _cacheLines; ++i ) {
      _cache.append( new CacheLine(_wordsPerBlock) );
   }

   clearCache();
}

void Cache::flushCache()
{
   // Write back to main memory
   for( int i = 0; i < _cacheLines; ++i ) {
      if( _cache[i]->dirty )
         _writeToMemory( i );
   }

   //clearCache();
}

void Cache::clearCache()
{
   for( int i = 0; i < _cacheLines; ++i ) {
      _cache[i]->dirty = false;
      _cache[i]->tag   = 0;
      for( int j = 0; j < _wordsPerBlock; ++j ) {
         _cache[i]->words[j] = 0;
      }
   }
}

int Cache::readData( int address )
{
   if( address > _mainMem->maxAddress() ) {
      // TODO: Account for bad address
      return 0;
   }

   int offset = address & (_wordsPerBlock-1);
   int tag = address >> floorLog2(_wordsPerBlock);
   int line = tag & (_cacheLines-1);
   tag >>= floorLog2(_cacheLines);

   if( _cache[line]->tag != tag )
      _loadFromMemory( tag, line );

   _updateRow( line );

   return _cache[line]->words[offset];
}

void Cache::writeData( int address, int value )
{
   if( address > _mainMem->maxAddress() ) {
      // TODO: bad address
      return;
   }

   int offset = address & (_wordsPerBlock-1);
   int tag = address >> floorLog2(_wordsPerBlock);
   int line = tag & (_cacheLines-1);
   tag >>= floorLog2( _cacheLines );

   if( _cache[line]->tag != tag )
      _loadFromMemory( tag, line );

   _cache[line]->words[offset] = value;
   _cache[line]->dirty = true;

   // Update the view
   _updateRow( line );
}

void Cache::_loadFromMemory( int tag, int line )
{
   if( _cache[line]->dirty )
      _writeToMemory( line );

   // Clear dirty bit and set tag
   _cache[line]->dirty = false;
   _cache[line]->tag = tag;

   tag <<= floorLog2( _cacheLines );
   tag |= line;
   int baseAddress = tag << floorLog2( _wordsPerBlock );

   for( int i = 0; i < _wordsPerBlock; ++i ) {
      _cache[line]->words[i] = _mainMem->readData( baseAddress+i );
   }

}

void Cache::_writeToMemory( int line )
{
   int tag = _cache[line]->tag;
   tag <<= floorLog2(_cacheLines);
   tag |= line;
   int baseAddress = tag << floorLog2(_wordsPerBlock);

   for( int i = 0; i < _wordsPerBlock; ++i ) {
      _mainMem->writeData( baseAddress+i, _cache[line]->words[i] );
   }
}

void Cache::setMemory( Memory *mem )
{
   _mainMem = mem;
}

int Cache::rowCount( const QModelIndex& parent ) const
{
   if( parent.isValid() )
      return 0;

   return _cache.size();
}

int Cache::columnCount( const QModelIndex& parent ) const
{
   if( parent.isValid() )
      return 0;

   return (_wordsPerBlock + 2);
}

QVariant Cache::data( const QModelIndex& index, int role ) const
{
   // Check validity of request
   if( !index.isValid() || role != Qt::DisplayRole )
      return QVariant();

   int line = index.row();
   int column = index.column();

   // Dirty bit
   if( column == 0 ) {
      return (_cache[line]->dirty ? QVariant("1") : QVariant("0"));
   }
   else if( column == 1 ) {
      int tag = _cache[line]->tag;
      return QVariant( "0x" + QString::number(tag,16));
   }
   else if( column >= 2 && column < _wordsPerBlock+2 ){
      int value = _cache[line]->words[column-2];
      return QVariant( "0x" + QString::number(value,16));
   }

   return QVariant();
}

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

void Cache::_updateRow( int line )
{
   QModelIndex start = index( line, 0 );
   QModelIndex end = index( line, _wordsPerBlock+2 );
   emit dataChanged( start, end );
}
