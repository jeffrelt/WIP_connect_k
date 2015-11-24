//
//  GameNode.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 10/18/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#ifndef GameNode_hpp
#define GameNode_hpp

#include "Move.h"


struct GameNode {
    GameNode() : child(NULL), next(NULL)
    {}
    ~GameNode()
    {
        if(next)
            delete next;
        if(child)
            delete child;
    }
    Move my_move;
    int value;
    GameNode* child;
    GameNode* next;
};


#endif /* GameNode_hpp */
