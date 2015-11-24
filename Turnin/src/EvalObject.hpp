//
//  EvalObject.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 11/11/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//
#ifndef EvalObject_hpp
#define EvalObject_hpp

#include "CQueue.hpp"
#include <iostream>


class EvalObject {
public:
    EvalObject(int k)
    {
        _gameover = false;
        _k = k;
        _score = 0;
        queue = new CQueue(k);
        AIcount = 0;
        HMcount = 0;
        Empty = 0 ;
    }
    void operator() (const cellType cell)
    {
        if (!queue->push(cell)) {
            _score += _check();
            cellType temp = queue->pop();
            if (temp == US)
                AIcount--;
            else if (temp == ENEMY)
                HMcount--;
            else if (temp == EMPTY)
                Empty--;
            if (!queue->push(cell))
            {
                D(std::cout << "!!!!!something is wrong!!!!!" << std::endl;)
            }
            else {
                if (cell == US)
                    AIcount++;
                else if (cell == ENEMY)
                    HMcount++;
                else if (cell == EMPTY)
                    Empty++;
            }
        }
        else {
            if (cell == US)
                AIcount++;
            else if (cell == ENEMY)
                HMcount++;
            else if (cell == EMPTY)
                Empty++;
        }

    }
    void endl()
    {
        _score += _check();
        queue->reset();
        AIcount = 0;
        HMcount = 0;
        Empty = 0;
    }
    operator int()
    {
        return _score;
    }
    bool gameOver()
    {
        return _gameover;
    }
private:
    int _check()
    {

        //D(std::cout << "AI: " << AIcount << std::endl;)
        //D(std::cout << "HM: " << HMcount << std::endl;)

        //both greater than 0
        if (AIcount && HMcount)
            return 0;
        else if (AIcount && !HMcount)
        {
            //AI has more
            if (AIcount == _k)
            {
                _gameover = true;
                return 5000;
            }
            else
                return 30 * AIcount  - 4 * Empty;
        }
        else if (HMcount && !AIcount)
        {
            //HM has more
            if (HMcount == _k)
            {
                _gameover = true;
                return -5000;
            }
            else
                return -30 * HMcount + 4 * Empty;
        }
        else {
            //both 0
            return 0;
        }
    }

    bool _gameover;
    int _k;
    int _score;
    int AIcount;
    int HMcount;
    int Empty;
    CQueue *queue;
};



#endif /* EvalObject_hpp */
