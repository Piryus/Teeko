#ifndef TEEKO_GAME_H
#define TEEKO_GAME_H


#include <SFML/Graphics.hpp>
#include "Player.h"
#include <vector>
#include <algorithm>

class Game {
public:
    Game(bool is_ai);
    void initGame();
    void PlaceMarker(Space space, int player);
    void MoveMarker(Space current_space, Space next_space, int player);
    vector<Player>* GetPlayers();
    vector<Space>* GetSpaces();
    vector<Marker>* GetMarkers();
    int GetTurnNumber();
    int GetPlayerTurn();
    Player* GetWinner();
    bool IsAIGame();

private:
    vector<Space> spaces_;
    vector<Marker> markers_;
    vector<Player> players_;
    int turn_; // Player 1 or 2
    int turn_number_;
    Player* winner_ = nullptr;
    bool ai_game_;
};

#endif //TEEKO_GAME_H
