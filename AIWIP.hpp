//
//  AIWIP.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 11/7/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#ifndef AIWIP_hpp
#define AIWIP_hpp

#include "AIShell.h"

class AIWIP: public AIShell {
public:
    AIWIP()
    {}
    AIWIP(const char* name) : AIShell(name)
    {}
protected:
    virtual void _boardPopulated()
    {
        buildScoring();
    }
    virtual void _logic(unsigned int target_depth)
    {
        unsigned int depth = 1;
        cellType turn = cellType(2 | (_move_count & 1));
        _tree_game = _game;
        alpha = INT8_MIN;
        beta = INT8_MAX;
        GameNode** walker = &_root;
        while (_run)
        {
            if (*walker)
            {

            }
            else
            {
                *walker = _tree_game.getPossibleMoves(_num_col, _num_row);
                GameNode* grader = *walker;
                while (grader)
                {
                    _tree_game.addMove(grader->my_move, turn);
                    grader->value = eval(_tree_game);
                    _tree_game.removeMove(grader->my_move);

                    /* alpha beta pruning here
                    if(turn==cellType::US)
                    {
                        if
                    }
                     */
                }
            }
        }
    }

    virtual void _cleanTree()
    {

    }
    int8_t alpha, beta;
    GameNode *_root;
    GameBoard _tree_game;

    /* what is in AIShell.h:
     std::string _name;
     std::thread* _builder;
     bool _gravity;
     uint8_t _num_col;
     uint8_t _num_row;
     uint8_t _k;
     GameBoard _game;
     uint8_t _move;
     std::atomic<bool> _run;
     std::atomic<unsigned int> _move_count;
     */

//heuristic stuff:

    int *_row; //socring for thw row
    int *_column;  //socring for the column
    int **_diagonal;


    int calculateMax(uint8_t number)   {
        return (number - _k + 1);
    }

    //this only need to be called once per game
    void buildScoring() {
        int col_number = calculateMax(_num_col);
        int row_number = calculateMax(_num_row);
        int temp = 0;
        buildHelper(_column, col_number, _num_col);
        buildHelper(_row, row_number, _num_row);
        diagHelper();

        std::cout << "row: " << std::endl;
        for (int i = 0; i < _num_row; i++) {
            std::cout << _row[i];
        }
        std::cout << std::endl;

        std::cout << "column: " << std::endl;
        for (int i = 0; i < _num_col; i++) {
            std::cout << _column[i];
        }
        std::cout << std::endl;

        std::cout << "diagonal: " << std::endl;
        for (int col = 0; col < _num_col; col++) {
            for (int row = 0; row < _num_row; row++) {
                std::cout << _diagonal[col][row];
            }
            std::cout << std::endl;
        }
    }

    void diagHelper() {
        int ** temp = new int* [_k];
        for (int i = 0; i < _k; i++) {
            temp[i] = new int[_k]();
        }

        for (int i = 0; i < _k; i++) {
            temp[i][i] = temp[i][i] + 1;
            temp[i][_k - i - 1] = temp[i][_k - i - 1] + 1 ;
        }
        diagApply(temp);

    }
    void diagApply(int **kboard) {
        for (int boardCol = 0; boardCol < _num_col - _k + 1; boardCol++) {
            for (int boardRow = 0; boardRow < _num_row - _k + 1; boardRow++) {
                for (int i = 0; i < _k; i++) {
                    _diagonal[boardCol + i][boardRow + i] += kboard[i][i];
                    _diagonal[boardCol + i][boardRow + (_k - i - 1)] += kboard[i][(_k - i - 1)];
                }
            }
        }
    }

    void buildHelper(int *array, int maxium, int size) {
        bool flag = _k < maxium;

        int changeIndex = std::max(maxium, (int)_k);
        //ture change from max
        if (flag) {
            int temp = 1;
            int i = 0;
            for (; i < changeIndex; i++ ) {
                if (temp >= _k) {
                    array[i] = _k;
                }
                else {
                    array[i] = temp++;
                }
            }
            temp = _k - 1;
            for (; i < size; i++) {
                array[i] = temp--;
            }

        }
        //false chage from _k
        else {
            int temp = 1;
            int i = 0;
            for (; i < changeIndex; i++ ) {
                if (temp >= maxium) {
                    array[i] = maxium;
                }
                else {
                    array[i] = temp++;
                }
            }
            temp = maxium - 1;
            for (; i < size; i++) {
                array[i] = temp--;
            }
        }
    }


    int goalTest() {
        int goal = 0;
        if (_k == 2)
            goal = 1;
        else if (_k == 3)
            goal = 2;
        else
            goal = _k - 1;

        //0 no wining move on us or the enemy
        //1 we are wining
        //-1 we are going to lose
        int vscore = 0;

        for (int i = 0; i < _num_col; i++) {
            for (int j = 0; j < _num_row; j++) {
                if (std::abs(vscore) == goal)
                    return vscore;
                if (_game[i][j] == cellType::US)
                {
                    if (vscore < 0)
                        vscore = 0;
                    vscore++;
                }
                else if (_game[i][j] == cellType::ENEMY)
                {
                    if (vscore > 0)
                        vscore = 0;
                    vscore--;
                }
                else if (_game[i][j] == cellType::EMPTY)
                {
                    vscore = 0;
                }
            }
        }

        int hscore = 0;
        for (int i = 0; i < _num_row; i++) {
            for (int j = 0; j < _num_col ; j++) {
                if (std::abs(hscore) == goal)
                    return hscore;
                if (_game[j][i] == cellType::US)
                {
                    if (hscore < 0)
                        hscore = 0;
                    hscore++;
                }
                else if (_game[j][i] == cellType::ENEMY)
                {
                    if (hscore > 0)
                        hscore = 0;
                    hscore--;
                }
                else if (_game[j][i] == cellType::EMPTY)
                {
                    hscore = 0;
                }
            }
        }

        return 0;

    }

    //call this to eval the gameboard everything else I added are just to build scoring sheets
    int eval(const GameBoard& board, bool our_turn)    {
        int AIscore = 0;
        int HMscore = 0;
        //i is column
        for (int i = 0; i < _num_col; i++) {
            //j is row
            for (int j = 0; j < _num_row ; j++) {
                if (board[i][j] == cellType::US) {
                    AIscore += 5 * _column[i] + 5 * _row[j] + 10 * _diagonal[i][j];
                }
                else if (board[i][j] == cellType::ENEMY) {
                    HMscore += 5 * _column[i] + 5 * _row[j] + 10 * _diagonal[i][j];
                }
            }
        }
        return AIscore - HMscore + 100 * goalTest)();
    }


};

#endif /* AIWIP_hpp */
