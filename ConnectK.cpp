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
#include "AIShell.h"
#include "AIWIP.hpp"


int main() {
    Move last_move(-1,-1);
    AIShell* AI[2];
    AI[0] = new AIWIP("p0");
    AI[1] = new AIWIP("p1");
    int which = 1;
    std::cout<<"Player #"<<" moved: (col row):"<<std::endl;
    AI[0]->setBoard(true,7,7,5,last_move);
    last_move = AI[0]->makeMove(2000);
    std::cout<<0<<" moved: "<<last_move<<std::endl;
    AI[1]->setBoard(true,7,7,5,last_move);
    
    
    for(int i = 0; i<255;++i){
        last_move = AI[which]->makeMove(2000);
        std::cout<<which<<" moved: "<<last_move<<std::endl;
        if(AI[which]->isGameover())
            break;
        which^=1;
        AI[which]->enemyMove(last_move);
    }
    

    if(AI[which]->isGameover())
    {
        std::cout<<"\nThe Winner is "<<AI[which]->name<<"!"<<std::endl;
    }
    else
    {
        std::cout<<"\nNo one wins..."<<which<<"!"<<std::endl;
    }
    
    return 0;
}

/*
 
 using namespace std;
 
 bool isFirstPlayer = false;
 
 AIShell* makeAIShellFromInput(){
 
	AIShell* shell = NULL;
 
	string begin =  "makeMoveWithState:";
	string end = "end";
	string input;
	bool go = true;
	while (go){
 cin >> input;
 if (input == end){
 exit(0);
 }
 else if (input == begin){
 //first I want the gravity, then number of cols, then number of rows,
 //then the col of the last move, then the row of the last move then the values
 //for all the spaces.
 // 0 for no gravity, 1 for gravity
 //then rows
 //then cols
 //then lastMove col
 //then lastMove row.
 //then deadline.
 //add the K variable after deadline.
 //then the values for the spaces.
 //cout<<"beginning"<<endl;
 int g;
 cin >> g;
 bool gravity=true;
 if (g==0){
 gravity = false;
 }
 cin >> g;
 int colCount = g;
 cin >>g;
 int rowCount = g;
 cin >>g;
 int lastMoveCol = g;
 cin >> g;
 int lastMoveRow = g;
 
 //add the deadline here:
 int deadline = -1;
 cin >>g;
 deadline = g;
 
 cin >> g;
 int k = g;
 
 //now the values for each space.
 
 //allocate 2D array.
 int **gameState = NULL;
 gameState = new int*[colCount];
 for (int i =0; i<colCount; i++){
 gameState[i] = new int[rowCount];
 }
 
 int countMoves = 0;
 for (int col =0; col<colCount; col++){
 for (int row =0; row<rowCount; row++){
 cin >> gameState[col][row];
 if (gameState[col][row] != AIShell::NO_PIECE)
 {
 countMoves += gameState[col][row];
 }
 }
 }
 
 if (countMoves % 2 == 0)
 {
 isFirstPlayer = true;
 }
 
 Move m(lastMoveCol, lastMoveRow);
 AIShell* shell = new AIShell (colCount, rowCount, gravity, gameState, m);
 shell->deadline=deadline;
 shell->k=k;
 
 return shell;
 }
 else {
 cout<<"unrecognized command "<< input<<endl;
 }
 //otherwise loop back to the top and wait for proper input.
	}
 
	return shell;
 }
 
 void returnMove(Move move){
	string madeMove = "ReturningTheMoveMade";
	//outputs madeMove then a space then the row then a space then the column
	//then a line break.
	cout<<madeMove<<" "<<move.col<<" "<<move.row<<endl;
 }
 
 
 bool checkIfFirstPlayer()
 {
	return isFirstPlayer;
 }
 
 int main() {
	cout<<"Make sure this program is ran by the Java shell. It is incomplete on its own. "<<endl;
	bool go = true;
	while (go){ //do this forever until the makeAIShellFromInput function ends
 //the process or it is killed by the java wrapper.
	AIShell *shell = makeAIShellFromInput();
	Move moveMade = shell->makeMove();
	returnMove(moveMade);
	delete shell;
	}
 
	return 0;
 }
*/