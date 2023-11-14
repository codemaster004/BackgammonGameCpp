//
// Created by Filip Dabkowski on 12/11/2023.
//
// TODO: Create a structure for board (pieces(triangle thingins), "?board", bar, court (home), dies?, pawns...)
// TODO: Create a structure for keeping moves already mad in the game?
// TODO: Keep Basic Student info in struct?
// Use enum for color pallet

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
} Court;

typedef struct {
    Point points[nPoints];
    Bar bar;
    Court courts[N_PLAYERS];
} Board;
