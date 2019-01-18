#include "include\L1541412.h"
#include<iostream>
#include <algorithm>
#include "C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\ENUM.h"
#include "C:\Users\Eman\Desktop\Chess\Chess\Skeleton\include\chessPlayer.h"

using namespace std;

/////////////////////   position tables  ////////////////////////////

int knight_table[8][8] = { -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0, 0, 0, 0, -20, -40, -30, 0, 10, 15, 15, 10, 0, -30,
-30, 5, 15, 20, 20, 15, 5, -30, -30, 0, 15, 20, 20, 15, 0, -30, -30, 5, 10, 15, 15, 10, 5, -30, -40, -20, 0, 5, 5, 0, -20, -40,
-50, -40, -20, -30, -30, -20, -40, -50 };


int pawn_table[8][8] = { 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 50, 50, 50, 50, 50, 50, 10, 10, 20, 30, 30, 20, 10, 10,
5, 5, 10, 27, 27, 10, 5, 5, 0, 0, 0, 25, 25, 0, 0, 0, 5, -5, -10, 0, 0, -10, -5, 5, 5, 10, 10, -25, -25, 10, 10, 5,
0, 0, 0, 0, 0, 0, 0, 0 };

int king_table_startGame[8][8] = { -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30,
-30, -40, -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30, -20, -10, -20, -20, -20, -20, -20, -20, -10,
20, 20, 0, 0, 0, 0, 20, 20, 20, 30, 10, 0, 0, 10, 30, 20 };

int king_table_endGame[8][8] = { -50, -40, -30, -20, -20, -30, -40, -50, -30, -20, -10, 0, 0, -10, -20, -30, -30, -10, 20, 30, 30, 20, -10, -30,
-30, -10, 30, 40, 40, 30, -10, -30, -30, -10, 30, 40, 40, 30, -10, -30, -30, -10, 20, 30, 30, 20, -10, -30,-30, -30, 0, 0, 0, 0, -30, -30,
-50, -30, -30, -30, -30, -30, -30, -50 };

int bishop_table[8][8] = { -20, -10, -10, -10, -10, -10, -10, -20, -10, 0, 0, 0, 0, 0, 0, -10, -10, 0, 5, 10, 10, 5, 0, -10,
-10, 5, 5, 10, 10, 5, 5, -10, -10, 0, 10, 10, 10, 10, 0, -10, -10, 10, 10, 10, 10, 10, 10, -10, -10, 5, 0, 0, 0, 0, 5, -10,
-20, -10, -40, -10, -10, -40, -10, -20 };

////// every piece holds specific weight(percentage ) in chess board //////////
int Pawn_weight = 100, Knight_weight = 320, bishop_weight = 325, rook_weight = 500, queen_weight = 975, king_weight = 32767;
const int positiveInfinity = +1000000;
const int negativeInfinity = -1000000;
int gamePhase;
int protect_arr[8][8];// a square value represents how many same color pieces defend it with factor of their pieceActionValues
int attack_arr[8][8];// a square value represents how many different color pieces attack it with factor of their pieceActionValues
int validMovesCount[8][8]; // contains valid moves per piece // pawn, knight, bishop, rook, queen, king // 0,1,2,3,4,5
int countBishops; // used in evaluating score,
// white an black pawn scores are separately evaluated and then used for scoring isolated pawns
int white_pawn_score[8];
int black_pawn_score[8];

int pieceActionValue(int piece_id)
{

	if (piece_id == 1 || piece_id == -1) // pawn
	{
		return 6;
	}
	else if (piece_id == 2 || piece_id == -2) // knight
	{
		return 3;
	}
	else if (piece_id == 3 || piece_id == -3) // bishop
	{
		return 3;
	}
	else if (piece_id == 4 || piece_id == -4) // rook
	{
		return 2;
	}
	else if (piece_id == 5 || piece_id == -5) // queen
	{
		return 1;
	}
	else if (piece_id == 6 || piece_id == -6) // king
	{
		return 1;
	}
	return 0;

}


void compute_evaluators(int R, int C, int dr, int dc, int maxMoves, gameState* S)
{
	int r, c;
	int MoveCounter = 0;
	//        action Move;

	for (r = R + dr, c = C + dc; MoveCounter < maxMoves && S->inRange(r, c) && S->Board.board[r][c] == 0; r += dr, c += dc, MoveCounter++);
	if (S->inRange(r, c) && S->Board.board[r][c] * S->Board.board[R][C] < 0){
		validMovesCount[R][C] = MoveCounter; // adding possible valid moves to array
		attack_arr[r][c] += pieceActionValue(S->Board.board[R][C]); // increase attack value of piece being attacked by actionValue of attacking piece
	}
	else if (S->Board.board[r][c] * S->Board.board[R][C] > 0) // same color piece
	{

		protect_arr[r][c] += pieceActionValue(S->Board.board[R][C]); // increase attack value of piece being attacked by actionValue of attacking piece

	}
}
void computeEvaluationFeatures(gameState* S){

	////////////////////////// COMPUTE GAME PHASE ///////////////////////////////
	int piece_count = 0;
	for (int i = 0; i<8; i++){
		for (int j = 0; j<8; j++)
		{
			if (S->Board.board[i][j] != 0) // piece found
				piece_count++;
		}
	}
	if (piece_count < 10) // end game phase
	{
		gamePhase = -1;
	}

	else if (piece_count > 22) // start phase
		gamePhase = 1;
	else
		gamePhase = 0;
	// middle phase

	/////////////////////// compute evaluators /////////////////
	int playerSign = S->playerColor * 2 - 1;
	for (int r = 0; r<8; r++){
		for (int c = 0; c<8; c++)
		{
			if (S->Board.board[r][c] * playerSign > 0){
				int pieceCode = S->Board.board[r][c];
				switch (pieceCode){
				case 4:  // rook
				case -4:
					compute_evaluators(r, c, 1, 0, 7, S);
					compute_evaluators(r, c, -1, 0, 7, S);
					compute_evaluators(r, c, 0, 1, 7, S);
					compute_evaluators(r, c, 0, -1, 7, S);
					break;
				case 2:  // knight
				case -2:
					compute_evaluators(r, c, 2, 1, 1, S);
					compute_evaluators(r, c, 2, -1, 1, S);
					compute_evaluators(r, c, -2, 1, 1, S);
					compute_evaluators(r, c, -2, -1, 1, S);
					compute_evaluators(r, c, 1, 2, 1, S);
					compute_evaluators(r, c, -1, 2, 1, S);
					compute_evaluators(r, c, 1, -2, 1, S);
					compute_evaluators(r, c, -1, -2, 1, S);
					break;
				case 3:  // bishop
				case -3:
					compute_evaluators(r, c, 1, 1, 7, S);
					compute_evaluators(r, c, 1, -1, 7, S);
					compute_evaluators(r, c, -1, 1, 7, S);
					compute_evaluators(r, c, -1, -1, 7, S);
					countBishops++;
					break;
				case 5:  // queen
				case -5:
					compute_evaluators(r, c, 1, 1, 7, S);
					compute_evaluators(r, c, 1, -1, 7, S);
					compute_evaluators(r, c, -1, 1, 7, S);
					compute_evaluators(r, c, -1, -1, 7, S);

					compute_evaluators(r, c, 1, 0, 7, S);
					compute_evaluators(r, c, -1, 0, 7, S);
					compute_evaluators(r, c, 0, 1, 7, S);
					compute_evaluators(r, c, 0, -1, 7, S);
					break;
				case 6: //king
				case -6:
					for (int dx = -1; dx < 2; dx++){
						for (int dy = -1; dy < 2; dy++){
							if (dx != 0 || dy != 0){
								if (S->Board.board[r][c] * S->Board.board[r + dx][c + dy] <= 0){
									int tmpPiece = S->Board.board[r + dx][c + dy];
									S->Board.board[r + dx][c + dy] = S->Board.board[r][c];
									bool impossPossible = S->kingUnderThreat(S->playerColor);
									S->Board.board[r][c] = S->Board.board[r + dx][c + dy];
									S->Board.board[r + dx][c + dy] = tmpPiece;

									if (!impossPossible)
										compute_evaluators(r, c, dx, dy, 1, S);
								}
							}
						}
					}
					break;
				case 1:  //pawn
				case -1:
					
					int pawnDirection = S->Board.whiteMoveDirection;
					if (S->Board.board[r][c] < 0)
						pawnDirection *= -1;

					compute_evaluators(r, c, pawnDirection, 0, 1,S);

					// First Double Move
					if (S->Board.board[r][c] > 0 && S->Board.whiteHome == 0 && r == 1)
						compute_evaluators(r, c, pawnDirection + 1, 0, 1,S);
					else if (S->Board.board[r][c] > 0 && S->Board.whiteHome == 7 && r == 6)
						compute_evaluators(r, c, pawnDirection - 1, 0, 1, S);
					else if (S->Board.board[r][c] <  0 && S->Board.whiteHome == 0 && r == 6)
						compute_evaluators(r, c, pawnDirection - 1, 0, 1, S);
					else if (S->Board.board[r][c] <  0 && S->Board.whiteHome == 7 && r == 1)
						compute_evaluators(r, c, pawnDirection + 1, 0, 1, S);

					// Side Pice Capture Move
					if (S->inRange(r + pawnDirection, c + 1))
					if (S->Board.board[r][c] * S->Board.board[r + pawnDirection][c + 1] < 0)
						compute_evaluators(r, c, pawnDirection, 1, 1,S);

					if (S->inRange(r + pawnDirection, c - 1))
					if (S->Board.board[r][c] * S->Board.board[r + pawnDirection][c - 1] < 0)
						compute_evaluators(r, c, pawnDirection, -1, 1, S);
				}
			}
		}
	}
}

int CalculategamePhase(gameState* S)
{
	int piece_count = 0;
	for (int i = 0; i<8; i++){
		for (int j = 0; j<8; j++)
		{
			if (S->Board.board[i][j] != 0) // piece found
				piece_count++;
		}
	}
	if (piece_count < 10){ // end game phase
		return -1;
	}
	else if (piece_count > 22) // start phase
	{
		return 1;
	}
	return 0; // middle phase
}

int evaluate_black_piece(int piece_id, int r, int c, gameState* S)
{
	int pos_row = 0 + (7 - r), pos_col = 0 + (7 - c);
	int black_score = 0; // score for current chess piece, rise if its position is good else decrease,
	
	// add number of  valid moves  to calculate mobility of particular chess piece
	black_score += validMovesCount[r][c]; /// MOBILITY SCORE ADDED
	/// weight , position_values of respective piece is added to score variable ////////////// ///POSITION SCORE ADDED
	if (piece_id == -1) // pawn
	{
		black_score += Pawn_weight; // adding weight
		black_score += pawn_table[pos_row][pos_col]; // adding position scores
		if (c == 0 || c == 7) // pawn is on edge of board, since it is not a good position so deducting score
		{
			black_score -= 15;
		}


		if (S->Board.whiteMoveDirection == -1){
			if (r == 6 && (c >= 0 && c <= 7)){
				if (attack_arr[r][c] == 0){
					black_pawn_score[((r * 8) + c) % 8] += 200;

				}
				if (attack_arr[r][c] == 0 && protect_arr[r][c] > 0)
				{
					black_pawn_score[((r * 8) + c) % 8] += 50;
				}

			}
			else if (r == 5 && (c >= 0 && c <= 7)){
				if (attack_arr[r][c] == 0){
					black_pawn_score[((r * 8) + c) % 8] += 100;

					
				}
				if (protect_arr[r][c] > 0 && attack_arr[r][c] == 0) // pawn is being protected
				{
					black_pawn_score[((r * 8) + c) % 8] += 25;
				}

			}
			else{
				black_pawn_score[((r * 8) + c) % 8] += 10;
			}
			if (black_pawn_score[((r * 8) + c) % 8] >= 20) // previously pawn exists on same column hence double pawn condition occur
			{
				black_score -= 16;/// since double pawn may hinder/ block movement effect of each other hence score is penalized
			}
		}
		else{
			if (S->Board.whiteMoveDirection == 1){
				if (r == 1 && (c >= 0 && c <= 7)){
					if (attack_arr[r][c] == 0){
						black_pawn_score[((r * 8) + c) % 8] += 200;

						
					}
					if (attack_arr[r][c] == 0 && protect_arr[r][c] > 0) // pawn is being protected
					{
						black_pawn_score[((r * 8) + c) % 8] += 50;
					}

				}
				else if (r == 2 && (c >= 0 && c <= 7)){
					if (attack_arr[r][c] == 0){
						black_pawn_score[((r * 8) + c) % 8] += 100;

						
					}
					if (attack_arr[r][c] == 0 && protect_arr[r][c] > 0) // pawn is being protected
					{
						black_pawn_score[((r * 8) + c) % 8] += 25;
					}

				}
				else{
					black_pawn_score[((r * 8) + c) % 8] += 10;
				}
				if (black_pawn_score[((r * 8) + c) % 8] >= 20) // previously pawn exists on same column hence double pawn condition occur
				{
					black_score -= 16;/// since double pawn may hinder/ block movement effect of each other hence score is penalized
				}

			}
		}
	}
	else if (piece_id == -4) // rook
	{
		black_score += rook_weight;
		if (gamePhase != 1) // game is in middle or end phase
		{
			if (S->Board.whiteMoveDirection == 1 && r <= 1) // rook is on 6th or seventh rank , its good
			{
				black_score += 10;
			}
			else if (S->Board.whiteMoveDirection == -1 && r >= 6){
				black_score += 10;
			}
		}
	}
	else if (piece_id == -2)// knight
	{
		black_score += Knight_weight;
		black_score += knight_table[pos_row][pos_col];
		if (gamePhase == -1) // end game phase so knights are not much valuable therefore subtract from score
		{
			black_score -= 10; // penalizing for score  of 10
		}
	}
	else if (piece_id == -3)// bishop
	{
		black_score += bishop_weight;
		black_score += bishop_table[pos_row][pos_col];
		if (countBishops >= 2 && gamePhase == -1){
			black_score += 20; // in end game phase bishops are more valuable and become more if they are more than double
		}
		else if (countBishops >= 2){
			black_score += 10;
		}
		else if (gamePhase == -1){
			black_score += 10;
		}
	}
	else if (piece_id == -5)// queen
	{
		black_score += queen_weight;
		if (S->Board.whiteMoveDirection == 1){
			if ((S->Board.board[7][3] != -5) && gamePhase != -1) // queens have been moved in starting game phase , its bad so deducting score
			{
				black_score -= 10;
			}

		}
		else{
			if ((S->Board.board[0][3] != -5) && gamePhase != -1) // queens have been moved in starting game phase , its bad so deducting score
			{
				black_score -= 10;
			}
		}
	}
	else if (piece_id == -6) // king
	{
		black_score += king_weight;
		
		if (gamePhase == -1){ // endPhase
			black_score += king_table_endGame[pos_row][pos_col]; // adding position value for ending game
		}
		else{
			black_score += king_table_startGame[pos_row][pos_col]; // adding position value for start game
		}
		if (validMovesCount[r][c] <= 1){ // if king has less than 2 moves than its very close to be captured,game is in losing position
			black_score -= 5;
		}

		/////////////////////if king moved and isCastled  == False  //////////  score -= 30;
	}
	// add protect or subtract attack values of that piece
	/// PIECE PROTECTED AND ATTACKED VALUES ADDED TO SCORE
	black_score += protect_arr[r][c];
	black_score -= attack_arr[r][c];
	if (attack_arr[r][c] > 0 && protect_arr[r][c] == 0) // piece is not defened but only attacked, its about to die so more score is subtracted
	{
		black_score -= (attack_arr[r][c] - protect_arr[r][c]) * 10;
	}


	//////////////////// calculate score for isolated or doubled pawn///////////////////////////
	return black_score;


}
int evaluate_white_piece(int piece_id, int r, int c, gameState* S)
{

	int white_score = 0; // score for current chess piece, rise if its position is good else decrease,
	int pos_row = r, pos_col = c;
	// add number of  valid moves  to calculate mobility of particular chess piece
	///ADDING MOBILITY SCORE
	white_score += validMovesCount[r][c];
	///////// weight , position_values of respective piece is added to score variable //////////////
	if (piece_id == 1) // pawn
	{
		white_score += Pawn_weight;
		white_score += pawn_table[pos_row][pos_col];
		if (c == 0 || c == 7) // pawn is on edge of board, since it is not a good position so deducting score
		{
			white_score -= 15;
		}
		 //////////////// adding scores for advancing pawns
		if (S->Board.whiteMoveDirection == -1){
			if (r == 1 && (c >= 0 && c <= 7)){
				if (attack_arr[r][c] == 0){
					white_pawn_score[((r * 8) + c) % 8] += 200;

					
				}
				if (attack_arr[r][c] == 0 && protect_arr[r][c] > 0) // pawn is being protected
				{
					white_pawn_score[((r * 8) + c) % 8] += 50;
				}

			}

			else if (r == 2 && (c >= 0 && c <= 7)){
				if (attack_arr[r][c] == 0){
					white_pawn_score[((r * 8) + c) % 8] += 100;

					
				}
				if (attack_arr[r][c] == 0 && protect_arr[r][c] > 0) // pawn is being protected
				{
					white_pawn_score[((r * 8) + c) % 8] += 25;
				}

			}
			else{
				white_pawn_score[((r * 8) + c) % 8] += 10;
			}
			if (white_pawn_score[((r * 8) + c) % 8] >= 20) // previously pawn exists on same column hence double pawn condition occur
			{
				white_score -= 16;/// since double pawn may hinder/ block movement effect of each other hence score is penalized
			}
		}
		else{
			if (S->Board.whiteMoveDirection == 1){
				if (r == 6 && (c >= 0 && c <= 7)){
					if (attack_arr[r][c] == 0){
						white_pawn_score[((r * 8) + c) % 8] += 200; // adding score for not being attacked 

						
					}
					if (attack_arr[r][c] == 0 && protect_arr[r][c] > 0) // pawn is being protected
					{
						white_pawn_score[((r * 8) + c) % 8] += 50; // adding bonus for being protected as well
					}

				}

				else if (r == 5 && (c >= 0 && c <= 7)){
					if (attack_arr[r][c] == 0){
						white_pawn_score[((r * 8) + c) % 8] += 100;

					}

					if (attack_arr[r][c] == 0 && protect_arr[r][c] > 0) // pawn is being protected
					{
						white_pawn_score[((r * 8) + c) % 8] += 25;
					}

				}
				else{
					white_pawn_score[((r * 8) + c) % 8] += 10;
				}
				if (white_pawn_score[((r * 8) + c) % 8] >= 20) // previously pawn exists on same column hence double pawn condition occur
				{
					white_score -= 16;/// since double pawn may hinder/ block movement effect of each other hence score is penalized
				}
			}
		}
	}
	else if (piece_id == 4) // rook
	{
		white_score += rook_weight;
		if (gamePhase != 1) // game is in middle or end phase
		{
			if (S->Board.whiteMoveDirection == 1 && r >= 6) // rook is on 6th or seventh rank , its good
			{
				white_score += 10;
			}
			else if (S->Board.whiteMoveDirection == -1 && r <= 1){
				white_score += 10;
			}
		}
	}
	else if (piece_id == 2)// knight
	{
		white_score += Knight_weight;
		white_score += knight_table[pos_row][pos_col]; //adding position advantage to score
		if (gamePhase == -1) // end game phase so knights are not much valuable therefore subtract from score
		{
			white_score -= 10; // penalizing for score  of 10
		}
	}
	else if (piece_id == 3)// bishop
	{
		white_score += bishop_weight;
		white_score += bishop_table[pos_row][pos_col];
		if (countBishops >= 2 && gamePhase == -1){
			white_score += 20; // in end game phase bishops are more valuable and become more if they are more than double
		}
		else if (countBishops >= 2){
			white_score += 10;
		}
		else if (gamePhase == -1){
			white_score += 10;
		}
	}
	else if (piece_id == 5)// queen
	{
		white_score += queen_weight;
		if (S->Board.whiteMoveDirection == 1){
			if ((S->Board.board[0][3] != 5 || S->Board.board[7][3] != -5) && gamePhase != -1) // queens have been moved in starting game phase , its bad so deducting score
			{
				white_score -= 10;
			}

		}
		else{
			if ((S->Board.board[0][3] != -5 || S->Board.board[7][3] != 5) && gamePhase != -1) // queens have been moved in starting game phase , its bad so deducting score
			{
				white_score -= 10;
			}
		}
	}
	else if (piece_id == 6) // king
	{
		white_score += king_weight;
		
		if (gamePhase == 1){ // endPhase
			white_score += king_table_startGame[pos_row][pos_col]; // adding position value for start game

		}
		else{ // end phase
			white_score += king_table_endGame[pos_row][pos_col]; // adding position value for ending game
		}
		if (validMovesCount[r][c] <= 1){ // if king has less than 2 moves than its very close to be captured,game is in losing position
			white_score -= 5;
		}

		/////////////////////if king moved and isCastled  == False  //////////  score -= 30;
	}
	// add protect or subtract attack values of that piece
	/// ADDING PROTECTED AND ATTACKED VALUE FOR PIECE
	white_score += protect_arr[r][c];
	white_score -= attack_arr[r][c];
	if (attack_arr[r][c] > 0 && protect_arr[r][c] == 0) // piece is not defened but only attacked, its about to die so more score is subtracted
	{
		white_score -= (attack_arr[r][c] - protect_arr[r][c]) * 10;
	}


	return white_score;


}

int isolatedPawnsScore(int &black_isolated_score, int  &white_isolated_score)
{

	for (int col = 0; col<8; col++) // for black pawns
	{

		switch (col){
		case 0:
			if (black_pawn_score[col] >= 10 && black_pawn_score[col + 1] == 0) // no pawn on adjacent
				black_isolated_score += 12;
			if (white_pawn_score[col] >= 10 && white_pawn_score[col + 1] == 0)
				white_isolated_score += 12;
			break;
		case 1:
			if (black_pawn_score[col] >= 10 && black_pawn_score[col + 1] == 0 && black_pawn_score[col - 1] == 0) // no pawn on adjacent
				black_isolated_score += 14;
			if (white_pawn_score[col] >= 10 && white_pawn_score[col + 1] == 0 && white_pawn_score[col - 1] == 0)
				white_isolated_score += 14;
			break;
		case 2:
			if (black_pawn_score[col] >= 10 && black_pawn_score[col + 1] == 0 && black_pawn_score[col - 1] == 0) // no pawn on adjacent
				black_isolated_score += 16;
			if (white_pawn_score[col] >= 10 && white_pawn_score[col + 1] == 0 && white_pawn_score[col - 1] == 0)
				white_isolated_score += 16;
			break;
		case 3:
			if (black_pawn_score[col] >= 10 && black_pawn_score[col + 1] == 0 && black_pawn_score[col - 1] == 0) // no pawn on adjacent
				black_isolated_score += 20;
			if (white_pawn_score[col] >= 10 && white_pawn_score[col + 1] == 0 && white_pawn_score[col - 1] == 0)
				white_isolated_score += 20;
			break;
		case 4:
			if (black_pawn_score[col] >= 10 && black_pawn_score[col + 1] == 0 && black_pawn_score[col - 1] == 0) // no pawn on adjacent
				black_isolated_score += 20;
			if (white_pawn_score[col] >= 10 && white_pawn_score[col + 1] == 0 && white_pawn_score[col - 1] == 0)
				white_isolated_score += 20;
			break;
		case 5:
			if (black_pawn_score[col] >= 10 && black_pawn_score[col + 1] == 0 && black_pawn_score[col - 1] == 0) // no pawn on adjacent
				black_isolated_score += 16;
			if (white_pawn_score[col] >= 10 && white_pawn_score[col + 1] == 0 && white_pawn_score[col - 1] == 0)
				white_isolated_score += 16;
			break;
		case 6:
			if (black_pawn_score[col] >= 10 && black_pawn_score[col + 1] == 0 && black_pawn_score[col - 1] == 0) // no pawn on adjacent
				black_isolated_score += 14;
			if (white_pawn_score[col] >= 10 && white_pawn_score[col + 1] == 0 && white_pawn_score[col - 1] == 0)
				white_isolated_score += 14;
			break;
		case 7:
			if (black_pawn_score[col] >= 10 && black_pawn_score[col - 1] == 0) // no pawn on adjacent
				black_isolated_score += 12;
			if (white_pawn_score[col] >= 10 && white_pawn_score[col - 1] == 0)
				white_isolated_score += 12;
			break;
		}

	}
	return 0;
}

int evaluate_board_score(gameState* S)
{

	int knight_count = 0, board_score = 0;
	for (int i = 0; i<8; i++)
	{

		for (int j = 0; j<8; j++)
		{

			if (S->Board.board[i][j] > 0) // piece is white
			{
				board_score += evaluate_white_piece(S->Board.board[i][j], i, j, S); 
			}
			else{
				board_score -= evaluate_black_piece(S->Board.board[i][j], i, j, S);
			}

		}
	}
	int white_isolated_score = 0, black_isolated_score = 0;
	isolatedPawnsScore(black_isolated_score, white_isolated_score);
	board_score -= white_isolated_score; // white pawns isolated, position gets weak for auto so subtracting score
	board_score += black_isolated_score; // for black isolated pawn, opponent gets weak, its good for auto so adding score 

	if (S->kingUnderThreat(White)) // black wins
		board_score -= 85; // not good , we are losing game so deducting score
	else if (S->kingUnderThreat(Black)) // white wins
		board_score += 85;  // good , we are winning , we are in goo poition so adding score 
	return board_score;
}



void copyStates(gameState* oldS, gameState* newS){

	for (int i = 0; i<8; i++){
		for (int j = 0; j<8; j++)
		{
			newS->Board.board[i][j] = oldS->Board.board[i][j];
		}
	}
	newS->playerColor = oldS->playerColor;
	newS->firstMoveOfWhiteKing = oldS->firstMoveOfWhiteKing;
	newS->firstMoveOfBlackKing = oldS->firstMoveOfBlackKing;

}

int minimax(int alpha, int beta, bool Max, int maxDepth, gameState* state)
{

	if (maxDepth == 0)  // base case
	{
		//evaluation* e = new evaluation(state);
		//return e->evaluate_board_score(state); // evaluate Board score
		computeEvaluationFeatures(state);
		return evaluate_board_score(state);
	}
	if (!Max)  // black turn // opponent
	{
		int beta_new = positiveInfinity;
		for (int i = 0; i< state->Actions.getActionCount(); i++)
		{
			gameState* state_new = new gameState();
			copyStates(state_new, state);
			state_new->computePossibleActions();
			action a;
			state->Actions.getAction(i, a);
			state_new->applyMove(a);
		/*///	state_new->computePossibleActions();*/
			beta_new = min(beta_new, minimax(alpha, beta, true, maxDepth - 1, state_new));
			beta = min(beta, beta_new);
			if (beta <= alpha) // alpha beta pruning 
			{
				break;
			}
		//	   delete state_new;
		}
		return beta_new;
	}
	else{  // white turn // auto

		int alpha_new = negativeInfinity;
		for (int i = 0; i< state->Actions.getActionCount(); i++)
		{
			gameState* state_new = new gameState();
			copyStates(state_new, state);
			state_new->computePossibleActions();
			action a;
			state->Actions.getAction(i, a);
			state_new->applyMove(a);
		//	state_new->computePossibleActions();
			alpha_new = max(alpha_new, minimax(alpha, beta, false, maxDepth - 1, state_new));
			alpha = max(alpha, alpha_new);
			if (alpha >= beta) // alpha beta pruning 
			{
				break;
			}
	//	        delete state_new;
		}
		return alpha_new;

	}
}

action evaluate_and_findBestMove(gameState* state, int maxDepth)
{
	action best_Action;
	int best_move_score = 0;

	state->computePossibleActions();
	state->Actions.getAction(0, best_Action); // let 1st move be best move;
	gameState* state_new = new gameState();
	copyStates(state_new, state);
	state_new->computePossibleActions();
	state_new->applyMove(best_Action); /// successor state
	best_move_score = minimax(negativeInfinity, positiveInfinity, true, maxDepth, state_new);
	for (int i = 1; i<state->Actions.getActionCount(); i++)
	{
		gameState* child_state = new gameState();
		copyStates(child_state, state); 
		child_state->computePossibleActions();
		action a;
		state->Actions.getAction(i, a);
		child_state->applyMove(a);
		int val = minimax(negativeInfinity, positiveInfinity, true, maxDepth, child_state);
		if (val > best_move_score)
		{
			best_move_score = val;
			state->Actions.getAction(i, best_Action);
		}
		    delete child_state; // deallocating
	}
	//   delete state_new;  // deallocating
	//delete state; // deleting internal state member
	return best_Action;
}

L1541412::L1541412(Color playerColor):chessPlayer("Auto Player Name", playerColor){


}

void L1541412::decideMove(const gameState* state, action* Move, int maxDepth ){

    /*cout<<endl<<"Please Enter a Move: from (row, col) to (row, col)"<<endl;
    cin>>(*Move).fromRow>>(*Move).fromCol>>(*Move).toRow>>(*Move).toCol;*/

	gameState temp = *state;
	
	action a = evaluate_and_findBestMove(&temp, maxDepth);
	Move->fromRow = a.fromRow;
	Move->fromCol = a.fromCol;
	Move->toCol = a.toCol;
	Move->toRow = a.toRow;
}
