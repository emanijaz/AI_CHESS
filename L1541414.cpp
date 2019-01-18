#include "include\L1541414.h"
#include<iostream>
#include "C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\ENUM.h"
#include "C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\chessPlayer.h"

using namespace std;

L1541414::L1541414(Color playerColor):chessPlayer("Auto Player Name", playerColor){


}

void L1541414::decideMove(const gameState* state, action* Move, int maxDepth ){

    cout<<endl<<"Please Enter a Move: from (row, col) to (row, col)"<<endl;
    cin>>(*Move).fromRow>>(*Move).fromCol>>(*Move).toRow>>(*Move).toCol;
}
