//
//  StackQueue.h
//  WIP
//
//  Created by Jeffrey Thompson on 10/14/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#ifndef StackQueue_h
#define StackQueue_h

#include <array>

template<typename DATA_TYPE, typename COUNT_TYPE, uint8_t COUNT_BITS>
class myDeque{
    // fixed sized circular queue/stack of flexable types
    // Note: is not safe - does no checking.
public:
    myDeque() : _base(0), _size(0) {}
    void pushBack(DATA_TYPE who)
    {
        int loc = (_base + _size) & ((1<<COUNT_BITS)-1);
        _data[loc] = who;
        ++_size;
    }
    void popBack()
    {
        --_size;
    }
    void pushFront(DATA_TYPE who)
    {
        ++_size;
        _base = (_base-1) & ((1<<COUNT_BITS)-1);
        _data[_base] = who;
    }
    void popFront()
    {
        _base = (_base+1) & ((1<<COUNT_BITS)-1);
        --_size;
    }
    DATA_TYPE operator [](int index)const
    //0 is front, -1 is back
    {
        int loc;
        if(index >= 0)
            loc = (_base + index) & ((1<<COUNT_BITS)-1);
        else
            loc = (_base + _size + index) & ((1<<COUNT_BITS)-1);
        return _hist[loc];
    }
    std::array<DATA_TYPE,(1<<COUNT_BITS)> const& getArray()const
    //for iterating over elements of the array
    {
        return _data;
    }
    COUNT_TYPE size()const
    {
        return _size;
    }
private:
    // 16 bytes in all
    std::array<DATA_TYPE,(1<<COUNT_BITS)> _data;
    COUNT_TYPE _base;
    COUNT_TYPE _size;
};

#endif //StackQueue_h