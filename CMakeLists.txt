cmake_minimum_required(VERSION 3.26)
project(BackgammonGame)

set(CMAKE_CXX_STANDARD 20)

include_directories(/opt/homebrew/opt/ncurses/include)
link_directories(/opt/homebrew/opt/ncurses/lib)

add_executable(BackgammonGame src/main.cpp
        src/ViewHandeler.cpp
        src/model/Player.cpp
        src/model/Pawn.cpp
        src/model/Board.cpp
        src/model/Pawn.h
        src/model/Board.h
        src/model/Player.h
        src/ViewHandeler.h
        src/DefineConsts.h
)

target_link_libraries(BackgammonGame ncurses)
