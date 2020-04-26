cmake_minimum_required(VERSION 3.15)
project(CSCI_366_Programming_Assignments)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cereal)
include_directories(dtl)
include_directories(googletest)
include_directories(images)
include_directories(src)
include_directories(test)

add_executable(CSCI_366_Programming_Assignments
        images/system_architecture.png
        src/BitArray2D.asm
        src/BitArray2D.cpp
        src/BitArray2D.hpp
        src/Client.cpp
        src/Client.hpp
        src/client_main.cpp
        src/CMakeLists.txt
        src/common.hpp
        src/Server.cpp
        src/Server.hpp
        src/server_main.cpp
        test/CMakeLists.txt
        test/correct_fire_message.json
        test/correct_hit_action_board.json
        test/correct_hit_result.json
        test/correct_miss_action_board.json
        test/correct_miss_result.json
        test/correct_out_of_bounds_result.json
        test/correct_result_message.json
        test/correct_start_action_board.json
        test/tests.cpp
        CMakeLists.txt
        LICENSE.md
        player_1.setup_board.txt
        player_2.setup_board.txt
        README.md)
