/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/
#ifndef CACHE_H
#define CACHE_H

#include <QAbstractTableModel>
#include <QtCore/QList>

#include "Memory.h"

class QModelIndex;
class CacheLine;

// A model of a direct-mapped cache
class Cache : public QAbstractTableModel
{
   Q_OBJECT
 
public:
   Cache( int wordsPerBlock, int lines );
   ~Cache();

   void setMemory( Memory* mem );
   void flushCache();
   void clearCache();

   int readData( int address, bool* cacheHit = NULL );
   void writeData( int address, int value, bool* cacheHit = NULL );

   // Virtual Implementations from QAbstractTableModel
   int rowCount( const QModelIndex& parent = QModelIndex() ) const;
   int columnCount( const QModelIndex& parent = QModelIndex() ) const;
   QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
   QVariant headerData( int section, Qt::Orientation o, int role = Qt::DisplayRole ) const;

private:
   void _loadFromMemory( int tag, int line );
   void _writeToMemory( int line );

   void _updateRow( int line );
   
private:
   QList<CacheLine*> _data;
   Memory* _mainMem;

   int _cacheLines;
   int _wordsPerBlock;
};

class CacheLine
{
public:
   CacheLine( int numWords = 0 )
   {
      dirty = false;
      tag = 0;
      words = new int[numWords];
   };

   bool dirty;
   int  tag;
   int* words;
};

#endif // CACHE_H
