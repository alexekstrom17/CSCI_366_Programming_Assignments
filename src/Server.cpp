//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include "common.hpp"
#include "Server.hpp"

const char isCarrier ='C';
const char isBattleship = 'B';
const char iscRuiser = 'R';
const char isSubmarine = 'S';
const char isDestroyer = 'D';
const char isMiss = 'O';

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 10;

struct p1b player1;
struct p1b player2;
/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
    ifstream fileName(file.c_str(), ifstream::in | ifstream::binary);
    if(!fileName.is_open())
    {
        return - 1;
    }
    fileName.seekg(0, ios::end);
    int fileSize = fileName.tellg();
    fileName.close();

    return fileSize;
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){

    //Initializing board and opening files
    this->board_size=board_size;
    this->p1_setup_board.open("p1_setup_board.txt");
    this->p2_setup_board.open("p2_setup_board.txt");

    //Checking if file fails to open
    if (this->p1_setup_board.fail() || this->p2_setup_board.fail()) {
        throw ServerException("Can not open file");
    }

    //Checking board size to make sure it is valid
    if(board_size != BOARD_SIZE){
        throw ServerException("Incorrect Board Size");
    }
    else if (p1_setup_board.length() < 10 || p2_setup_board.length() < 10){
        throw ServerException("Incorrect Board Size");
    }
    else if (board_size==BOARD_SIZE && p1_setup_board.length() < 10 ) {
        throw ServerException("Incorrect Board Size");
    }
    else if (board_size==BOARD_SIZE && p2_setup_board.length() < 10 ) {
        throw ServerException("Incorrect Board Size");
    }
}

Server::~Server() {
}

BitArray2D *Server::scan_setup_board(string setup_board_name){
    player1_setup_board = new BitArray2D(BOARD_SIZE, BOARD_SIZE);
    player2_setup_board = new BitArray2D(BOARD_SIZE, BOARD_SIZE);

    for(int a; a < BOARD_SIZE; a++){
        for(int b; b < BOARD_SIZE; b++){
            if(player1.board[a][b] != '_'){
                player1_setup_board->set(a, b);
                cout << player1_setup_board->get(a,b);
            }
        }
    }

    for(int c; c < BOARD_SIZE; c++){
        for(int d; d < BOARD_SIZE; d++){
            if(player2.board[c][d] != '_'){
                player2_setup_board->set(c, d);
                cout << player2_setup_board->get(c,d);
            }
        }
    }

}
int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {

    char grid[BOARD_WIDTH][BOARD_HEIGHT];

    //Evaluating Player's shot
    if (player == 1 && (grid[x][y] == isCarrier || grid[x][y] == isBattleship || grid[x][y] == iscRuiser ||
                        grid[x][y] == isSubmarine || grid[x][y] == isDestroyer)) {
        return HIT;
    } else if (player == 1 && (grid[x][y] != isCarrier || grid[x][y] != isBattleship || grid[x][y] != iscRuiser ||
                               grid[x][y] != isSubmarine || grid[x][y] != isDestroyer)) {
        return MISS;
    } else if (player == 1 && (x > BOARD_HEIGHT || y > BOARD_WIDTH))
        return OUT_OF_BOUNDS;

    //Evaluating Player's shot
    if (player == 2 && (grid[x][y] == isCarrier || grid[x][y] == isBattleship || grid[x][y] == iscRuiser ||
                        grid[x][y] == isSubmarine || grid[x][y] == isDestroyer)) {
        return HIT;
    } else if (player == 2 && (grid[x][y] != isCarrier || grid[x][y] != isBattleship || grid[x][y] != iscRuiser ||
                               grid[x][y] != isSubmarine || grid[x][y] != isDestroyer)) {
        return MISS;
    } else if (player == 2 && (x > BOARD_HEIGHT || y > BOARD_WIDTH))
        return OUT_OF_BOUNDS;

    //Checking if player number is valid
    if (player > MAX_PLAYERS) {
        throw ServerException("Player Number Too High");
    } else if ( player < 1  )  {
        throw ServerException("Player Number Too Low");
    }
}

int Server::process_shot(unsigned int player, unsigned int x, unsigned int y) {

    //Checking if player number is valid
    if (player > MAX_PLAYERS) {
        throw ServerException("Player Number Too High");
    } else if ( player < 1  )  {
        throw ServerException("Player Number Too Low");
    }

    //Writing players shot to json file
    ifstream shotfile("player_" + to_string(player) +".shot.json");
    cereal::JSONInputArchive archiveIn(shotfile);
    archiveIn(CEREAL_NVP(x), CEREAL_NVP(y));

    ofstream resultfile("player_" + to_string(player) + ".result.json");
    cereal::JSONOutputArchive archiveOut(resultfile);
    int result = evaluate_shot(player, x, y);
    archiveOut(CEREAL_NVP(result));

    //Closing json file
    std::remove("player_1.shot.json");
    std::remove("player_2.shot.json");
}