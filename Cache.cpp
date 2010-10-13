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

Cache::Cache( int wordsPerBlock, int lines ) : QAbstractTableModel(),
                                               _cache(lines,wordsPerBlock)
{
   _cacheLines = lines;
   _wordsPerBlock = wordsPerBlock;

   _flushCache();
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
      return (_cache[line].dirty ? QVariant("1") : QVariant("0"));
   }
   else if( column == 1 ) {
      int tag = _cache[line].tag;
      return QVariant(QString::number(tag,16));
   }
   else {
      return QVariant(QString::number(_cache[line].words[column-2],16));
   }
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

   return QVariant();
}

void Cache::_flushCache()
{
   for( int i = 0; i < _cacheLines; ++i ) {
      _cache[i].dirty = false;
      _cache[i].tag   = 0;
      for( int j = 0; j < _wordsPerBlock; ++j ) {
         _cache[i].words[j] = 0;
      }
   }
}
