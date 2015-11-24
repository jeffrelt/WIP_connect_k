//
//  GameBoard.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 10/11/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//
#include <stdio.h>
#include "Move.h"
#include "GameNode.h"


struct Cell{
    
    const Cell operator = (const cellType who)
    {
        _cell = who;
        return *this;
    }
    operator const cellType()const
    {
        return (cellType)_cell;
    }
    friend
    bool operator == (Cell lhs, cellType rhs)
    {
        return (cellType)lhs._cell == rhs;
    }
    friend
    bool operator <= (Cell lhs, cellType rhs)
    {
        return (cellType)lhs._cell <= rhs;
    }
    friend
    std::ostream& operator << (std::ostream& out, const Cell& cell)
    {
        if( cell == EMPTY)
            out<<' ';
        else if( cell == US)
            out<<'X';
        else
            out<<'O';
        return out;
    }
private:
    char _cell;
};

class GameBoard{
public:
    
    void setBoard(int col, int row)
    {
        //Wipe the board
        //set the boundies when less than 16x16
        for(int i = 0; i<col; ++i)
            for(int j = 0; j<row; ++j)
                _board[i][j]=EMPTY;
    }
    bool addMove(Move where, cellType who)
    {
        int col = where.getCol();
        int row = where.getRow();
        if(_board[col][row]) //not empty
            throw "bad move";
        _board[col][row]=who;
        return true;
    }
    bool addMove(int col, int row, cellType who)
    {
        if(_board[col][row]) //not empty
            throw "bad move";
        _board[col][row]=who;
        return true;
    }
    bool removeMove(Move where)
    {
        int col = where.getCol();
        int row = where.getRow();
        if(_board[col][row] <= BOUNDRY) //not empty
            throw "bad move";
        _board[col][row]= EMPTY;
        return true;
    }
    const Cell* operator[](int col)const
    {
        return _board[col];
    }

private:
    Cell _board[10][10];
};
