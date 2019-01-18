#include "Skeleton\include\actionList.h"
#include <iostream>
#include<iomanip>
#include "Skeleton\include\chess.h"
#include "Players\include\L1541412.h"
#include "Players\include\L1541414.h"
#include "Players\include\humanPlayer.h"

using namespace std;

int main(){
    chess Game;
//    Game.Players[0] = new humanPlayer("Human Player Name", White);
    Game.Players[0] = new L1541412();
//	Game.Players[0] 
	Game.Players[1] = new L1541414();
    Game.playGame();
    return 0;
}
