//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

// Forward declaration of the Board
typedef struct Board Board;

enum PawnColor {
	WHITE,
	BLACK
};

typedef struct {
	int *owner;
	int id;
	bool isHome;
	bool isOnBar;
	PawnColor color;
	short moveDirection;
} Pawn;

bool canBeMoved(Board game, int pieceIndex, int moveBy);

#endif //BACKGAMMONGAME_PAWN_H
