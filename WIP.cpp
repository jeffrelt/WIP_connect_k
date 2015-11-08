//
//  WIP.cpp
//  WIP
//
//  Created by Jeffrey Thompson on 10/11/15.
//  Copyright © 2015 Jeffrey Thompson. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include "AIWIP.hpp"


void say(const char* message)
{
    std::cout<<message<<std::endl;
}
void say(const std::string message)
{
    std::cout<<message<<std::endl;
}

void returnMove(Move move){
    //outputs madeMove then a space then the row then a space then the column
    //then a line break.
    std::cout<<"ReturningTheMoveMade "<<move<<std::endl;
}

// yep globals...
int num_col,num_row,deadline,k;
std::string dump;

void getNextMove(Move& last_move, int& deadline){
    while( dump.compare("makeMoveWithState:") ){
        std::cin >> dump;
    }
    std::cin >> dump;
    std::cin >> dump;
    std::cin >> dump;
    std::cin >> last_move;
    std::cin >> deadline;
}

int main() {
    int which = 1;
    bool gravity = false;
    Move last_move;
    AIShell shell;
    say("Loading Board");
    while( dump.compare("makeMoveWithState:") )
        std::cin >> dump;

    std::cin >> gravity;
    std::cin >> num_col;
    std::cin >> num_row;
    std::cin >> last_move;
    std::cin >> deadline;
    std::cin >> k;
    shell.setBoard(gravity,num_col,num_row,k,last_move);
    for(int i = num_col*num_row;i;--i){
        std::cin >> dump;
    }
    say("Loaded!");
    
    while (1){ //do this forever until the makeAIShellFromInput function ends
        //the process or it is killed by the java wrapper.
        std::cout<<which<<" moved: "<<last_move<<std::endl;
        
        last_move = shell.makeMove(deadline);
        which^=1;
        std::cout<<which<<" moved: "<<last_move<<std::endl;
        which^=1;
        returnMove( last_move );
        
        getNextMove(last_move, deadline);
        shell.enemyMove( last_move );
    }
    return 0;
}