//
//  NodeQueue.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 10/19/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#ifndef NodeQueue_hpp
#define NodeQueue_hpp

#include "Node.hpp"
#include <mutex>
#include <condition_variable>

class NodeQueue
{
public:
    void pushBack(Node* who) //accepts a list or single node
    {
        std::lock_guard<std::mutex> lock(_mutex);
        unsafePushBack(who);
    }
    // return QUEUE_EMPTY (-1) if the queue is empty
    Node* popFront()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return unsafePopFront();
    }
    // wait until queue populates if empty
    Node* popFrontWait()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while(_front == nullptr)
            _cv.wait(lock);
        return unsafePopFront();
    }
    
// unsafe queue operations
// do not use using single threaded
    void unsafePushBack(Node* who)
    {
        // connect "who" to the linked list
        if(_back)
            _back->next = who;
        else
        {
            _front = who;
            _cv.notify_one();
        }
        // set our new back
        _back = who;
        
        // walk _back to the end of the list (in case returned a list)
        while(_back->next)
        {
            _back = _back->next;
            _cv.notify_one();
        }
    }
    Node* unsafePopFront()
    {
        if(_front == nullptr)
            return nullptr;
        // get the front value
        Node* hold = _front;
        hold->next = nullptr;
        // move the front to the next node
        _front = _front->next;
        return hold;
    }
    
private:
    std::mutex _mutex;
    std::condition_variable _cv;
    Node* _front = nullptr;
    Node* _back = nullptr;
};

#endif /* NodeQueue_hpp */
