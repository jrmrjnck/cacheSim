/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 4
 * October 17, 2010
 *
\****************************************************************************/

#include <QtCore/QDebug>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtGui/QListWidgetItem>
#include <QtGui/QHeaderView>
#include <QtGui/QMessageBox>

#include "Exceptions.h"
#include "Processor.h"

// Constructor
Processor::Processor( QWidget *parent ) : QWidget( parent )
{
   qDebug() << QString("Accumulator size = %1 bits.")
               .arg(sizeof(WORD)*CHAR_BIT);

   // Initialize memory components
   _cache = new Cache( 4, 8 );
   _memory = new Memory( 16, 512 );
   _cache->setMemory( _memory );
   _counter = 0;
   _accumulator = 0;

   // Initialize GUI elements
   _insDisplay = new QListWidget();
   _insDisplay->setSelectionMode( QAbstractItemView::NoSelection );
   _insDisplay->setSpacing( 5 );

   _stepButton = new QPushButton( "Step" );

   QLabel* programLabel = new QLabel( "Program:" );
   QLabel* accLabel = new QLabel( "Accumulator Value:" );

   _accBox = new QLineEdit;
   _accBox->setReadOnly( true );
   _accBox->setAlignment( Qt::AlignRight );
   _updateAccDisplay();

   QGridLayout* _layout = new QGridLayout( this );
   _layout->addWidget( programLabel );
   _layout->addWidget( _insDisplay, 1, 0, 1, 3 );
   _layout->addWidget( _stepButton, 0, 2 );
   _layout->addWidget( accLabel, 2, 0 );
   _layout->addWidget( _accBox, 2, 2 );

   setLayout( _layout );

   connect( _stepButton, SIGNAL(clicked()), SLOT(step()) );
   _enableGui( false );
}

// Run all instructions
void Processor::run()
{
   int s = _instructions.size();
   while( s-- )
   {
      step();
   }
}

// Slot called when gui wants to step to the next instruction
void Processor::step()
{
   // Clear the arrow from the previous instruction
   _insDisplay->item(_counter)->setIcon(QIcon("blank.png"));

   // Watch for critical errors and "crash" the program
   try {
      _execInstruction();
   }
   catch( std::runtime_error& e ) {
      QMessageBox error;
      error.setIcon( QMessageBox::Critical );
      error.setText( e.what() );
      error.exec();
      _enableGui( false );
   }

   // If we just ran the final instruction
   if( _counter == _instructions.size()-1 ) {
      _enableGui( false );
      return;
   }

   // Point the arrow at the next instruction
   _insDisplay->item(++_counter)->setIcon(QIcon("arrow.png"));
   _insDisplay->setCurrentRow( _counter );
}

// Actually run the instruction pointed at by _counter
void Processor::_execInstruction()
{
   int operand = _instructions.at(_counter).operand;
   bool cacheHit = false;
   Qt::GlobalColor color = Qt::black;

   switch( _instructions.at(_counter).opcode )
   {
   case LOAD:
      _accumulator = _cache->readData( operand, &cacheHit );
      cacheHit ? color = Qt::green : color = Qt::red;
      break;
   case STORE:
      _cache->writeData( operand, _accumulator, &cacheHit );
      cacheHit ? color = Qt::green : color = Qt::red;
      break;
   case ADD:
      _accumulator += operand;
      break;
   case HALT:
      _cache->flushCache();
      _enableGui( false );
      break;
   default:
      break;
   }

   _insDisplay->item(_counter)->setForeground( color );

   _updateAccDisplay();
}

// Update the accumulator display with the internal
// value of _accumulator
void Processor::_updateAccDisplay()
{
   QString number = QString::number( _accumulator, 16 );
   int digits = (sizeof(_accumulator)*CHAR_BIT) / 4;
   while( number.size() < digits ) {
      number.prepend( "0" );
   }
   _accBox->setText( "0x" + number.right(digits) );
}

Cache* Processor::cache()
{
   return _cache;
}

Memory* Processor::memory()
{
   return _memory;
}

// Open a file specified by filename and read the instructions,
// one per line
void Processor::readFile( QString filename ) throw(FileAccessException)
{
   // Check for a valid file
   QFile sourceFile( filename );
   if( !sourceFile.exists() || !sourceFile.open(QIODevice::ReadOnly) ) {
      throw FileAccessException( "Couldn't read " + filename.toStdString() );
   }

   _instructions.clear();
   _insDisplay->clear();

   QTextStream fileStream( &sourceFile );
   bool programExists = false;

   // Interpret each line as an instruction
   for(int i = 0; !fileStream.atEnd(); ++i ) {
      QString line = fileStream.readLine().trimmed();
      QStringList parts = line.split( ' ', QString::SkipEmptyParts );

      // Skip blank lines
      if( line.isEmpty() )
         continue;

      // Malformed instructions
      if( parts.size() != 2 && parts.at(0) != "HALT" ) {
         _showParseError( QString("Wrong number of operands for instructions on line %1:\n%2")
                          .arg(i+1).arg(line) );
         continue;
      }

      Instruction ins;

      // Find the opcode
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
         _showParseError( QString("Invalid opcode found on line %1:\n%2").arg(i+1).arg(line) );
         continue;
      }

      bool ok = true;

      // Find the operand
      if( ins.opcode == HALT )
         ins.operand = 0;
      else if( ins.opcode == ADD )
         ins.operand = parts.at(1).toInt( &ok, 10 );
      else {
         ins.operand = parts.at(1).toInt( &ok, 16 );
      }

      // Couldn't convert the operand to a number
      if( !ok ) {
         _showParseError( QString("Could not convert operand on line %1 to number:\n%2")
                          .arg(i+1).arg(line) );
         continue;
      }

      _instructions.append( ins );
      _insDisplay->insertItem( i, new QListWidgetItem(QIcon("blank.png"),line) );

      // Check to see that at least one valid instruction was found
      programExists = true;
   }

   if( !programExists )
      return;

   // Reset the GUI elements
   _counter = 0;
   _insDisplay->setCurrentRow( 0 );
   _accumulator = 0;
   _updateAccDisplay();
   _enableGui( true );
   _insDisplay->item(0)->setIcon( QIcon("arrow.png") );
   _cache->clearCache();
   _memory->clearMemory();
}

// Alert the user to some kind of parse error in the file
int Processor::_showParseError( QString message )
{
   QMessageBox error;
   error.setIcon( QMessageBox::Warning );
   error.setText( QString("%1\n\n%2")
                  .arg(message)
                  .arg("This line will be removed from the program.") );
   return error.exec();
}

// Enable or disable the buttons
void Processor::_enableGui( bool enable )
{
   _stepButton->setEnabled( enable );
   
   emit enableGui( enable );
}
