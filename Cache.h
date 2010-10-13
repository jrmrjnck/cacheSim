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
#include <QtCore/QVector>

class QModelIndex;
class CacheLine;

// A model of a direct-mapped cache
class Cache : public QAbstractTableModel
{
   Q_OBJECT
 
public:
   Cache( int wordsPerBlock, int lines );

   int rowCount( const QModelIndex& parent = QModelIndex() ) const;
   int columnCount( const QModelIndex& parent = QModelIndex() ) const;
   QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
   QVariant headerData( int section, Qt::Orientation o, int role = Qt::DisplayRole ) const;

private:
   void _flushCache();
   
private:
   QVector<CacheLine> _cache;

   int _cacheLines;
   int _wordsPerBlock;
};

class CacheLine
{
public:
   CacheLine( int numWords = 0 ) : dirty(0),
                                   tag(0),
                                   words(new int[numWords])
   {};

   bool dirty;
   int  tag;
   int* words;
};

#endif // CACHE_H
