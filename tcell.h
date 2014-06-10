#ifndef TCELL_H
#define TCELL_H

#include <QRect>
#include <QWidget>



class TCell
{
public:
    TCell( uint ix = 0, uint iy = 0 );
    uint x;
    uint y;
    bool hasBomb;
    uint state; // 0 - closed, 1 - opened and empty, 2 - flag, 3 - opened with bomb
    uint neigBombs;
};



#endif // TCELL_H
