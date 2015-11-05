//
//  GameManager.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 10/18/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <stdio.h>
#include <vector>
#include <thread>
#include <cstdint>
#include <array>
#include <memory>

#include "Move.h"
#include "GameNode.hpp"
#include "Job.hpp"
#include "GameBoard.hpp"
#include "NodeQueue.hpp"

class GameManager
{
public:
    GameManager()
    {
        for(int i = 0; i < UINT16_MAX; ++i)
        {
            _empty_nodes.unsafePushBack(&_node_objects[i]);
        }
    }
    
    void setBoard(bool gravity, uint8_t num_col, uint8_t num_row, uint8_t k, Move last_move)
    {
        
        //if(last_move.getCol() >= 0)
            //manager->enemyMoved(last_move);
    }
    
    
    
    Move makeMove(int deadline)
    {
        run = 0;
        Move return_this = _root->my_move;
        // update our state, start new jobs
        //_game.play(hold, OURS);
        GameNode* hold = _root;
        _root = _root->getChild();
        hold->child = nullptr;
        _empty_nodes.pushBack(hold);
        return return_this;
    }
    void enemyMoved(Move where)
    {
        GameNode* walker = _root;
        GameNode* new_root = nullptr;
        while(walker)
            if(walker->my_move == where)
            {
                new_root = walker;
                break;
            }
            else
                walker = walker->getNext();
        //if(!walker)
        
    }
    

    
private:
    Job& getJob();
    void queueJob(GameState who);
    
    GameNode* getNode()
    {
        return static_cast<GameNode*>(_empty_nodes.popFront());
    }
    void returnNode(GameNode* who) //and all other nodes attached
    {
        GameNode* hold = who;
        // return all children
        while(who)
        {
            if(who->getChild())
                returnNode(who->getChild());
            
            who = who->getNext();
        }
        // return the linked list
        _empty_nodes.pushBack(hold);
    }
    
    int run;
    //std::unique_ptr<std::thread> _manager;
    //std::vector<std::unique_ptr<std::thread>> _thread_pool;
    //NodeQueue<Job,255> _empty_jobs;

    std::array<GameNode,UINT16_MAX> _node_objects;
    NodeQueue _empty_nodes;
    
    std::unique_ptr<GameState> _game(new GameState);
    GameNode* _root;
};

#endif /* GameManager_hpp */
