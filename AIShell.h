#ifndef AISHELL_H
#define AISHELL_H

#include "GameBoard.hpp"

// A new AIShell will be created for every move request.
class AIShell{

public:
    void setBoard(bool gravity, uint8_t num_col,uint8_t num_row,uint8_t k, Move last_move)
    {
        _gravity = gravity;
        _num_col = num_col;
        _num_row = num_row;
        _k = k;
        _game.setBoard(num_col,num_row);
        if(last_move.getCol() < 15)
            enemyMove(last_move);
    }
    void enemyMove(Move their_move)
    {
        _game.addMove(their_move, cellType::ENEMY);
    }
    Move makeMove(int deadline)
    {
        for (int col = 0; col<_num_col; col++){
            for (int row = 0; row<_num_row; row++){
                if (_game[col][row] == cellType::EMPTY){
                    _game.addMove(col,row, cellType::US);
                    return Move(col, row);
                }
            }
        }
        return Move(0, 0);
    }
    
private:
    bool _gravity;
    uint8_t _num_col;
    uint8_t _num_row;
    uint8_t _k;
    GameBoard _game;
};

#endif //AISHELL_H
