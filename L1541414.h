#ifndef L1541414_H
#define L1541414_H
#include "C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\chessPlayer.h"
#include"C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\ENUM.h"

class L1541414 : public chessPlayer {
    public:
        L1541414(Color playerColor=White);
        void decideMove(const gameState* state, action* Move, int maxDepth);
};


#endif // AUTOPLAYER_H
