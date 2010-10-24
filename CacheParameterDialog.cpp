/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 5
 * October 28, 2010
 *
\****************************************************************************/

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QRadioButton>

#include "CacheParameterDialog.h"

CacheParameterDialog::CacheParameterDialog( int blockSize, int words, 
                                            QWidget* parent ) : QDialog( parent )
{
   // Set internal data
   _blockSize = blockSize;
   _words = words;

   // Initialize GUI
   setWindowTitle( "Change Cache Parameters" );

   // Accept/Cancel buttons
   QDialogButtonBox* buttons = new QDialogButtonBox( Qt::Horizontal );
   buttons->addButton( "Update", QDialogButtonBox::AcceptRole );
   buttons->addButton( "Cancel", QDialogButtonBox::RejectRole );
   connect( buttons, SIGNAL(accepted()), this, SLOT(accept()) );
   connect( buttons, SIGNAL(rejected()), this, SLOT(reject()) );

   // Cache size radio buttons
   QList<int> sizeOptions;
   sizeOptions << 16 << 32 << 64;
   _sizeGroup = new QGroupBox( "Total words in cache" );
   QVBoxLayout* sizeLayout = new QVBoxLayout( _sizeGroup );

   foreach( int size, sizeOptions ) {
      QRadioButton* option = new QRadioButton( QString::number(size) );
      sizeLayout->addWidget( option );
      if( size == _words )
         option->setChecked( true );
   }

   // Words per block radio buttons
   QList<int> blockOptions;
   blockOptions << 1 << 2 << 4 << 8;
   _blockGroup = new QGroupBox( "Words per cache block" );
   QVBoxLayout* blockLayout = new QVBoxLayout( _blockGroup );

   foreach( int size, blockOptions ) {
      QRadioButton* option = new QRadioButton( QString::number(size) );
      blockLayout->addWidget( option );
      if( size == _blockSize )
         option->setChecked( true );
   }

   _optionTabs = new QTabWidget;
   _optionTabs->addTab( _blockGroup, "Block Size" );
   _optionTabs->addTab( _sizeGroup, "Cache Size" );
   
   QVBoxLayout* layout = new QVBoxLayout;
   layout->addWidget( _optionTabs );
   layout->addWidget( buttons );
   setLayout( layout );
}

int CacheParameterDialog::blockSize()
{
   return _blockSize;
}

int CacheParameterDialog::lines()
{
   return (_words / _blockSize);
}

void CacheParameterDialog::setTab( int index )
{
   _optionTabs->setCurrentIndex( index );
}

// Save the selections on close
void CacheParameterDialog::closeEvent( QCloseEvent* event )
{
   // Find cache size
   QList<QRadioButton*> options = _sizeGroup->findChildren<QRadioButton*>();
   foreach( QRadioButton* option, options)
   {
      if( option->isChecked() ) {
         _words = option->text().toInt( NULL, 10 );
         break;
      }
   }

   // Find block size
   options = _blockGroup->findChildren<QRadioButton*>();
   foreach( QRadioButton* option, options )
   {
      if( option->isChecked() ) {
         _blockSize = option->text().toInt( NULL, 10 );
         break;
      }
   }
}
