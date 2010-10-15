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
#include <QtGui/QListWidgetItem>
#include <QtGui/QHeaderView>

#include "Exceptions.h"
#include "Processor.h"

Processor::Processor(QWidget *parent) : QWidget(parent)
{
   // Initialize memory components
   _cache = new Cache( 4, 8 );
   _memory = new Memory;
   _cache->setMemory( _memory );
   _counter = 0;
   _running = false;
   _accumulator = 0;

   // Initialize GUI elements
   _insDisplay = new QListWidget();
   _insDisplay->setSelectionMode( QAbstractItemView::NoSelection );
   _insDisplay->setSpacing( 5 );
   _stepButton = new QPushButton( "Step" );
   QLabel* _programLabel = new QLabel( "Program:" );
   QLabel* _accLabel = new QLabel( "Accumulator Value:" );
   _accBox = new QLineEdit;
   _accBox->setReadOnly( true );
   _accBox->setAlignment( Qt::AlignRight );
   _accBox->setText( "0x0" );

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
   if( !_running )
      return;

   _insDisplay->item(_counter)->setIcon(QIcon("blank.png"));

   _execInstruction();

   if( _counter == _instructions.size()-1 ) {
      _running = false;
      _stepButton->setEnabled( false );
      return;
   }

   _insDisplay->item(++_counter)->setIcon(QIcon("arrow.png"));
}

void Processor::_execInstruction()
{
   int operand = _instructions.at( _counter ).operand;
   switch( _instructions.at(_counter).opcode )
   {
   case LOAD:
      _accumulator = _cache->readData( operand );
      break;
   case STORE:
      _cache->writeData( _instructions.at(_counter).operand,
                         _accumulator );
      break;
   case ADD:
      _accumulator += operand;
      break;
   case HALT:
      _cache->flushCache();
      break;
   default:
      break;
   }
   _updateAccDisplay();
}

void Processor::_updateAccDisplay()
{
   _accBox->setText( "0x" + QString::number(_accumulator, 16) );
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
   _insDisplay->clear();

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

      if( ins.opcode == HALT )
         ins.operand = 0;
      else if( ins.opcode == ADD )
         ins.operand = parts.at(1).toInt( 0, 10 );
      else {
         ins.operand = parts.at(1).toInt( 0, 16 );
      }

      _instructions.append( ins );
      _insDisplay->insertItem( i, new QListWidgetItem(QIcon("blank.png"),line) );
   }
   _counter = 0;
   _stepButton->setEnabled( true );
   _insDisplay->item(0)->setIcon( QIcon("arrow.png") );
   _running = true;
}
