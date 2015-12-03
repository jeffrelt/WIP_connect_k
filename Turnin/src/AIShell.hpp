#ifndef AISHELL_H
#define AISHELL_H

#include <cmath>
#include <unistd.h>
#include <climits>
#include <sstream>
#include "GameBoard.hpp"
#include "GameNode.hpp"
#include <cmath>

//#define DEBUG_ON
//#define SINGLE_THREAD

#ifdef DEBUG_ON
#define SINGLE_THREAD
#define D(x) x
#else
#define D(x)
#endif


// A new AIShell will be created for every move request.
class AIShell {

public:
    AIShell()
    {
        name = "WIP";
    }
    AIShell(const char* my_name)
    {
        name = my_name;
    }
    ~AIShell()
    {
    }

    void setBoard(bool gravity, int num_col, int num_row, int k, Move last_move)
    {
        _gravity = gravity;
        _num_col = num_col;
        _num_row = num_row;
        _k = k;
        _move_count = 0;
        _run = true;
        _game.setBoard(num_col, num_row);
        if (last_move.getCol() != -1)
        {
            D(out << name << ": Enemy moved " << last_move << std::endl;)
            _game.addMove(last_move, ENEMY);
        }
        _boardPopulated();
#ifndef SINGLE_THREAD
        pthread_mutex_init(&_building, NULL);
        pthread_mutex_init(&_m, NULL);
        pthread_create(&_builder, NULL, _buildGameTree, this);
#endif
    }


    void enemyMove(Move their_move)
    {
#ifdef SINGLE_THREAD
        _game.addMove(their_move, ENEMY);
#else
        pthread_mutex_lock( &_m );
        _run = false;
        _move = their_move;
        _move_count++;
        pthread_mutex_lock( &_building );
        pthread_mutex_unlock( &_building );
        pthread_mutex_unlock( &_m );
#endif
    }

    Move makeMove(int deadline)
    {
        Move return_move;
#ifdef SINGLE_THREAD
        _cleanTree();
        int depth = _gravity ? 8 : 3;
        for (int i = 1; i <= depth; i++)
        {
            D(out << name << ": Starting search at depth " << i << std::endl;)
            _logic(i);
            return_move = _move;
        }
        _game.addMove(_move, US);
#else
        usleep(deadline * 1000);
        pthread_mutex_lock( &_m );
        _run = false;
        _move_count++;
        return_move = _move;
        pthread_mutex_lock( &_building );
        return_move = _move;
        pthread_mutex_unlock( &_building );
        pthread_mutex_unlock( &_m );
#endif
        return return_move;
    }
    std::string name;
    std::stringstream out;
protected:
    virtual void _boardPopulated()
    {
        // for the children...
    }
    virtual void _logic(int target_depth)
    {
        if (target_depth == 1)
        {
            for (int col = 0; col < _num_col; col++)
                for (int row = 0; row < _num_row; row++)
                    if (_game[col][row] == EMPTY)
                    {
                        _move = Move(col, row);
                        return;
                    }
        }
        else
        {
            usleep(100);
        }
    }
    virtual void _cleanTree()
    {
        // for the children...
    }
    static void* _buildGameTree(void *instance)
    {
        AIShell* This = (AIShell*)instance;
        unsigned int depth = 1;
        pthread_mutex_lock( &This->_building );
        while (1)
        {
            This->_logic(depth);
            if (This->_run)
                depth++;
            else
            {
                depth = 1;
                if (This->_move_count & 1)
                {
                    This->_game.addMove(This->_move, US);
                    D(This->out << "***" << This->name << ": I moved " << This->_move << std::endl;)
                }
                else
                {
                    This->_game.addMove(This->_move, ENEMY);
                    D(This->out << "***" << This->name << ": Enemy moved " << This->_move << std::endl;)
                }
                This->_cleanTree();
                pthread_mutex_unlock( &This->_building );
                pthread_mutex_lock( &This->_m );
                This->_run = true;
                pthread_mutex_unlock( &This->_m );
                pthread_mutex_lock( &This->_building );
            }
        }
    }
    pthread_t _builder;
    pthread_mutex_t _building;
    pthread_mutex_t _m;
    bool _gravity;
    int _num_col;
    int _num_row;
    int _k;
    GameBoard _game;
    Move _move;
    volatile bool _run;
    int _move_count;
};

#endif //AISHELL_H
