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

class EvalObject {
public:
    EvalObject(int k)
    {
        _gameover = false;
        _k = k;
        _score = 0;
        queue = new CQueue(k);
    }
    void operator() (const cellType cell)
    {
        bool temp = queue->push(cell);
        if (!temp) {
            _score += _check(queue->get());
            queue->pop();
            if (!queue->push(cell))
                std::cout << "!!!!!something is wrong!!!!!" << std::endl;
        }
    }
    void endl()
    {
        _score += _check(queue->get());
        queue->reset();
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
    int _check(Cell * arrary)
    {
        int AIcount = 0;
        int HMcount = 0;
        for (int i = 0 ; i < _k; i++) {
            if (arrary[i] == US)
                AIcount++;
            else if ( arrary[i] == ENEMY)
                HMcount++;
        }
/*
        std::cout << "AI: " << AIcount << std::endl;
        std::cout << "HM: " << HMcount << std::endl;
*/
        //both greater than 0
        if (AIcount && HMcount)
            return 0;
        else if (AIcount && !HMcount)
        {
            //AI has more
            if (AIcount == _k)
            {
                _gameover = true;
                return 1000;
            }
            else
                return 10 * AIcount;
        }
        else if (HMcount && !AIcount)
        {
            //HM has more
            if (HMcount == _k)
            {
                _gameover = true;
                return -1000;
            }
            else
                return -10 * HMcount;
        }
        else {
            //both 0
            return 0;
        }
    }

    bool _gameover;
    int _k;
    int _score;
    CQueue *queue;
};



#endif /* EvalObject_hpp */
