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
    void moveToFront(GameNode** old_head, GameNode** new_head)
    {
        if(old_head==new_head)
            return;
        GameNode* hold = *new_head;
        *new_head=hold->next;
        hold->next=*old_head;
        *old_head = hold;
    }
    int8_t ids(int16_t alpha, int16_t beta, GameNode** root, GameBoard board, unsigned int remaining_depth, cellType turn)
    {
        int16_t best;
        GameNode** walker = root;
        unsigned int next_depth = remaining_depth-1;
        cellType next_turn = (cellType)(turn^1);
        bool our_turn = turn&1;
        int16_t val;
        if(!*root)
            *root = board.getPossibleMoves(_num_col,_num_row);

        if(our_turn)
            best = INT16_MIN;
        else
            best = INT16_MAX;

        while(*walker)
        {
            if(!_run)
                throw 1;
            board.addMove((*walker)->my_move,turn);
            if(next_depth)
                val = ids(alpha, beta, &(*walker)->child, board, next_depth, next_turn);
            else
                val = eval(board,our_turn);
            board.removeMove((*walker)->my_move);
            //(*walker)->value = val;
            if(our_turn)
            {
                if(val >= beta)
                    return val;
                if(val > best)
                {
                    moveToFront(root,walker);
                    best = val;
                }
            }
            else
            {
                if(val <= alpha)
                    return val;
                if(val < best)
                {
                    moveToFront(root,walker);
                    best = val;
                }
            }
            
            walker = &(*walker)->next;
        }
        return best;
    }
    virtual void _logic(unsigned int target_depth)
    {
        try{
            ids(INT16_MIN, INT16_MAX, &_root, _game, target_depth, cellType(2|(_move_count&1)));
            _move=_root->my_move;
        }
        catch (...)
        {
            D(std::cout<<_name<<": Turn over. Made it to Depth "<<target_depth-1<<std::endl;)
        }
        
        
        /* iterative may be attempted later...
        GameNode** walker = &_root;
        while(_run)
        {
            if(!*walker)
            {
                *walker = _tree_game.getPossibleMoves(_num_col,_num_row);
                GameNode* grader = *walker;
                while (grader)
                {
                    _tree_game.addMove(grader->my_move,turn);
                    if(depth < target_depth)
                    grader->value=eval(_tree_game, turn&1);
                    _tree_game.removeMove(grader->my_move);
                    grader=grader->getNext();
                    if(turn==cellType::US)
                    {
                        // Alpha/beta pruning goes here
                        
                    }
                }
            }
            else if(depth<target_depth)
            {
                
            }
        }
         */
    }
    virtual void _cleanTree()
    {
        if(!_root)
            return;
        GameNode* walker = _root;
        GameNode* hold = nullptr;
        while(walker->next && walker->next->my_move != _move)
            walker = walker->next;
        if(walker->next)
        {
            hold = walker->child;
            walker->child = nullptr;
        }
        
        delete _root;
        _root = hold;
    }
    GameNode *_root;
    
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
        _row = new int [_num_col] ();
        _column = new int [_num_row]();
        _diagonal = new int*[_num_col];
        for (int i = 0; i < _num_col; i++) {
            _diagonal[i] = new int[_num_row]();
        }
        _row = new int [_num_col] ();
        _column = new int [_num_row]();
        _diagonal = new int*[_num_col];
        for (int i = 0; i < _num_col; i++) {
            _diagonal[i] = new int[_num_row]();
        }


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


    int goalTest(const GameBoard& board) {
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

        //column
        for (int i = 0; i < _num_col; i++) {
            //row
            for (int j = 0; j < _num_row; j++) {
                if (std::abs(vscore) == goal)
                    return vscore;
                if (board[i][j] == cellType::US)
                {
                    if (vscore < 0)
                        vscore = 0;
                    vscore++;
                }
                else if (board[i][j] == cellType::ENEMY)
                {
                    if (vscore > 0)
                        vscore = 0;
                    vscore--;
                }
                else if (board[i][j] == cellType::EMPTY)
                {
                    vscore = 0;
                }
            }
        }

        int hscore = 0;
        //row
        for (int i = 0; i < _num_row; i++) {
            //column
            for (int j = 0; j < _num_col ; j++) {
                if (std::abs(hscore) == goal)
                    return hscore;
                if (board[j][i] == cellType::US)
                {
                    if (hscore < 0)
                        hscore = 0;
                    hscore++;
                }
                else if (board[j][i] == cellType::ENEMY)
                {
                    if (hscore > 0)
                        hscore = 0;
                    hscore--;
                }
                else if (board[j][i] == cellType::EMPTY)
                {
                    hscore = 0;
                }
            }
        }

        return 0;

    }

    //call this to eval the gameboard everything else I added are just to build scoring sheets
    int eval(const GameBoard& board, bool our_turn)    {
        std::cout << "start eval" << std::endl;
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
        return AIscore - HMscore + 100 * goalTest(board);
    }


};

#endif /* AIWIP_hpp */
