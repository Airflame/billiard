#include "include/GameClient.h"
#include "include/GameServer.h"

int main() {
    bool isServer;
    std::cout << "Run server[1] or client[0]?" << std::endl;
    std::cin >> isServer;
    if (isServer) {
        GameServer server;
        server.run();
    } else {
        GameClient client;
        client.run();
    }
}