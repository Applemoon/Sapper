#include "tcell.h"
#include <QTimer>



TCell :: TCell( uint ix, uint iy ) : x( ix ), y( iy ), hasBomb( false ), state( 0 ), neigBombs( 0 )
{}
