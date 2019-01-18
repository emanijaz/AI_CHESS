#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include<string>
#include "C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\ENUM.h"
#include "C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\chessPlayer.h"

class humanPlayer : public chessPlayer {
    public:
        humanPlayer(std::string playerName="DEFAULT NAME", Color playerColor=White);
        void decideMove(const gameState* state, action* Move, int maxDepth = -1);
};


#endif // HUMANPLAYER_H
