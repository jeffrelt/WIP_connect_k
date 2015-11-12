//
//  cQueue.hpp
//  WIP
//
//  Created by Jeffrey Thompson on 11/7/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#ifndef CQueue_hpp
#define CQueue_hpp

class CQueue {
public:
    CQueue(int cap)
    {
        capacity = cap;
        queue = new Cell [cap];
        rear = -1;
        front = -1;
    }

    cellType pop() {
        cellType data;
        if (front == -1)
        {
            return BOUNDRY;
        }
        data = queue[front];
        if (front == rear)
        {
            front = -1;
            rear = -1;
        }
        else
            front = (front + 1) % capacity;
        return data;
    }

    bool push(cellType cell)
    {
        if ((rear + 1) % capacity == front)
            return false;
        rear = (rear + 1) % capacity;
        queue[rear] = cell;
        if (front == -1)
            front = 0;
        return true;
    }

    Cell* get()
    {
        return queue;
    }

    void reset() {
        front = -1;
        rear = -1;
    }
    ~CQueue()
    {
        delete [] queue;
    }

//private:
    Cell *queue;
    int front;
    int rear;
    int capacity;

};
#endif /* CQueue_hpp */
