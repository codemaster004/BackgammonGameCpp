cmake_minimum_required(VERSION 3.26)
project(BackgammonGame)

set(CMAKE_CXX_STANDARD 20)

include_directories(/opt/homebrew/opt/ncurses/include)
link_directories(/opt/homebrew/opt/ncurses/lib)

add_executable(BackgammonGame src/main.cpp
        src/view/Handeler.cpp
        src/model/Player.cpp
        src/model/Pawn.cpp
        src/model/Board.cpp
        src/model/Pawn.h
        src/model/Board.h
        src/model/Player.h
        src/view/Handeler.h
        src/configs/GameConfigs.h
        src/configs/UIConfigs.h
        src/Utility.h
        src/Utility.cpp
        src/viewModel/UserInterface.cpp
        src/viewModel/UserInterface.h
        src/controller/InputControll.cpp
        src/controller/InputControll.h
        src/view/Drawing.cpp
        src/view/Drawing.h
        src/viewModel/Space.cpp
        src/viewModel/Space.h
        src/model/SerializeToFile.cpp
        src/model/SerializeToFile.h
        src/model/Storage.cpp
        src/model/Storage.h
        src/Base64.cpp
        src/Base64.h
        src/configs/MenuConfigs.h
        src/model/History.cpp
        src/model/History.h
        src/ByteContainer.cpp
        src/ByteContainer.h
)

target_link_libraries(BackgammonGame ncurses)
