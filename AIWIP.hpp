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

class AIWIP: public AIShell{
public:
    AIWIP()
    {}
    AIWIP(const char* name) : AIShell(name)
    {}
protected:
    virtual void _logic(int depth)
    {
        if(depth == 1)
        {
            for (int col = _num_col; col; col--)
            {
                for (int row = _num_row; row; row--)
                {
                    if (_game[col][row] == cellType::EMPTY)
                    {
                        _game.addMove(col,row, cellType::US);
                        _move = Move(col, row);
                        return;
                    }
                }
            }
        }
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    GameNode *root;
};

#endif /* AIWIP_hpp */
