#ifndef BILLIARD_GAMESERVER_H
#define BILLIARD_GAMESERVER_H
#include <SFML/Network.hpp>
#include "Ball.h"
#include "Hole.h"
#include "Cane.h"
#include "Text.h"


class GameServer {
private:
    sf::Vector2f receivedVelocity;
    bool playable = true;
    bool received = false;
    sf::TcpSocket socket;
    void netLoop();
    void connect();
public:
    GameServer();
    void run();
};


#endif
