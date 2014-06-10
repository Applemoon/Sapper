#ifndef FIELDOPTIONSDIALOG_H
#define FIELDOPTIONSDIALOG_H

#include <QDialog>
#include <mainwindow.h>



namespace Ui
{
    class fieldOptionsDialog;
}



class fieldOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit fieldOptionsDialog( QWidget *parent = 0, MainWindow *gW = 0 );
    ~fieldOptionsDialog();

public slots:
    void setManualOptionsEnabled();
    void refreshBombsCount();

signals:
    void sendOptions( uint w, uint h, uint b );

private slots:
    void on_pushButtonApply_clicked();
    void on_pushButtonExit_clicked();

private:
    Ui::fieldOptionsDialog *ui;
    MainWindow *gameWindow;

    int generateCode( int w, int h, int b );
};



#endif // FIELDOPTIONSDIALOG_H
