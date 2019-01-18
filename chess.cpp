#include "include\chess.h"
#include "C:\Users\Eman\Desktop\Chess\Chess\Players\include\humanPlayer.h"
#include "C:\Users\Eman\Desktop\Chess\Chess\Players\include\L1541412.h"
#include "C:\Users\Eman\Desktop\Chess\Chess\Players\include\L1541414.h"
#include <iostream>

chess::chess(){
    G = new gameState();
    Players[0] = new L1541412();
    Players[1] = new L1541414();
    Interface = new UI();
    Interface->state = G;
}


bool chess::gameOver(){
    if(G->Actions.getActionCount() == 0)
        return true;
    return false;
}

void chess::playGame(){

    action Move;
    int player = 0;
    G->computePossibleActions();
    while(!gameOver()){
        Interface->updateView(Players[0]->playerName, Players[1]->playerName);

        if (G->playerColor == Black){
            for(int i = 0; i<8; i++)
                for(int j = 0; j<8; j++)
                    G->Board.board[i][j]*=-1;
        }

        Players[player]->decideMove(G,&Move);

        if (G->playerColor == Black){
            for(int i = 0; i<8; i++)
                for(int j = 0; j<8; j++)
                    G->Board.board[i][j]*=-1;
        }

        if (G->Actions.validAction(Move)){
            player = (player+1)%2;
            G->applyMove(Move);
        }
        G->computePossibleActions();
    }

    Interface->updateView(Players[0]->playerName, Players[1]->playerName);
    if (G->kingUnderThreat(White))
        std::cout<<"Black Wins";
    else if(G->kingUnderThreat(Black))
        std::cout<<"White Wins";
    else
        std::cout<<"Stale Mate";
}
