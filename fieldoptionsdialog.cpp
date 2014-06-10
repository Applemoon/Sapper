#include "fieldoptionsdialog.h"
#include "ui_fieldoptionsdialog.h"



fieldOptionsDialog :: fieldOptionsDialog( QWidget *parent, MainWindow *gW ) : QDialog( parent ),
                                          ui( new Ui::fieldOptionsDialog ), gameWindow( gW )
{
    ui->setupUi( this );
    setFixedSize( size() );
    refreshBombsCount();

    connect( ui->radioButtonEasy, SIGNAL( toggled( bool ) ), this, SLOT( setManualOptionsEnabled() ) );
    connect( ui->radioButtonMedium, SIGNAL( toggled( bool ) ), this, SLOT( setManualOptionsEnabled() ) );
    connect( ui->radioButtonHard, SIGNAL( toggled( bool ) ), this, SLOT( setManualOptionsEnabled() ) );
    connect( ui->radioButtonManual, SIGNAL( toggled( bool ) ), this, SLOT( setManualOptionsEnabled() ) );

    connect( ui->spinBoxHeight, SIGNAL( valueChanged( int ) ), this, SLOT( refreshBombsCount() ) );
    connect( ui->spinBoxWidth, SIGNAL( valueChanged( int ) ), this, SLOT( refreshBombsCount() ) );

    connect( this, SIGNAL( sendOptions( uint, uint, uint ) ), gameWindow, SLOT( setupField( uint, uint, uint ) ) );
}



fieldOptionsDialog :: ~fieldOptionsDialog()
{
    delete ui;
}



void fieldOptionsDialog::on_pushButtonApply_clicked()
{
    int w;
    int h;
    int b;
    if ( ui->radioButtonEasy->isChecked() )
    {
        w = 9;
        h = 9;
        b = 10;
    }
    else if ( ui->radioButtonMedium->isChecked() )
    {
        w = 16;
        h = 16;
        b = 40;
    }
    else if ( ui->radioButtonHard->isChecked() )
    {
        w = 30;
        h = 16;
        b = 99;
    }
    else if ( ui->radioButtonManual->isChecked() )
    {
        w = ui->spinBoxWidth->value();
        h = ui->spinBoxHeight->value();
        b = ui->spinBoxBombs->value();
    }
    emit sendOptions( w, h, b );
    accept();
}



void fieldOptionsDialog::on_pushButtonExit_clicked()
{
    reject();
}



void fieldOptionsDialog :: setManualOptionsEnabled()
{
    bool manual = ( sender() == ui->radioButtonManual );
    ui->spinBoxWidth->setEnabled( manual );
    ui->spinBoxHeight->setEnabled( manual );
    ui->spinBoxBombs->setEnabled( manual );
}



void fieldOptionsDialog :: refreshBombsCount()
{
    int w = ui->spinBoxWidth->value();
    int h = ui->spinBoxHeight->value();
    ui->spinBoxBombs->setMaximum( h*w-9 );
}
