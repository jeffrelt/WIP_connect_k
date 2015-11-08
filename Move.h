#ifndef MOVE_H
#define MOVE_H

#pragma once
#include <iostream>

class Move
{
public:
    Move() {}
    Move(int x, int y)
    {
        _col=x;
        _row=y;
    }
    Move(const Move& other)
    {
        _col=other._col;
        _row=other._row;
    }
    Move move(int x, int y)
    {
        Move return_this(*this);
        return_this._col+=x;
        return_this._row+=y;
        return return_this;
    }
    int getCol()const
    {
        return _col;
    }
    int getRow()const
    {
        return _row;
    }
    void setCol(int col)
    {
        _col = col;
    }
    void setRow(int row)
    {
        _row = row;
    }
    bool operator !=(const Move& other)
    {
        return (_col != other._col ||_row != other._row);
    }
    friend
    std::istream& operator >> (std::istream& in, Move& move)
    {
        uint8_t hold;
        in>>hold;
        move._col = hold;
        in>>hold;
        move._row = hold;
        return in;
    }
    friend
    std::ostream& operator << (std::ostream& out, const Move& move)
    {
        int hold = move.getCol();
        out<<hold<<" ";
        hold=move.getRow();
        out<<hold;
        return out;
    }
    
    
private:
// data - just 1 byte
    
    int _row;
    int _col;

    
};

#endif //MOVE_H

