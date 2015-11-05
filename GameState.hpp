//
//  GameState.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 10/18/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <stdio.h>


/*
class GameState{
public:
    GameBoard(GameBoard& other) : _data(other._data)
    {
        _board = other._board();
    }
    GameBoard(bool gravity, uint8_t num_col, uint8_t num_row, uint8_t k, Move last_move) : _data(0)
    {
        _gravity = gravity;
        _cell_count = num_col*num_row;
        _board_size.setCol(num_col);
        _board_size.setRow(num_row);
        _k = k;
        _board = new Cell[_cell_count];
        if(last_move.getRow() >= 0)
            _setCell(last_move, Cell::ENEMY);
    }
    void makeMove(Move where)
    {
        if(_move_count & 0x1)
            _setCell(where, Cell::ENEMY);
        else
            _setCell(where, Cell::US);
        ++_move_count;
    }
    void undoMove(Move where)
    {
        _setCell(where, Cell::EMPTY);
        --_move_count;
    }
    Cell& getCell(Move where)
    {
        return getCell(where.getCol(), where.getRow());
    }
    Cell& getCell(uint8_t col, uint8_t row)
    {
        return _board[row[col];
                      }
                      
                      private:
                      void _setCell(Move where, Cell::Type who)
                      {
                      getCell(where) = who;
                      }
                      // data - 16 bytes
                      std::array<Move,14> _board;
                      union{
                      struct{
                      uint8_t _move_count;
                      uint8_t _gravity:1;
                      uint8_t _status:3;
                      uint8_t _k:4;
                      };
                      uint16_t _data;
        };
        };
        */
#endif /* GameBoard_hpp */
        
        
        /*
         each "expanded node" needs
         parent prt
         expected move
         depth
         std::vector of children
         
         each "node" needs
         col, row, score, ptr to expansion,
         
         
         */