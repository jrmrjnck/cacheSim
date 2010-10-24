/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QWidget>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>

#include "Cache.h"
#include "Memory.h"
#include "Exceptions.h"
#include "CacheStatistics.h"

typedef int16_t WORD;

enum OpCode{LOAD,STORE,ADD,HALT};

struct Instruction
{
   OpCode opcode;
   int operand;
};

// The Processor class takes care of running the program,
// interfacing with the cache, and also acts as a view on
// the internal state of the program (i.e. instruction list + PC)
class Processor : public QWidget
{
Q_OBJECT
public:
   explicit Processor(QWidget *parent = 0);
   ~Processor();

   Cache*  cache();
   Memory* memory();
   void readFile( QString filename ) throw(FileAccessException);

public slots:
   void run();
   void step();
   void changeBlockSize();
   void changeCacheSize();

private:
   void _execInstruction();
   void _updateAccDisplay();
   void _enableGui( bool enabled );

   int _showParseError( QString message );

signals:
   void enableGui( bool );
   void cacheChanged( void );

private:
   Cache*  _cache;
   Memory* _memory;

   Cache* _newCache;

   QString _programFileName;
   QList<Instruction> _instructions;
   int  _counter;
   WORD _accumulator;

   QListWidget* _insDisplay;
   QPushButton* _stepButton;
   QLineEdit*   _accBox;

   CacheStatistics* _statistics;
};

#endif // PROCESSOR_H
