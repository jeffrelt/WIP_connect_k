//
//  GameNode.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 10/18/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#ifndef GameNode_hpp
#define GameNode_hpp


#include "Node.hpp"
#include "Move.h"


class GameNode : public Node{
public:
    GameNode* getChild()
    {
        return static_cast<GameNode*>(child);
    }
    GameNode* getNext()
    {
        return static_cast<GameNode*>(Node::next);
    }
    
    Move my_move;
    int8_t value;
    Node* child;
};


#endif /* GameNode_hpp */
