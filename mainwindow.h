#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tcell.h>



namespace Ui {
    class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();
    void newGame();

    bool bombed;

private:
    Ui::MainWindow *ui;

    QVector< QVector<TCell> > field;
    uint xCells;
    uint yCells;
    uint bombs;
    uint cellSize;
    uint bombsCount;
    bool clickedYet;
    bool gameOverShown;

    void setBombs( uint xCell, uint yCell );
    void paintEvent( QPaintEvent * );
    void drawCell( TCell cell );
    void mousePressEvent( QMouseEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );
    bool cellClicked( TCell &cell, Qt::MouseButton btn );
    void clickAround( TCell &cell );
    bool checkWin();
    uint setNeigBombs( TCell &cell );

private slots:
    void setupField( uint w, uint h, uint b );
    void gameOver( bool win = false );
};



#endif // MAINWINDOW_H
