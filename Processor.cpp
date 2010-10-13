/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtGui/QTableWidget>
#include <QtGui/QHeaderView>

#include "Exceptions.h"
#include "Processor.h"

Processor::Processor(QWidget *parent) : QWidget(parent)
{
   // Initialize memory components
   _cache = new Cache( 4, 8 );
   _memory = new Memory();

   // Initialize GUI elements
   _insDisplay = new QTableWidget( 0, 2 );
   _insDisplay->horizontalHeader()->setVisible(false);
   _insDisplay->verticalHeader()->setVisible(false);
   _insDisplay->setShowGrid( false );
   _insDisplay->setLineWidth( 0 );
   _insDisplay->setEditTriggers( QAbstractItemView::NoEditTriggers );
   _stepButton = new QPushButton( "Step" );
   QLabel* _programLabel = new QLabel( "Program:" );
   QLabel* _accLabel = new QLabel( "Accumulator Value:" );
   _accBox = new QLineEdit;
   _accBox->setReadOnly( true );

   QGridLayout* _layout = new QGridLayout( this );
   _layout->addWidget( _programLabel );
   _layout->addWidget( _insDisplay, 1, 0, 1, 3 );
   _layout->addWidget( _stepButton, 0, 2 );
   _layout->addWidget( _accLabel, 2, 0 );
   _layout->addWidget( _accBox, 2, 2 );

   setLayout( _layout );

   connect( _stepButton, SIGNAL(clicked()), SLOT(_step()) );
}

// Execute the next instruction
void Processor::_step()
{
}

// Stop execution of program
void Processor::_halt()
{
}

Cache* Processor::getCache()
{
   return _cache;
}

Memory* Processor::getMemory()
{
   return _memory;
}

void Processor::readFile( QString filename )
{
   QFile sourceFile( filename );
   if( !sourceFile.exists() || !sourceFile.open(QIODevice::ReadOnly) ) {
      throw FileAccessException( "Couldn't open file " + filename.toStdString() );
   }

   _instructions.clear();
   QTextStream fileStream( &sourceFile );

   for(int i = 0; !fileStream.atEnd(); ++i ) {
      QString line = fileStream.readLine();
      QStringList parts = line.split( ' ' );
      if( parts.size() > 2 ) {
         // TODO: Invalid instruction
         return;
      }

      Instruction ins;
      QString oc = parts.at(0);
      if( oc == "LOAD" )
         ins.opcode = LOAD;
      else if( oc == "STORE" )
         ins.opcode = STORE;
      else if( oc == "ADD" )
         ins.opcode = ADD;
      else if( oc == "HALT" )
         ins.opcode = HALT;
      else {
         // TODO: Invalid instruction
      }

      _insDisplay->setRowCount( i + 1 );
      if( ins.opcode == HALT )
         ins.operand = 0;
      else {
         ins.operand = parts.at(1).toInt();
         _insDisplay->setItem( i, 1, new QTableWidgetItem(parts.at(1)) );
      }

      _instructions.append( ins );
      _insDisplay->setItem( i, 0, new QTableWidgetItem(oc) );
   }
}
