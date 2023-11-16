//
// Created by Filip Dabkowski on 16/11/2023.
//

#ifndef BACKGAMMONGAME_DEFINECONSTS_H
#define BACKGAMMONGAME_DEFINECONSTS_H

#define N_BOARDS 4
#define POINTS_PER_BOARD 6
#define PAWNS_PER_POINT 5
#define PAWNS_PER_PLAYER 15
#define N_PLAYERS 2
#define PLAYERS_PER_GAME 2
#define N_DICES 2

typedef unsigned int uint;

const uint nPoints = N_BOARDS * POINTS_PER_BOARD;
const uint totalPawns = N_PLAYERS * PAWNS_PER_PLAYER;


#endif //BACKGAMMONGAME_DEFINECONSTS_H
