/****************************************************************************
** Form implementation generated from reading ui file 'QueryWidget.ui'
**
** Created: Tue Apr 25 22:47:28 2006
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "QueryWidget.hh"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qslider.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
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

    mateTypeBox = new QGroupBox( queryBox, "mateTypeBox" );
    mateTypeBox->setColumnLayout(0, Qt::Vertical );
    mateTypeBox->layout()->setSpacing( 6 );
    mateTypeBox->layout()->setMargin( 11 );
    mateTypeBoxLayout = new QGridLayout( mateTypeBox->layout() );
    mateTypeBoxLayout->setAlignment( Qt::AlignTop );

    singletonCheck = new QCheckBox( mateTypeBox, "singletonCheck" );
    QFont singletonCheck_font(  singletonCheck->font() );
    singletonCheck_font.setPointSize( 9 );
    singletonCheck->setFont( singletonCheck_font ); 
    singletonCheck->setChecked( TRUE );

    mateTypeBoxLayout->addWidget( singletonCheck, 5, 0 );

    unmatedCheck = new QCheckBox( mateTypeBox, "unmatedCheck" );
    QFont unmatedCheck_font(  unmatedCheck->font() );
    unmatedCheck_font.setPointSize( 9 );
    unmatedCheck->setFont( unmatedCheck_font ); 
    unmatedCheck->setChecked( TRUE );

    mateTypeBoxLayout->addWidget( unmatedCheck, 6, 0 );

    stretchedCheck = new QCheckBox( mateTypeBox, "stretchedCheck" );
    QFont stretchedCheck_font(  stretchedCheck->font() );
    stretchedCheck_font.setPointSize( 9 );
    stretchedCheck->setFont( stretchedCheck_font ); 
    stretchedCheck->setChecked( TRUE );

    mateTypeBoxLayout->addWidget( stretchedCheck, 1, 0 );

    linkingCheck = new QCheckBox( mateTypeBox, "linkingCheck" );
    QFont linkingCheck_font(  linkingCheck->font() );
    linkingCheck_font.setPointSize( 9 );
    linkingCheck->setFont( linkingCheck_font ); 
    linkingCheck->setChecked( TRUE );

    mateTypeBoxLayout->addWidget( linkingCheck, 4, 0 );

    compressedCheck = new QCheckBox( mateTypeBox, "compressedCheck" );
    QFont compressedCheck_font(  compressedCheck->font() );
    compressedCheck_font.setPointSize( 9 );
    compressedCheck->setFont( compressedCheck_font ); 
    compressedCheck->setChecked( TRUE );

    mateTypeBoxLayout->addWidget( compressedCheck, 2, 0 );

    happyCheck = new QCheckBox( mateTypeBox, "happyCheck" );
    happyCheck->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QFont happyCheck_font(  happyCheck->font() );
    happyCheck_font.setPointSize( 9 );
    happyCheck->setFont( happyCheck_font ); 
    happyCheck->setChecked( TRUE );

    mateTypeBoxLayout->addWidget( happyCheck, 0, 0 );

    orientationCheck = new QCheckBox( mateTypeBox, "orientationCheck" );
    QFont orientationCheck_font(  orientationCheck->font() );
    orientationCheck_font.setPointSize( 9 );
    orientationCheck->setFont( orientationCheck_font ); 
    orientationCheck->setChecked( TRUE );

    mateTypeBoxLayout->addWidget( orientationCheck, 3, 0 );

    happyLabel = new QLabel( mateTypeBox, "happyLabel" );
    happyLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, happyLabel->sizePolicy().hasHeightForWidth() ) );

    mateTypeBoxLayout->addWidget( happyLabel, 0, 1 );

    stretchedLabel = new QLabel( mateTypeBox, "stretchedLabel" );
    stretchedLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, stretchedLabel->sizePolicy().hasHeightForWidth() ) );

    mateTypeBoxLayout->addWidget( stretchedLabel, 1, 1 );

    compressedLabel = new QLabel( mateTypeBox, "compressedLabel" );
    compressedLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, compressedLabel->sizePolicy().hasHeightForWidth() ) );

    mateTypeBoxLayout->addWidget( compressedLabel, 2, 1 );

    linkingLabel = new QLabel( mateTypeBox, "linkingLabel" );
    linkingLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, linkingLabel->sizePolicy().hasHeightForWidth() ) );

    mateTypeBoxLayout->addWidget( linkingLabel, 4, 1 );

    orientationLabel = new QLabel( mateTypeBox, "orientationLabel" );
    orientationLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, orientationLabel->sizePolicy().hasHeightForWidth() ) );

    mateTypeBoxLayout->addWidget( orientationLabel, 3, 1 );

    singletonLabel = new QLabel( mateTypeBox, "singletonLabel" );
    singletonLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, singletonLabel->sizePolicy().hasHeightForWidth() ) );

    mateTypeBoxLayout->addWidget( singletonLabel, 5, 1 );

    unmatedLabel = new QLabel( mateTypeBox, "unmatedLabel" );
    unmatedLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, unmatedLabel->sizePolicy().hasHeightForWidth() ) );

    mateTypeBoxLayout->addWidget( unmatedLabel, 6, 1 );

    queryBoxLayout->addMultiCellWidget( mateTypeBox, 3, 3, 0, 1 );

    featureBox = new QGroupBox( queryBox, "featureBox" );
    featureBox->setColumnLayout(0, Qt::Vertical );
    featureBox->layout()->setSpacing( 6 );
    featureBox->layout()->setMargin( 11 );
    featureBoxLayout = new QGridLayout( featureBox->layout() );
    featureBoxLayout->setAlignment( Qt::AlignTop );

    noneLabel = new QLabel( featureBox, "noneLabel" );
    QFont noneLabel_font(  noneLabel->font() );
    noneLabel_font.setPointSize( 9 );
    noneLabel->setFont( noneLabel_font ); 
    noneLabel->setAlignment( int( QLabel::AlignBottom | QLabel::AlignLeft ) );

    featureBoxLayout->addWidget( noneLabel, 0, 1 );

    allLabel = new QLabel( featureBox, "allLabel" );
    QFont allLabel_font(  allLabel->font() );
    allLabel_font.setPointSize( 9 );
    allLabel->setFont( allLabel_font ); 
    allLabel->setAlignment( int( QLabel::AlignBottom | QLabel::AlignRight ) );

    featureBoxLayout->addWidget( allLabel, 0, 2 );

    insertSlider = new QSlider( featureBox, "insertSlider" );
    insertSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, insertSlider->sizePolicy().hasHeightForWidth() ) );
    insertSlider->setMinValue( 0 );
    insertSlider->setMaxValue( 10 );
    insertSlider->setValue( 10 );
    insertSlider->setOrientation( QSlider::Horizontal );
    insertSlider->setTickmarks( QSlider::NoMarks );
    insertSlider->setTickInterval( 1 );

    featureBoxLayout->addMultiCellWidget( insertSlider, 1, 1, 1, 2 );

    readSlider = new QSlider( featureBox, "readSlider" );
    readSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, readSlider->sizePolicy().hasHeightForWidth() ) );
    readSlider->setMinValue( 0 );
    readSlider->setMaxValue( 10 );
    readSlider->setValue( 10 );
    readSlider->setOrientation( QSlider::Horizontal );
    readSlider->setTickmarks( QSlider::NoMarks );
    readSlider->setTickInterval( 1 );

    featureBoxLayout->addMultiCellWidget( readSlider, 2, 2, 1, 2 );

    snpSlider = new QSlider( featureBox, "snpSlider" );
    snpSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, snpSlider->sizePolicy().hasHeightForWidth() ) );
    snpSlider->setMinValue( 0 );
    snpSlider->setMaxValue( 10 );
    snpSlider->setValue( 10 );
    snpSlider->setOrientation( QSlider::Horizontal );
    snpSlider->setTickmarks( QSlider::NoMarks );
    snpSlider->setTickInterval( 1 );

    featureBoxLayout->addMultiCellWidget( snpSlider, 3, 3, 1, 2 );

    surrogateSlider = new QSlider( featureBox, "surrogateSlider" );
    surrogateSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, surrogateSlider->sizePolicy().hasHeightForWidth() ) );
    surrogateSlider->setMinValue( 0 );
    surrogateSlider->setMaxValue( 10 );
    surrogateSlider->setValue( 10 );
    surrogateSlider->setOrientation( QSlider::Horizontal );
    surrogateSlider->setTickmarks( QSlider::NoMarks );
    surrogateSlider->setTickInterval( 1 );

    featureBoxLayout->addMultiCellWidget( surrogateSlider, 4, 4, 1, 2 );

    surrogateCheck = new QCheckBox( featureBox, "surrogateCheck" );
    QFont surrogateCheck_font(  surrogateCheck->font() );
    surrogateCheck_font.setPointSize( 9 );
    surrogateCheck->setFont( surrogateCheck_font ); 
    surrogateCheck->setChecked( TRUE );

    featureBoxLayout->addWidget( surrogateCheck, 4, 0 );

    snpCheck = new QCheckBox( featureBox, "snpCheck" );
    QFont snpCheck_font(  snpCheck->font() );
    snpCheck_font.setPointSize( 9 );
    snpCheck->setFont( snpCheck_font ); 
    snpCheck->setChecked( TRUE );

    featureBoxLayout->addWidget( snpCheck, 3, 0 );

    readCheck = new QCheckBox( featureBox, "readCheck" );
    QFont readCheck_font(  readCheck->font() );
    readCheck_font.setPointSize( 9 );
    readCheck->setFont( readCheck_font ); 
    readCheck->setChecked( TRUE );

    featureBoxLayout->addWidget( readCheck, 2, 0 );

    insertCheck = new QCheckBox( featureBox, "insertCheck" );
    QFont insertCheck_font(  insertCheck->font() );
    insertCheck_font.setPointSize( 9 );
    insertCheck->setFont( insertCheck_font ); 
    insertCheck->setChecked( TRUE );

    featureBoxLayout->addWidget( insertCheck, 1, 0 );

    qcCheck = new QCheckBox( featureBox, "qcCheck" );
    QFont qcCheck_font(  qcCheck->font() );
    qcCheck_font.setPointSize( 9 );
    qcCheck->setFont( qcCheck_font ); 
    qcCheck->setChecked( TRUE );

    featureBoxLayout->addWidget( qcCheck, 5, 0 );

    qcSlider = new QSlider( featureBox, "qcSlider" );
    qcSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, qcSlider->sizePolicy().hasHeightForWidth() ) );
    qcSlider->setMinValue( 0 );
    qcSlider->setMaxValue( 10 );
    qcSlider->setValue( 10 );
    qcSlider->setOrientation( QSlider::Horizontal );
    qcSlider->setTickmarks( QSlider::NoMarks );
    qcSlider->setTickInterval( 1 );

    featureBoxLayout->addMultiCellWidget( qcSlider, 5, 5, 1, 2 );

    breakSlider = new QSlider( featureBox, "breakSlider" );
    breakSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0, breakSlider->sizePolicy().hasHeightForWidth() ) );
    breakSlider->setMinValue( 0 );
    breakSlider->setMaxValue( 10 );
    breakSlider->setValue( 10 );
    breakSlider->setOrientation( QSlider::Horizontal );
    breakSlider->setTickmarks( QSlider::NoMarks );
    breakSlider->setTickInterval( 1 );

    featureBoxLayout->addMultiCellWidget( breakSlider, 6, 6, 1, 2 );

    breakCheck = new QCheckBox( featureBox, "breakCheck" );
    QFont breakCheck_font(  breakCheck->font() );
    breakCheck_font.setPointSize( 9 );
    breakCheck->setFont( breakCheck_font ); 
    breakCheck->setChecked( TRUE );

    featureBoxLayout->addWidget( breakCheck, 6, 0 );

    otherCheck = new QCheckBox( featureBox, "otherCheck" );
    QFont otherCheck_font(  otherCheck->font() );
    otherCheck_font.setPointSize( 9 );
    otherCheck->setFont( otherCheck_font ); 
    otherCheck->setChecked( FALSE );

    featureBoxLayout->addWidget( otherCheck, 7, 0 );

    queryBoxLayout->addMultiCellWidget( featureBox, 2, 2, 0, 1 );

    displayBox = new QGroupBox( queryBox, "displayBox" );
    displayBox->setColumnLayout(0, Qt::Vertical );
    displayBox->layout()->setSpacing( 6 );
    displayBox->layout()->setMargin( 11 );
    displayBoxLayout = new QVBoxLayout( displayBox->layout() );
    displayBoxLayout->setAlignment( Qt::AlignTop );

    coverageCheck = new QCheckBox( displayBox, "coverageCheck" );
    coverageCheck->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QFont coverageCheck_font(  coverageCheck->font() );
    coverageCheck_font.setPointSize( 9 );
    coverageCheck->setFont( coverageCheck_font ); 
    coverageCheck->setChecked( TRUE );
    displayBoxLayout->addWidget( coverageCheck );

    ceCheck = new QCheckBox( displayBox, "ceCheck" );
    ceCheck->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QFont ceCheck_font(  ceCheck->font() );
    ceCheck_font.setPointSize( 9 );
    ceCheck->setFont( ceCheck_font ); 
    ceCheck->setChecked( FALSE );
    displayBoxLayout->addWidget( ceCheck );

    mateCheck = new QCheckBox( displayBox, "mateCheck" );
    mateCheck->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QFont mateCheck_font(  mateCheck->font() );
    mateCheck_font.setPointSize( 9 );
    mateCheck->setFont( mateCheck_font ); 
    mateCheck->setChecked( TRUE );
    displayBoxLayout->addWidget( mateCheck );

    partitionCheck = new QCheckBox( displayBox, "partitionCheck" );
    partitionCheck->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QFont partitionCheck_font(  partitionCheck->font() );
    partitionCheck_font.setPointSize( 9 );
    partitionCheck->setFont( partitionCheck_font ); 
    partitionCheck->setChecked( TRUE );
    displayBoxLayout->addWidget( partitionCheck );

    tintCheck = new QCheckBox( displayBox, "tintCheck" );
    tintCheck->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    QFont tintCheck_font(  tintCheck->font() );
    tintCheck_font.setPointSize( 9 );
    tintCheck->setFont( tintCheck_font ); 
    tintCheck->setChecked( FALSE );
    displayBoxLayout->addWidget( tintCheck );

    queryBoxLayout->addWidget( displayBox, 4, 0 );

    mateColorBox = new QButtonGroup( queryBox, "mateColorBox" );
    mateColorBox->setColumnLayout(0, Qt::Vertical );
    mateColorBox->layout()->setSpacing( 6 );
    mateColorBox->layout()->setMargin( 11 );
    mateColorBoxLayout = new QVBoxLayout( mateColorBox->layout() );
    mateColorBoxLayout->setAlignment( Qt::AlignTop );

    categoricalButton = new QRadioButton( mateColorBox, "categoricalButton" );
    QFont categoricalButton_font(  categoricalButton->font() );
    categoricalButton_font.setPointSize( 9 );
    categoricalButton->setFont( categoricalButton_font ); 
    categoricalButton->setChecked( TRUE );
    mateColorBoxLayout->addWidget( categoricalButton );

    continuousButton = new QRadioButton( mateColorBox, "continuousButton" );
    QFont continuousButton_font(  continuousButton->font() );
    continuousButton_font.setPointSize( 9 );
    continuousButton->setFont( continuousButton_font ); 
    continuousButton->setChecked( FALSE );
    mateColorBoxLayout->addWidget( continuousButton );

    linkingButton = new QRadioButton( mateColorBox, "linkingButton" );
    QFont linkingButton_font(  linkingButton->font() );
    linkingButton_font.setPointSize( 9 );
    linkingButton->setFont( linkingButton_font ); 
    mateColorBoxLayout->addWidget( linkingButton );

    libraryButton = new QRadioButton( mateColorBox, "libraryButton" );
    QFont libraryButton_font(  libraryButton->font() );
    libraryButton_font.setPointSize( 9 );
    libraryButton->setFont( libraryButton_font ); 
    mateColorBoxLayout->addWidget( libraryButton );

    queryBoxLayout->addWidget( mateColorBox, 4, 1 );
    QueryWidgetLayout->addWidget( queryBox );
    languageChange();
    resize( QSize(273, 682).expandedTo(minimumSizeHint()) );
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
    searchButton->setText( tr( "Search" ) );
    happyEdit->setText( tr( "2.0" ) );
    happyButton->setText( tr( "Happy Distance" ) );
    mateTypeBox->setTitle( tr( "Mate Types" ) );
    singletonCheck->setText( tr( "Singleton" ) );
    unmatedCheck->setText( tr( "Unmated" ) );
    stretchedCheck->setText( tr( "Stretched" ) );
    linkingCheck->setText( tr( "Linking" ) );
    compressedCheck->setText( tr( "Compressed" ) );
    happyCheck->setText( tr( "Happy" ) );
    orientationCheck->setText( tr( "Orientation" ) );
    happyLabel->setText( tr( "pixmap" ) );
    stretchedLabel->setText( tr( "pixmap" ) );
    compressedLabel->setText( tr( "pixmap" ) );
    linkingLabel->setText( tr( "pixmap" ) );
    orientationLabel->setText( tr( "pixmap" ) );
    singletonLabel->setText( tr( "pixmap" ) );
    unmatedLabel->setText( tr( "pixmap" ) );
    featureBox->setTitle( tr( "Features" ) );
    noneLabel->setText( tr( "NONE" ) );
    allLabel->setText( tr( "ALL" ) );
    surrogateCheck->setText( tr( "Surrogate" ) );
    snpCheck->setText( tr( "SNPs" ) );
    readCheck->setText( tr( "Read Cov" ) );
    insertCheck->setText( tr( "Insert Cov" ) );
    qcCheck->setText( tr( "AsmQC" ) );
    breakCheck->setText( tr( "Breaks" ) );
    otherCheck->setText( tr( "Other" ) );
    displayBox->setTitle( tr( "Display" ) );
    coverageCheck->setText( tr( "Coverage Stat" ) );
    ceCheck->setText( tr( "CE Statistic" ) );
    mateCheck->setText( tr( "Connect Mates" ) );
    partitionCheck->setText( tr( "Partition Types" ) );
    tintCheck->setText( tr( "Tint Partitions" ) );
    mateColorBox->setTitle( tr( "Mate Colors" ) );
    categoricalButton->setText( tr( "Categorical" ) );
    continuousButton->setText( tr( "Continuous" ) );
    linkingButton->setText( tr( "Linking" ) );
    libraryButton->setText( tr( "Library" ) );
}

