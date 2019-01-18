#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "chessBoard.h"
#include <string>
#include "ENUM.h"
#include "actionList.h"

class gameState{
    public:
        Color playerColor;
        chessBoard Board;
        actionList Actions;
        // New State Added
        bool firstMoveOfWhiteKing;
        bool firstMoveOfBlackKing;

        gameState();
     //   gameState(gameState* state);
        void setPlayer(Color player = White);
        int loadGameFromFile(std::string fileName);
        bool inRange(int r, int c);
        void computePieceMoves(int R, int C, int dr, int dc, int maxMoves);
        void computePieceActions(int R, int C);
        int computePossibleActions();
        int applyMove(action& Move);

        bool kingUnderThreat(Color kingColor);
        bool checkThreat(int r, int c, Color kingColor, int PieceCodes);
        bool pieceCanMove(int R, int C, int r, int c, Color playerColor);
        void promotePawn(int pawnCode, action& Move);
};

#endif // GAMESTATE_H
