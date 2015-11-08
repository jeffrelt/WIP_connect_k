#ifndef AISHELL_H
#define AISHELL_H

#include <chrono>
#include <thread>
#include <utility>
#include <atomic>
#include <climits>
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
    void setBoard(bool gravity, int num_col,int num_row,int k, Move last_move)
    {
        _gravity = gravity;
        _num_col = num_col;
        _num_row = num_row;
        _k = k;
        _move_count = 0;
        _run = true;
        _game.setBoard(num_col,num_row);
        if(last_move.getCol() != -1)
        {
            D(std::cout<<_name<<": Enemy moved "<<last_move<<std::endl;)
            _game.addMove(last_move, cellType::ENEMY);
        }
        _boardPopulated();
        //_builder = new std::thread(&AIShell::_buildGameTree,this);
    }
    void enemyMove(Move their_move)
    {
        //_run = false;
        _move = their_move;
        _move_count++;
        _game.addMove(_move, cellType::ENEMY);
    }
    Move makeMove(int deadline)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(deadline));
        for(int i = 1; i<=4; ++i)
            _logic(i);
        //_run = false;
        _move_count++;
        return Move(_move);
    }
    
protected:
    virtual void _boardPopulated()
    {
        // for the children...
    }
    virtual void _logic(int target_depth)
    {
        if(target_depth == 1)
        {
            for (int col = 0; col<_num_col; col++)
                for (int row = 0; row<_num_row; row++)
                    if (_game[col][row] == cellType::EMPTY)
                    {
                        _move = Move(col, row);
                        return;
                    }
        }
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    virtual void _cleanTree()
    {
        // for the children...
    }
    void _buildGameTree()
    {
        unsigned int depth = 1;
        while(1)
        {
            _logic(depth);
            if(_run)
                depth++;
            else
            {
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
                _cleanTree();
                _run = true;
            }
        }
    }
    
    std::string _name;
    std::thread* _builder;
    bool _gravity;
    int _num_col;
    int _num_row;
    int _k;
    GameBoard _game;
    Move _move;
    std::atomic<bool> _run;
    std::atomic<int> _move_count;
};

#endif //AISHELL_H
