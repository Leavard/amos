/****************************************************************************
** Form implementation generated from reading ui file 'QueryWidget.ui'
**
** Created: Sat Apr 22 16:17:54 2006
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "QueryWidget.hh"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qslider.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a QueryWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QueryWidget::QueryWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "QueryWidget" );
    QueryWidgetLayout = new QVBoxLayout( this, 0, 0, "QueryWidgetLayout"); 

    queryBox = new QGroupBox( this, "queryBox" );
    queryBox->setFlat( TRUE );
    queryBox->setColumnLayout(0, Qt::Vertical );
    queryBox->layout()->setSpacing( 6 );
    queryBox->layout()->setMargin( 11 );
    queryBoxLayout = new QGridLayout( queryBox->layout() );
    queryBoxLayout->setAlignment( Qt::AlignTop );

    ceLabel = new QLabel( queryBox, "ceLabel" );
    ceLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, ceLabel->sizePolicy().hasHeightForWidth() ) );
    QFont ceLabel_font(  ceLabel->font() );
    ceLabel_font.setPointSize( 9 );
    ceLabel->setFont( ceLabel_font ); 
    ceLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    queryBoxLayout->addWidget( ceLabel, 4, 0 );

    covLabel = new QLabel( queryBox, "covLabel" );
    covLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, covLabel->sizePolicy().hasHeightForWidth() ) );
    QFont covLabel_font(  covLabel->font() );
    covLabel_font.setPointSize( 9 );
    covLabel->setFont( covLabel_font ); 
    covLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    queryBoxLayout->addWidget( covLabel, 5, 0 );

    kmerLabel = new QLabel( queryBox, "kmerLabel" );
    kmerLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, kmerLabel->sizePolicy().hasHeightForWidth() ) );
    QFont kmerLabel_font(  kmerLabel->font() );
    kmerLabel_font.setPointSize( 9 );
    kmerLabel->setFont( kmerLabel_font ); 
    kmerLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    queryBoxLayout->addWidget( kmerLabel, 6, 0 );

    ceSlider = new QSlider( queryBox, "ceSlider" );
    ceSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, ceSlider->sizePolicy().hasHeightForWidth() ) );
    ceSlider->setMinValue( 0 );
    ceSlider->setMaxValue( 10 );
    ceSlider->setValue( 0 );
    ceSlider->setOrientation( QSlider::Horizontal );
    ceSlider->setTickmarks( QSlider::Below );
    ceSlider->setTickInterval( 1 );

    queryBoxLayout->addWidget( ceSlider, 4, 1 );

    snpSlider = new QSlider( queryBox, "snpSlider" );
    snpSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, snpSlider->sizePolicy().hasHeightForWidth() ) );
    snpSlider->setMinValue( 0 );
    snpSlider->setMaxValue( 10 );
    snpSlider->setValue( 0 );
    snpSlider->setOrientation( QSlider::Horizontal );
    snpSlider->setTickmarks( QSlider::Below );
    snpSlider->setTickInterval( 1 );

    queryBoxLayout->addWidget( snpSlider, 3, 1 );

    snpLabel = new QLabel( queryBox, "snpLabel" );
    snpLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, snpLabel->sizePolicy().hasHeightForWidth() ) );
    QFont snpLabel_font(  snpLabel->font() );
    snpLabel_font.setPointSize( 9 );
    snpLabel->setFont( snpLabel_font ); 
    snpLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    queryBoxLayout->addWidget( snpLabel, 3, 0 );

    covSlider = new QSlider( queryBox, "covSlider" );
    covSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, covSlider->sizePolicy().hasHeightForWidth() ) );
    covSlider->setMinValue( 0 );
    covSlider->setMaxValue( 10 );
    covSlider->setValue( 0 );
    covSlider->setOrientation( QSlider::Horizontal );
    covSlider->setTickmarks( QSlider::Below );
    covSlider->setTickInterval( 1 );

    queryBoxLayout->addWidget( covSlider, 5, 1 );

    kmerSlider = new QSlider( queryBox, "kmerSlider" );
    kmerSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, kmerSlider->sizePolicy().hasHeightForWidth() ) );
    kmerSlider->setMinValue( 0 );
    kmerSlider->setMaxValue( 10 );
    kmerSlider->setValue( 0 );
    kmerSlider->setOrientation( QSlider::Horizontal );
    kmerSlider->setTickmarks( QSlider::Below );
    kmerSlider->setTickInterval( 1 );

    queryBoxLayout->addWidget( kmerSlider, 6, 1 );

    line1 = new QFrame( queryBox, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );

    queryBoxLayout->addMultiCellWidget( line1, 2, 2, 0, 1 );

    layout8 = new QHBoxLayout( 0, 0, 6, "layout8"); 

    happyEdit = new QLineEdit( queryBox, "happyEdit" );
    happyEdit->setCursor( QCursor( 4 ) );
    happyEdit->setAlignment( int( QLineEdit::AlignRight ) );
    layout8->addWidget( happyEdit );

    happyButton = new QPushButton( queryBox, "happyButton" );
    happyButton->setMaximumSize( QSize( 130, 21 ) );
    QFont happyButton_font(  happyButton->font() );
    happyButton_font.setPointSize( 9 );
    happyButton->setFont( happyButton_font ); 
    layout8->addWidget( happyButton );

    queryBoxLayout->addMultiCellLayout( layout8, 1, 1, 0, 1 );

    layout9 = new QHBoxLayout( 0, 0, 6, "layout9"); 

    searchEdit = new QLineEdit( queryBox, "searchEdit" );
    searchEdit->setCursor( QCursor( 4 ) );
    searchEdit->setAlignment( int( QLineEdit::AlignLeft ) );
    layout9->addWidget( searchEdit );

    searchButton = new QPushButton( queryBox, "searchButton" );
    searchButton->setMaximumSize( QSize( 60, 21 ) );
    QFont searchButton_font(  searchButton->font() );
    searchButton_font.setPointSize( 9 );
    searchButton->setFont( searchButton_font ); 
    layout9->addWidget( searchButton );

    queryBoxLayout->addMultiCellLayout( layout9, 0, 0, 0, 1 );
    QueryWidgetLayout->addWidget( queryBox );
    languageChange();
    resize( QSize(181, 255).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( searchEdit, SIGNAL( returnPressed() ), searchButton, SLOT( animateClick() ) );
    connect( happyEdit, SIGNAL( returnPressed() ), happyButton, SLOT( animateClick() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
QueryWidget::~QueryWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QueryWidget::languageChange()
{
    setCaption( tr( "Form2" ) );
    queryBox->setTitle( tr( "Queries" ) );
    ceLabel->setText( tr( "CE Stat" ) );
    covLabel->setText( tr( "Coverage" ) );
    kmerLabel->setText( tr( "Kmer Freq" ) );
    snpLabel->setText( tr( "SNPs" ) );
    happyEdit->setText( tr( "2.0" ) );
    happyButton->setText( tr( "Happy Distance" ) );
    searchButton->setText( tr( "Search" ) );
}

