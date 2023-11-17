//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_PAWN_H
#define BACKGAMMONGAME_PAWN_H

enum PawnColor {
	WHITE,
	RED
};

typedef struct {
	PawnColor color;
	int *owner;
	bool isHome;
	bool isOnBar;
	const short moveDirection;
} Pawn;


#endif //BACKGAMMONGAME_PAWN_H
