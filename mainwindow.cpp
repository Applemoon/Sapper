#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fieldoptionsdialog.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include <math.h>
//#include <windows.h>
#include <assert.h>

#define DEFAULT_BOARD_SIZE 16
#define DEFAULT_BOMBS_NUMBER 40
#define CELL_SIZE 20



MainWindow :: MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ), cellSize( CELL_SIZE )
{
    ui->setupUi( this );
}



MainWindow :: ~MainWindow()
{
    delete ui;
    field.clear();
}



void MainWindow :: newGame()
{
    field.clear();
    fieldOptionsDialog fod( this, this );
    int result = fod.exec();

    if ( result == 0 )
    {
        //show();
        //close();
    }
    else
    {
       setFixedSize( xCells*cellSize, yCells*cellSize );
       for ( uint i = 0; i < xCells; i++ )
       {
           QVector<TCell> tempVector;
           for ( uint j = 0; j < yCells; j++ )
           {
               uint x = i*cellSize;
               uint y = j*cellSize;
               TCell cell( x, y );
               tempVector.push_back( cell );
           }
           field.push_back( tempVector );
       }

       gameOverShown = false;
       clickedYet = false;
       bombed = false;
       bombsCount = bombs;
       statusBar()->showMessage( QString::number( bombsCount ) );
       show();
    }
}



void MainWindow :: setBombs( uint xCell, uint yCell )
{
    if ( bombs >= xCells*yCells-9 )
        assert( false && "To many bombs." );

    uint x;
    uint y;
    for ( uint i = 0; i < bombs; i++ )
    {
        do
        {
            QTime time = QTime::currentTime();
            srand( (uint)time.msec() );
            x = rand() % xCells;
            y = rand() % yCells;
        }
        while( field[x][y].hasBomb || ( ( abs( x-xCell ) <=1 ) && ( abs( y-yCell ) <=1 ) ) );
        field[x][y].hasBomb = true;
    }
}



void MainWindow :: paintEvent( QPaintEvent * )
{
    for ( int i = 0; i < field.size(); i++ )
        for ( int j = 0; j < field[i].size(); j++ )
            drawCell( field[i][j] );
}



void MainWindow :: drawCell( TCell cell )
{
    QPainter painter;
    painter.begin( this );
    switch( cell.state )
    {
    case( 0 ):
        {
            painter.setPen( QPen( Qt::lightGray, 2 ) );
            painter.setBrush( Qt::gray );
            painter.drawRect( cell.x, cell.y, cellSize, cellSize );
            break;
        }
    case( 1 ):
        {
            painter.setPen( QPen( Qt::lightGray, 2 ) );
            painter.setBrush( Qt::white );
            painter.drawRect( cell.x, cell.y, cellSize, cellSize );

            int xCell = cell.x / cellSize;
            int yCell = cell.y / cellSize;

            painter.setFont( QFont( "Arial", 12, QFont::Bold ) );
            switch ( field[xCell][yCell].neigBombs )
            {
            case( 0 ):
                {
                    painter.setPen( QPen( Qt::white, 2 ) );
                    break;
                }
            case( 1 ):
                {
                    painter.setPen( QPen( Qt::blue, 2 ) );
                    break;
                }
            case( 2 ):
                {
                    painter.setPen( QPen( Qt::darkGreen, 2 ) );
                    break;
                }
            case( 3 ):
                {
                    painter.setPen( QPen( Qt::red, 2 ) );
                    break;
                }
            case( 4 ):
                {
                    painter.setPen( QPen( Qt::darkBlue, 2 ) );
                    break;
                }
            case( 5 ):
                {
                    painter.setPen( QPen( Qt::darkRed, 2 ) );
                    break;
                }
            case( 6 ):
                {
                    painter.setPen( QPen( Qt::green, 2 ) );
                    break;
                }
            case( 7 ):
                {
                    painter.setPen( QPen( Qt::darkRed, 2 ) );
                    break;
                }
            case( 8 ):
                {
                    painter.setPen( QPen( Qt::darkRed, 2 ) );
                    break;
                }
            default:
                {
                    assert( false && "Unexpected number in cell." );
                }
            }
            QRect cellRect( cell.x, cell.y, cellSize, cellSize);
            painter.drawText( cellRect , Qt::AlignCenter, QString::number( field[xCell][yCell].neigBombs ) );
            break;
        }
    case( 2 ):
        {
            painter.setPen( QPen( Qt::lightGray, 2 ) );
            painter.setBrush( Qt::gray );
            painter.drawRect( cell.x, cell.y, cellSize, cellSize );

            QImage flag( ":/pics/flag.png" );
            int x = cell.x + cellSize/2 - flag.width()/2;
            int y = cell.y + cellSize/2 - flag.height()/2;
            painter.drawImage( x, y, flag );
            break;
        }
    case( 3 ):
        {
            painter.setPen( QPen( Qt::lightGray, 2 ) );
            painter.setBrush( Qt::red );
            painter.drawRect( cell.x, cell.y, cellSize, cellSize );

            QImage flag( ":/pics/bomb.png" );
            int x = cell.x + cellSize/2 - flag.width()/2;
            int y = cell.y + cellSize/2 - flag.height()/2;
            painter.drawImage( x, y, flag );
            break;
        }
    default:
        {
            assert( false && "Unexpected cell state" );
        }
    }
    painter.end();
}



void MainWindow :: mousePressEvent( QMouseEvent *event )
{
    if ( !bombed )
    {
        uint xCell = event->x() / cellSize;
        uint yCell = event->y() / cellSize;

        if ( !clickedYet )
        {
            setBombs( xCell, yCell );
            clickedYet = true;
        }

        bombed = cellClicked( field[xCell][yCell], event->button() );
        update();
    }
}



void MainWindow :: mouseDoubleClickEvent( QMouseEvent *event )
{
    uint xCell = event->x() / cellSize;
    uint yCell = event->y() / cellSize;

    if ( field[xCell][yCell].state == 1 )
    {
        bool upFree = ( yCell > 0 );
        bool downFree = ( yCell < yCells-1 );
        bool leftFree = ( xCell > 0 );
        bool rightFree = ( xCell < xCells-1 );

        uint flags = 0;

        if ( upFree )
        {
            if ( leftFree )
                if ( field[xCell-1][yCell-1].state == 2 ) flags++;
            if ( field[xCell][yCell-1].state == 2 ) flags++;
            if ( rightFree )
                if ( field[xCell+1][yCell-1].state == 2 ) flags++;
        }
        if ( leftFree )
            if ( field[xCell-1][yCell].state == 2 ) flags++;
        if ( rightFree )
            if ( field[xCell+1][yCell].state == 2 ) flags++;
        if ( downFree )
        {
            if ( leftFree )
                if ( field[xCell-1][yCell+1].state == 2 ) flags++;
            if ( field[xCell][yCell+1].state == 2 ) flags++;
            if ( rightFree )
                if ( field[xCell+1][yCell+1].state == 2 ) flags++;
        }

        if ( flags == field[xCell][yCell].neigBombs )
            clickAround( field[xCell][yCell] );
    }
}



void MainWindow :: gameOver( bool win/* = false*/ )
{
    QMessageBox gameOverBox;
    if ( win )
        gameOverBox.setText( QString::fromLocal8Bit( "ѕобеда!" ) );
    else
        gameOverBox.setText( QString::fromLocal8Bit( "Ѕомба! ¬ы проиграли." ) );
    gameOverBox.exec();
    gameOverShown = true;
    close();
}



bool MainWindow :: cellClicked( TCell &cell, Qt::MouseButton btn )
{
    if ( btn == Qt::LeftButton )
    {
        if ( cell.state == 0 )
        {
            if ( cell.hasBomb )
            {
                cell.state = 3;
                QTimer::singleShot( 500, this, SLOT( gameOver() ) );
                return true;
            }
            else
            {
                cell.state = 1;
                if ( setNeigBombs( cell ) == 0 )
                    clickAround( cell );

                if ( checkWin() )
                    if ( !gameOverShown )
                        gameOver( true );
            }
        }
    }
    else if ( btn == Qt::RightButton )
    {
        if ( cell.state == 0 )
        {
            cell.state = 2;
            bombsCount--;
            statusBar()->showMessage( QString::number( bombsCount ) );
        }
        else if ( cell.state == 2 )
            {
                cell.state = 0;
                bombsCount++;
                statusBar()->showMessage( QString::number( bombsCount ) );
            }
    }

    update();
    return false;
}



void MainWindow :: clickAround( TCell &cell )
{
    uint xCell = cell.x / cellSize;
    uint yCell = cell.y / cellSize;

    bool upFree = ( yCell > 0 );
    bool downFree = ( yCell < yCells-1 );
    bool leftFree = ( xCell > 0 );
    bool rightFree = ( xCell < xCells-1 );

    if ( upFree )
    {
        if ( leftFree && field[xCell-1][yCell-1].state != 2 )
                cellClicked( field[xCell-1][yCell-1], Qt::LeftButton );

        if ( field[xCell][yCell-1].state != 2 )
            cellClicked( field[xCell][yCell-1], Qt::LeftButton );

        if ( rightFree && field[xCell+1][yCell-1].state != 2 )
                cellClicked( field[xCell+1][yCell-1], Qt::LeftButton );
    }

    if ( leftFree && field[xCell-1][yCell].state != 2 )
            cellClicked( field[xCell-1][yCell], Qt::LeftButton );

    if ( rightFree && field[xCell+1][yCell].state != 2 )
            cellClicked( field[xCell+1][yCell], Qt::LeftButton );

    if ( downFree )
    {
        if ( leftFree && field[xCell-1][yCell+1].state != 2 )
                cellClicked( field[xCell-1][yCell+1], Qt::LeftButton );

        if ( field[xCell][yCell+1].state != 2 )
            cellClicked( field[xCell][yCell+1], Qt::LeftButton );

        if ( rightFree && field[xCell+1][yCell+1].state != 2 )
                cellClicked( field[xCell+1][yCell+1], Qt::LeftButton );
    }
}



bool MainWindow :: checkWin()
{
    for ( int i = 0; i < field.size(); i++ )
        for ( int j = 0; j < field[i].size(); j++ )
            if ( !field[i][j].hasBomb &&  field[i][j].state != 1 )
                return false;
    return true;
}



uint MainWindow :: setNeigBombs( TCell &cell )
{
    uint xCell = cell.x / cellSize;
    uint yCell = cell.y / cellSize;

    cell.neigBombs = 0;
    bool upFree = ( yCell > 0 );
    bool downFree = ( yCell < yCells-1 );
    bool leftFree = ( xCell > 0 );
    bool rightFree = ( xCell < xCells-1 );

    if ( upFree )
    {
        if ( leftFree )
            if ( field[xCell-1][yCell-1].hasBomb ) cell.neigBombs++;
        if ( field[xCell][yCell-1].hasBomb ) cell.neigBombs++;
        if ( rightFree )
            if ( field[xCell+1][yCell-1].hasBomb ) cell.neigBombs++;
    }
    if ( leftFree )
        if ( field[xCell-1][yCell].hasBomb ) cell.neigBombs++;
    if ( rightFree )
        if ( field[xCell+1][yCell].hasBomb ) cell.neigBombs++;
    if ( downFree )
    {
        if ( leftFree )
            if ( field[xCell-1][yCell+1].hasBomb ) cell.neigBombs++;
        if ( field[xCell][yCell+1].hasBomb ) cell.neigBombs++;
        if ( rightFree )
            if ( field[xCell+1][yCell+1].hasBomb ) cell.neigBombs++;
    }
    return cell.neigBombs;
}



void MainWindow :: setupField( uint w, uint h, uint b )
{
    xCells = w;
    yCells = h;
    bombs = b;
}




/*
  исправить многократное сообщение при победе и проигрыше
  исправить вылет игры при нажатии "¬ыход" на форме настроек
  исправить рандом на более случайный
  вопрос о новой игре после окончани€
*/
