#ifndef L1541412_H
#define L1541412_H
#include "C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\chessPlayer.h"
#include"C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\ENUM.h"

class L1541412 : public chessPlayer {
    public:
        L1541412(Color playerColor=White);
        void decideMove(const gameState* state, action* Move, int maxDepth);
};


#endif // AUTOPLAYER_H
