//
//  GameBoard.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 10/11/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#ifndef GameBoard_hpp
#define GameBoard_hpp

#include <stdio.h>
#include <array>
#include "Move.h"
#include "GameNode.hpp"

enum cellType{ EMPTY=0, BOUNDRY=1, US=2, ENEMY=3 };



struct Cell{
    const Cell operator = (const cellType who)
    {
        _cell = who;
        return *this;
    }
    operator const cellType()
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
private:
    char _cell;
};

class GameBoard{
public:
    GameBoard operator = (GameBoard& other)
    {
        _board = other._board;
        return *this;
    }
    void setBoard(int col, int row)
    {
        //Wipe the board
        //set the boundies when less than 16x16
        for(int i = 0; i<col; ++i)
            for(int j = 0; j<row; ++j)
                _board[col][row]=cellType::EMPTY;
    }
    bool addMove(Move where, cellType who)
    {
        int col = where.getCol();
        int row = where.getRow();
        if(_board[col][row]) //not empty
            return false;
        _board[col][row]=who;
        return true;
    }
    bool addMove(int col, int row, cellType who)
    {
        if(_board[col][row]) //not empty
            return false;
        _board[col][row]=who;
        return true;
    }
    bool removeMove(Move where)
    {
        int col = where.getCol();
        int row = where.getRow();
        if(_board[col][row] <= cellType::BOUNDRY) //not empty
            return false;
        _board[col][row]=cellType::EMPTY;
        return true;
    }
    const std::array<Cell,16>& operator[](int col)const
    {
        return _board[col];
    }
    GameNode* getPossibleMoves(int num_col, int num_row, bool best_only = false)
    {
        GameNode* root = nullptr;
        GameNode* hold;
        for (int col = 0; col<num_col; col++){
            for (int row = 0; row<num_row; row++){
                if (_board[col][row] == cellType::EMPTY){
                    hold = root;
                    // NOTE: here we create a GameNode - this will change later
                    // it is the callers responsibility to delete it
                    root = new GameNode;
                    root->my_move=Move(col,row);
                    root->next=hold;
                }
            }
        }
        return root;
    }

private:
    std::array<std::array<Cell,16>,16> _board;
};

#endif //GameBoard_hpp