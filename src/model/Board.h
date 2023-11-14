//
// Created by Filip Dabkowski on 14/11/2023.
//

#ifndef BACKGAMMONGAME_BOARD_H
#define BACKGAMMONGAME_BOARD_H

#define N_BOARDS 4
#define POINTS_PER_BOARD 6
#define PAWNS_PER_POINT 5
#define PAWNS_PER_PLAYER 15
#define N_PLAYERS 2

typedef unsigned int uint;

const uint nPoints = N_BOARDS * POINTS_PER_BOARD;
const uint totalPawns = N_PLAYERS * PAWNS_PER_PLAYER;

enum PointColor {
    LIGHT,
    DARK
};

typedef struct {
    // TODO: Pawn goes here
    int pawns[PAWNS_PER_POINT];
    PointColor color;
} Point;

typedef struct {
    // TODO: Pawn goes here
    int *pawns[totalPawns];
} Bar;

typedef struct {
    // TODO: Pawn goes here
    int *pawns[PAWNS_PER_PLAYER];
    // TODO: Use player structure here
    int *owner;
} Court;

typedef struct {
    Point points[nPoints];
    Bar bar;
    Court courts[N_PLAYERS];
} Board;


#endif //BACKGAMMONGAME_BOARD_H
