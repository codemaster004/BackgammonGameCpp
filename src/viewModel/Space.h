//
// Created by Filip Dabkowski on 23/11/2023.
//

#ifndef BACKGAMMONGAME_SPACE_H
#define BACKGAMMONGAME_SPACE_H

typedef struct {
	int x;
	int y;
} Pos;

typedef struct {
	Pos min;
	Pos max;
} Placement;

Placement initBoard();

Placement initDice(Placement board);

Placement initIndex(Placement board, int offset);

Pos initCenter(Placement space);

void moveSpace(Placement *space, Pos by);

int width(Placement space);

int height(Placement space);

#endif //BACKGAMMONGAME_SPACE_H
