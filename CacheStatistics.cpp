/****************************************************************************\
 *
 * Jonathan Doman
 * 905260988
 * ECE 3574 - Homework 5
 * October 28, 2010
 *
\****************************************************************************/

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>

#include "CacheStatistics.h"

// Constructor
CacheStatistics::CacheStatistics()
{
   _loadCheckBox = new QCheckBox( "LOAD" );
   _storeCheckBox = new QCheckBox( "STORE" );
   _loadCheckBox->setChecked( true );
   _storeCheckBox->setChecked( true );

   // Instruction selection group box
   QGroupBox* insType = new QGroupBox( "Instruction" );
   insType->setFlat( false );
   QVBoxLayout* insLayout = new QVBoxLayout;
   insLayout->addWidget( _loadCheckBox );
   insLayout->addWidget( _storeCheckBox );
   insType->setLayout( insLayout );

   // Count display
   QLabel* hits = new QLabel( "Hits:" );
   QLabel* misses = new QLabel( "Misses:" );
   hits->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
   misses->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
   _hits = new QLineEdit;
   _misses = new QLineEdit;
   _hits->setReadOnly( true );
   _misses->setReadOnly( true );

   QGridLayout* layout = new QGridLayout( this );
   layout->addWidget( insType, 0, 0, 2, 1 );
   layout->addWidget( hits, 0, 1 );
   layout->addWidget( _hits, 0, 2 );
   layout->addWidget( misses, 1, 1 );
   layout->addWidget( _misses, 1, 2 );
   layout->setColumnStretch( 3, 1 );

   // Initialize data
   _loadMisses = _loadHits = _storeHits = _storeMisses = 0;
   updateDisplay();

   connect( _loadCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateDisplay()) );
   connect( _storeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateDisplay()) );
}

// Destructor
CacheStatistics::~CacheStatistics()
{
}

void CacheStatistics::loadCount( bool hit )
{
   if( hit )
      ++_loadHits;
   else
      ++_loadMisses;
}

void CacheStatistics::storeCount( bool hit )
{
   if( hit )
      ++_storeHits;
   else
      ++_storeMisses;
}

void CacheStatistics::updateDisplay()
{
   int hits = 0;
   int misses = 0;

   if( _loadCheckBox->isChecked() ) {
      hits += _loadHits;
      misses += _loadMisses;
   }
   if( _storeCheckBox->isChecked() ) {
      hits += _storeHits;
      misses += _storeMisses;
   }

   _hits->setText( QString::number(hits) );
   _misses->setText( QString::number(misses) );
}

void CacheStatistics::reset()
{
   _loadHits = _loadMisses = _storeHits = _storeMisses = 0;
   updateDisplay();
}
