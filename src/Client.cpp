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
    if (player > MAX_PLAYERS) {
        throw ServerException("Player Number Too High");
    } else if ( player < 1  )  {
        throw ServerException("Player Number Too Low");
    }

    this->board_name = "player_" + to_string(player) + ".action_board.json";
    vector<vector<int>> board(board_size, vector<int>(board_size) );
    cereal::JSONOutputArchive archive(board);

    archive(CEREAL_NVP(board));
    this->initialized = true;

}


void Client::fire(unsigned int x, unsigned int y) {

    //Writing shot to players json file
    this->board_name = "player_" + to_string(player) + ".shot.json";      //Vector ffor action board
    ofstream actfile(board_name);
    cereal::JSONOutputArchive archive(actfile);
    archive(CEREAL_NVP(x), CEREAL_NVP(y));

    //Printing shot to user
    cout << "Shot fired at " + to_string(x) + ", " + to_string(y);
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
    int result;
    ifstream resultfile("player_" + toString(player) + ".result.json");
    cereal::JSONInputArchive archive(resultfile);
    archive(CEREAL_NVP(result));

    //Configuring results
    if(HIT == true) {
        std::remove("player_" + toString(player) + ".result.json");
    } else if (MISS == true) {
        return MISS;
    } else if (OUT_OF_BOUNDS == true) {
        return OUT_OF_BOUNDS;
    } else {
        continue;
    }
}
void Client::update_action_board(int result, unsigned int x, unsigned int y) {

    //Writing to action board to update it
    vector<vector<int>> actVector;
    fstream actfile("player_" + toString(player) + ".action_board.json");
    cereal::JSONInputArchive inarchive(actfile);
    inarchive(actVector);
    actVector[x][y] = result;

    ofstream outfile("player_" + toString(player) + ".action_board.json");
    cereal::JSONOutputArchive outArchive(outfile);
    outArchive(cereal::make_nvp("board", actVector));

}

string Client::render_action_board(){
}