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
#include "EvalObject.hpp"

class AIWIP: public AIShell {
public:
    AIWIP() : _root(NULL)
    {}
    AIWIP(const char* name) : AIShell(name), _root(NULL)
    {}
protected:
    virtual GameNode* getPossibleMoves(const GameBoard& board, bool best_only = false)
    {
        D(out << name << ": Generating possible moves";)
        GameNode* root = NULL;
        GameNode* hold;
        for (int col = 0; col < _num_col; col++) {
            for (int row = 0; row< _num_row; row++) {
                if (board[col][row] == EMPTY) {
                    hold = root;
                    // NOTE: here we create a GameNode - this will change later
                    // it is the callers responsibility to delete it
                    root = new GameNode;
                    root->my_move = Move(col, row);
                    D(out <<", "<<root->my_move;)
                    root->next = hold;
                    if (_gravity)
                        break;
                }
            }
        }
        D(out<< std::endl;)
        return root;
    }
    virtual void _boardPopulated()
    {
        //buildScoring();
    }
    virtual bool moveToFront(GameNode** old_head, GameNode** new_head)
    {
        if(old_head == new_head)
            return true;
        GameNode* hold = *new_head;
        *new_head = hold->next;
        hold->next = *old_head;
        *old_head = hold;
        D(out << name << ": Moving last to head of list" << std::endl;)
        return false;
    }
    virtual int ids(int alpha, int beta, GameNode** root, GameBoard board, int remaining_depth, cellType turn)
    {
        int best;
        GameNode** walker = root;
        int next_depth = remaining_depth - 1;
        cellType next_turn = (cellType)(turn ^ 1);
        bool our_turn = !(turn & 1);
        int val;
        if (!*root)
            *root = getPossibleMoves(board);
        if (!*root)
            return val = eval3(board, _k);
        
        D(int possible_count = 0;
          while(*walker)
          {
              walker = &(*walker)->next;
              ++possible_count;
          }
          walker = root;)
        
        if (our_turn)
            best = INT_MIN;
        else
            best = INT_MAX;

        while (*walker)
        {
            if (!_run)
                return best;
            board.addMove((*walker)->my_move, turn);
            D(out << turn << ": added " << (*walker)->my_move << " at depth " << search_depth - remaining_depth << std::endl;)
            if (next_depth)
                val = ids(alpha, beta, &(*walker)->child, board, next_depth, next_turn);
            else
                val = eval3(board, _k);
            board.removeMove((*walker)->my_move);
            D(out << turn << ": removed " << (*walker)->my_move << " at depth " << search_depth - remaining_depth << std::endl;)
            (*walker)->value = val;
            if (our_turn)
            {
                if (val > alpha)
                {
                    D(out << turn << ": new alpha: " << val << " old:" << alpha << std::endl;)
                    alpha = val;
                }
                if (val > best)
                {
                    if(moveToFront(root,walker))
                        walker = &(*walker)->next;
                    D(out << turn << ": new best: " << val << " old:" << best << std::endl;)
                    best = val;
                }
                else
                    walker = &(*walker)->next;
            }
            else
            {
                if (val < beta)
                {
                    D(out << turn << ": new beta: " << val << " old:" << beta << std::endl;)
                    beta = val;
                }
                if (val < best)
                {
                    if(moveToFront(root,walker))
                        walker = &(*walker)->next;
                    D(out << turn << ": new best: " << val << " old:" << best << std::endl;)
                    best = val;
                }
                else
                    walker = &(*walker)->next;
            }
            if (alpha >= beta)
            {
                D(out << name << ": trimming branch as a>=b ("<<alpha<<">="<<beta<<") returning " << best << " from depth " << search_depth - remaining_depth << std::endl;)
                return best;
            }
            D(--possible_count;)
        }
        D(out << name << ": returning " << best << " from depth " << search_depth - remaining_depth << std::endl;
          if(possible_count)
          {
              out << "**error we have not explored all moves! remaining: "<<possible_count<<std::endl;
              throw "not all moves explored";
          })
        return best;
    }
    virtual void _logic(int target_depth)
    {
        search_depth = target_depth + 1;
        int best = ids(INT_MIN, INT_MAX, &_root, _game, target_depth, cellType(2 | (_move_count & 1)));
        if (_root)
        {
#ifndef SINGLE_THREAD
            if(pthread_mutex_trylock(&_m))
            {
                out << name << ": Search over, stopped in depth " << target_depth << std::endl;
                return;
            }
#endif
            _move = _root->my_move;
            out << name << ": best from search at depth " << target_depth << " is " << _move << " with a score of " << best << std::endl;
            D(out<< "**********************" << std::endl;)
#ifndef SINGLE_THREAD
            pthread_mutex_unlock(&_m);
#endif
        }
        else
        {
            out << name << ": Game Over!"<< std::endl<<"*********************************************" << std::endl;
#ifndef SINGLE_THREAD
            pthread_exit(NULL);
#endif
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
                    if(turn==US)
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
        /* bugs in here - need to prune the move from within the tree too!
        if (!_root)
            return;
        GameNode* walker = _root;
        GameNode* hold = nullptr;
        while (walker->next && walker->next->my_move != _move)
            walker = walker->next;
        if (walker->next)
        {
            hold = walker->child;
            walker->child = nullptr;
        }

        delete _root;
        _root = hold;
         */
        if (_root)
        {
            delete _root;
            _root = NULL;
        }
    }
    GameNode *_root;
    int search_depth;

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


    int eval2(const GameBoard& board, bool our_turn)
    {
        int score = 0;
        for (int i = 0; i < _num_col; ++i)
        {
            cellType last = BOUNDRY;
            for (int j = 0; j < _num_row; ++j)
            {

            }
        }


        return 0;
    }

    int eval3(GameBoard& board, int k)
    {

        EvalObject coleval (k);
        EvalObject roweval (k);
        EvalObject d1eval (k);
        EvalObject d2eval (k);

        D(out << "roweval" << std::endl;)
        for (int i = 0; i < _num_row; i++ )
        {
            for (int j = 0; j < _num_col; j++)
            {
                roweval(board[j][i]);
            }
            roweval.endl();
            if (roweval.gameOver())
                return int(roweval) > 0 ? INT_MAX : INT_MIN;
        }


        D(out << "coleval" << std::endl;)
        for (int i = 0; i < _num_col; i++ )
        {
            for (int j = 0; j < _num_row; j++)
            {
                coleval(board[i][j]);
            }
            coleval.endl();
            if (coleval.gameOver())
                return int(coleval) > 0 ? INT_MAX : INT_MIN;
        }

        D(out << "deval" << std::endl;)
        for (int boardCol = 0; boardCol < _num_col - _k + 1; boardCol++) {
            for (int boardRow = 0; boardRow < _num_row - _k + 1; boardRow++) {
                for (int i = 0; i < _k; i++) {
                    d1eval(board[boardCol + i][boardRow + i]);
                    d2eval(board[boardCol + i][boardRow + (_k - i - 1)]);
                }
                d1eval.endl();
                d2eval.endl();
                if (d1eval.gameOver())
                    return int(d1eval) > 0 ? INT_MAX : INT_MIN;
                if (d2eval.gameOver())
                    return int(d2eval) > 0 ? INT_MAX : INT_MIN;
            }
        }

        int score = int(coleval) + int(roweval) + int(d1eval) + int(d2eval);

        D(out << "score: " << score << std::endl;)
        D(out << "coleval: " << int(coleval) << std::endl;)
        D(out << "roweval: " << int(roweval) << std::endl;)
        D(out << "d1eval: " << int(d1eval) << std::endl;)
        D(out << "d2eval: " << int(d2eval) << std::endl;)

        return score;

    }


    int *_row; //socring for thw row
    int *_column;  //socring for the column
    int **_diagonal;
    int calculateMax(int number)   {
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
        D(out << name << ": after_new" << std::endl;)

        int col_number = calculateMax(_num_col);
        int row_number = calculateMax(_num_row);
        int temp = 0;
        D(out << name << ": after calc_max" << std::endl;)
        buildHelper(_column, col_number, _num_col);
        buildHelper(_row, row_number, _num_row);
        D(out << name << ": after build_helpers" << std::endl;)
        diagHelper();

        D(out << name << ": post_helper" << std::endl;)
        D(out << "row: " << std::endl;)
        for (int i = 0; i < _num_row; i++) {
            D(out << _row[i];)
        }
        D(out << std::endl;)

        D(out << "column: " << std::endl;)
        for (int i = 0; i < _num_col; i++) {
            D(out << _column[i];)
        }
        D(out << std::endl;)

        D(out << "diagonal: " << std::endl;)
        for (int col = 0; col < _num_col; col++) {
            for (int row = 0; row < _num_row; row++) {
                D(out << _diagonal[col][row];)
            }
            D(out << std::endl;)
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
        D(out << name << ": before diagApply" << std::endl;)
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


//call this to eval the gameboard everything else I added are just to build scoring sheets
    int eval(const GameBoard & board, bool our_turn)    {

        //out << "start eval" << std::endl;
        int AIscore = 0;
        int HMscore = 0;
        //i is column
        for (int i = 0; i < _num_col; i++) {
            //j is row
            for (int j = 0; j < _num_row ; j++) {
                if (board[i][j] == US) {
                    AIscore += 5 * _column[i] + 5 * _row[j] + 10 * _diagonal[i][j];
                }
                else if (board[i][j] == ENEMY) {
                    HMscore += 5 * _column[i] + 5 * _row[j] + 10 * _diagonal[i][j];
                }
            }
        }
        int score = AIscore - HMscore + 100 * goalTest(board);

        D(out << name << ": score: " <<  score << std::endl;)
        /*out << "AIscore: " <<  AIscore << std::endl;
        out << "HMscore: " << HMscore << std::endl;
        out << "Goaltest: " << goalTest(board) << std::endl;*/
        return score;
    }


};

#endif /* AIWIP_hpp */
