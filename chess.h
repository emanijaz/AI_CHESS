#ifndef L154267_H
#define L154267_H
#include "chessPlayer.h"
#include "ENUM.h"

class L154267 : public chessPlayer {
public:
	L154267(Color playerColor = White);
	void decideMove(const gameState* state, action* Move, int maxDepth);
};


#endif // AUTOPLAYER_H
