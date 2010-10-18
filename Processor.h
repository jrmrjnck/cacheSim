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

    Cache*  cache();
    Memory* memory();
    void    readFile( QString filename );

private slots:
    void _step();

private:
    void _execInstruction();
    void _updateAccDisplay();

    int _showParseError( QString message );

private:
    Cache*  _cache;
    Memory* _memory;

    QList<Instruction> _instructions;
    int  _counter;
    WORD _accumulator;

    QListWidget* _insDisplay;
    QPushButton* _stepButton;
    QLineEdit*   _accBox;
};

#endif // PROCESSOR_H
