/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 5
 * October 28, 2010
 *
\****************************************************************************/
#ifndef CACHESTATISTICS_H
#define CACHESTATISTICS_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>

class CacheStatistics : public QWidget
{
   Q_OBJECT;
public:
   CacheStatistics();
   ~CacheStatistics();

   void loadCount( bool hit );
   void storeCount( bool hit );
   void reset();

public slots:
   void updateDisplay();

private:
   QCheckBox* _loadCheckBox;
   QCheckBox* _storeCheckBox;
   QLineEdit* _hits;
   QLineEdit* _misses;

   int _loadHits;
   int _loadMisses;
   int _storeHits;
   int _storeMisses;
};

#endif // CACHESTATISTICS_H
