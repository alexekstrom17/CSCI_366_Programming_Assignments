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
#include "Client.hpp"
#include "Server.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){

    //initializing player and board
    this->player = player;
    this->board_size = board_size;
    //Checking if player number is valid

    if(player < 1 || player > MAX_PLAYERS)
        throw ClientWrongPlayerNumberException();

    this->board_name = "player_ " + to_string(player) + ".action_board.json";

    vector< vector<int> > board(board_size, vector<int>(board_size));
    ofstream action_board_file(board_name);
    cereal::JSONOutputArchive archive(action_board_file);
    archive(CEREAL_NVP(board));

    initialized = true;
}

void Client::fire(unsigned int x, unsigned int y) {

    //Writing shot to players json file
    ofstream out_file("player_1.shot.json");
    cereal::JSONOutputArchive archive(out_file);
    archive(CEREAL_NVP(x),CEREAL_NVP(y));

}


bool Client::result_available() {

    //Storing players json file as string
    string f1 = "player_1.result.json";
    string f2 = "player_2.result.json";
    ifstream p1(f1);
    ifstream p2(f2);

    if (p1 || p2) {
        return true;
    } else {
        return false;
    }

}


int Client::get_result() {

    //getting result and opening player json file
    int shot;
    ifstream result("player_1.result.json");
    cereal::JSONInputArchive arch(result);
    arch(shot);

    switch(shot){
        case HIT:
            std::remove("player_1.result.json");
            std::remove("player_2.result.json");
            return HIT;
        case MISS:
            return MISS;
        case OUT_OF_BOUNDS:
            return OUT_OF_BOUNDS;
    }
    if(shot < -1 or shot > 1){
        throw ClientException("Bad Result from file");
    }
    return shot;
}

void Client::update_action_board(int result, unsigned int x, unsigned int y) {

    //Writing to action board to update it
    vector<vector<int> > vect(board_size, vector<int> (board_size,0));
    vect[y][x] = result;
    ofstream arr("player_1.action_board.json");
    cereal::JSONOutputArchive arch_write(arr);
    arch_write(cereal::make_nvp("board", vect));

}


string Client::render_action_board(){
}