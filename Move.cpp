#include "Move.h"


Move::Move() : _data(0)
{}

Move::Move(int col, int row){
    set(row, col);
}

Move Move::operator = (Move other)
{
    _data = other._data;
    return *this;
}

int Move::getRow()
{
    return _data>>4;
}

int Move::getCol()
{
    return 0x0f & _data;
}

inline
void Move::set(int row, int col)
{
    _data = row<<4 | col;
}

inline
void Move::setRow(int row)
{
    _data &= 0x0f;
    _data |= row<<4;
}

inline
void Move::setCol(int col)
{
    _data &= 0xf0;
    _data |= col;
}

