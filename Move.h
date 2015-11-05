#ifndef MOVE_H
#define MOVE_H

#pragma once
#include <cstdint>
#include <iostream>

class Move
{
public:
    Move() {}
    Move(const Move& other)
    {
        _data = other._data;
    }
    Move(uint8_t x, uint8_t y)
    {
        _col=x;
        _row=y;
    }
    Move move(uint8_t x, uint8_t y)
    {
        Move return_this(*this);
        return_this._col+=x;
        return_this._row+=y;
        return return_this;
    }
    uint8_t getCol()const
    {
        return _col;
    }
    uint8_t getRow()const
    {
        return _row;
    }
    void setCol(uint8_t col)
    {
        _col = col;
    }
    void setRow(uint8_t row)
    {
        _row = row;
    }
    bool operator == (const Move& other)const
    {
        return _data == other._data;
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
    union{
        struct{
            uint8_t _row:4;
            uint8_t _col:4;
        };
        uint8_t _data;
    };
    
};

#endif //MOVE_H

