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

class Memory : public QAbstractTableModel
{
   Q_OBJECT

public:
   Memory();
   ~Memory();

   int rowCount( const QModelIndex& parent = QModelIndex() ) const;
   int columnCount( const QModelIndex& parent = QModelIndex() ) const;
   QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
   QVariant headerData( int section, Qt::Orientation o, int role = Qt::DisplayRole ) const;

};

#endif // MEMORY_H
