#ifndef AISHELL_H
#define AISHELL_H

#include "GameBoard.hpp"

using namespace std;
// A new AIShell will be created for every move request.
class AIShell {

public:
    void setBoard(bool gravity, uint8_t num_col, uint8_t num_row, uint8_t k, Move last_move)
    {
        _gravity = gravity;
        _num_col = num_col;
        _num_row = num_row;
        _k = k;
        _row = new int [num_col] ();
        _column = new int [num_row]();
        _diagonal = new int*[num_col];
        for (int i = 0; i < num_col; i++) {
            _diagonal[i] = new int[num_row]();
        }

        _game.setBoard(num_col, num_row);
        if (last_move.getCol() < 15)
            enemyMove(last_move);

    }
    void enemyMove(Move their_move)
    {
        _game.addMove(their_move, cellType::ENEMY);
    }
    Move makeMove(int deadline)
    {
        for (int col = 0; col < _num_col; col++) {
            for (int row = 0; row < _num_row; row++) {
                if (_game[col][row] == cellType::EMPTY) {
                    _game.addMove(col, row, cellType::US);
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

        cout << "row: " << endl;
        for (int i = 0; i < _num_row; i++) {
            cout << _row[i];
        }
        cout << endl;

        cout << "column: " << endl;
        for (int i = 0; i < _num_col; i++) {
            cout << _column[i];
        }
        cout << endl;

        cout << "diagonal: " << endl;
        for (int col = 0; col < _num_col; col++) {
            for (int row = 0; row < _num_row; row++) {
                cout << _diagonal[col][row];
            }
            cout << endl;
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

        int changeIndex = max(maxium, (int)_k);
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
        int score = 0;

        for (int i = 0; i < _num_col; i++) {
            for (int j = 0; j < _num_row; j++) {
                if (std::abs(score) == goal)
                    return score;
                if (_game[i][j] == cellType::US)
                {
                    if (score < 0)
                        score = 0;
                    score++;
                }
                else if (_game[i][j] == cellType::ENEMY)
                {
                    if (score > 0)
                        score = 0;
                    score--;
                }
                else if (_game[i][j] == cellType::EMPTY)
                {
                    score = 0;
                }
            }
        }

    }

    //call this to eval the gameboard everything else I added are just to build scoring sheets
    int eval()    {
        int AIscore = 0;
        int HMscore = 0;
        //i is column
        for (int i = 0; i < _num_col; i++) {
            //j is row
            for (int j = 0; j < _num_row ; j++) {
                if (_game[i][j] == cellType::US) {
                    AIscore += 5 * _column[i] + 5 * _row[j] + 10 * _diagonal[i][j];
                }
                else if (_game[i][j] == cellType::ENEMY) {
                    HMscore += 5 * _column[i] + 5 * _row[j] + 10 * _diagonal[i][j];
                }
            }
        }
        return AIscore - HMscore;
    }

};

#endif //AISHELL_H
