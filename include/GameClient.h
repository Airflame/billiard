#ifndef BILLIARD_GAMECLIENT_H
#define BILLIARD_GAMECLIENT_H
#include <SFML/Network.hpp>
#include "Ball.h"
#include "Hole.h"
#include "Cane.h"
#include "Text.h"

class GameClient {
private:
    sf::Vector2f receivedPositions[16];
    bool playable = true;
    bool lost = false;
    bool turn = false;
    bool received = false;
    sf::TcpSocket socket;
    void netLoop();
    void connect();
public:
    GameClient();
    void run();
};


#endif //BILLIARD_GAMECLIENT_H
