#ifndef AISHELL_H
#define AISHELL_H

#include <chrono>
#include <thread>
#include <utility>
#include <atomic>
#include "GameBoard.hpp"
#include "GameNode.cpp"

#define DEBUG_ON

#ifdef DEBUG_ON
#define D(x) x
#else
#define D(x)
#endif


// A new AIShell will be created for every move request.
class AIShell{

public:
    AIShell()
    {
        _name = "WIP";
        _builder=nullptr;
    }
    AIShell(const char* name)
    {
        _name = name;
        _builder=nullptr;
    }
    ~AIShell()
    {
        if(_builder)
            delete _builder;
    }
    void setBoard(bool gravity, uint8_t num_col,uint8_t num_row,uint8_t k, Move last_move)
    {
        _gravity = gravity;
        _num_col = num_col;
        _num_row = num_row;
        _k = k;
        _move_count = 0;
        _game.setBoard(num_col,num_row);
        if(last_move.getCol() < 15)
        {
            D(std::cout<<_name<<": Enemy moved "<<last_move<<std::endl;)
            _game.addMove(last_move, cellType::ENEMY);
        }
        _builder = new std::thread(&AIShell::_buildGameTree,this);
    }
    void enemyMove(Move their_move)
    {
        _move = their_move;
        _move_count++;
    }
    Move makeMove(int deadline)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(deadline));
        _move_count++;
        return Move(_move);
    }
    
protected:
    virtual void _logic(int depth)
    {
        if(depth == 1)
        {
            for (int col = 0; col<_num_col; col++)
            {
                for (int row = 0; row<_num_row; row++)
                {
                    if (_game[col][row] == cellType::EMPTY)
                    {
                        _move = Move(col, row);
                        return;
                    }
                }
            }
        }
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    void _buildGameTree()
    {
        unsigned int count = _move_count;
        unsigned int depth = 1;
        while(1)
        {
            _logic(depth);
            if(count == _move_count)
                depth++;
            else
            {
                count = _move_count;
                Move m(_move);
                depth = 1;
                if(_move_count & 1)
                {
                    _game.addMove(m, cellType::US);
                    D(std::cout<<_name<<": I moved "<<m<<std::endl;)
                }
                else
                {
                    _game.addMove(m, cellType::ENEMY);
                    D(std::cout<<_name<<": Enemy moved "<<m<<std::endl;)
                }
            }
        }
    }
    
    std::string _name;
    std::thread* _builder;
    bool _gravity;
    uint8_t _num_col;
    uint8_t _num_row;
    uint8_t _k;
    uint8_t _status;
    GameBoard _game;
    uint8_t _move;
    std::atomic<unsigned int> _move_count;
};

#endif //AISHELL_H
