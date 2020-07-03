#include "../include/GameClient.h"

GameClient::GameClient() = default;

void GameClient::netLoop() {
    while (playable) {
        int id;
        sf::Packet packet;
        socket.receive(packet);
        packet >> id;

        switch (id) {
            case 0:
                float x, y;
                int n;
                packet >> x >> y >> n;
                receivedPositions[n] = sf::Vector2f(x, y);
                break;

            case 1:
                packet >> turn;
                break;

            case 2:
                packet >> lost;
                playable = false;
                break;
        }
    }
}

void GameClient::connect() {
    std::cout << "Enter IP address:" << std::endl;
    std::string strIp;
    std::cin >> strIp;
    sf::IpAddress ip(strIp);
    socket.connect(ip, 7777);
    std::cout << "Connection established" << std::endl;
}

void GameClient::run() {
    srand(time(nullptr));

    connect();

    std::vector<Ball> balls(16);
    std::vector<std::vector<bool>> validCollisions(16, std::vector<bool>(1000, true));
    balls[0].setPosition(sf::Vector2f(300, 300));
    balls[1].setPosition(sf::Vector2f(830, 300));
    balls[14].setPosition(sf::Vector2f(857, 283));
    balls[2].setPosition(sf::Vector2f(857, 317));
    balls[3].setPosition(sf::Vector2f(884, 268));
    balls[8].setPosition(sf::Vector2f(884, 300));
    balls[12].setPosition(sf::Vector2f(884, 332));
    balls[4].setPosition(sf::Vector2f(913, 246));
    balls[10].setPosition(sf::Vector2f(913, 282));
    balls[7].setPosition(sf::Vector2f(913, 318));
    balls[15].setPosition(sf::Vector2f(913, 351));
    balls[13].setPosition(sf::Vector2f(944, 234));
    balls[9].setPosition(sf::Vector2f(944, 267));
    balls[5].setPosition(sf::Vector2f(944, 300));
    balls[11].setPosition(sf::Vector2f(944, 333));
    balls[6].setPosition(sf::Vector2f(944, 366));
    for (int i = 1; i <= 7; i++)
        balls[i].setFillColor(sf::Color(41, 128, 185));
    for (int i = 9; i <= 15; i++)
        balls[i].setFillColor(sf::Color(231, 76, 60));
    balls[8].setFillColor(sf::Color::Black);

    std::vector<Hole> holes(6);
    for (int i = 0; i < 6; i++)
        holes[i].setPosition(sf::Vector2f(i % 3 * 595 + 5, i / 3 * 590 + 5));

    Cane cane;
    Text endText;

    bool movable;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1200, 600), "Billiard - Client", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::Thread netThread(&GameClient::netLoop, this);
    netThread.launch();

    while (window.isOpen()) {
        window.clear(sf::Color(10, 108, 3));

        for (const auto& h : holes)
            window.draw(h);
        for (const auto& b : balls)
            window.draw(b);
        if (cane.drawCane) {
            window.draw(cane.arm);
            window.draw(cane.aim);
        }
        if (!playable)
            window.draw(endText);

        sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);
        sf::Vector2f ballPosition = balls[0].getPosition();
        cane.update(mousePosition, ballPosition);

        movable = true;
        for (int i = 0; i < balls.size(); i++) {
            if (!balls[i].isHidden()) {
                sf::Vector2f oldPosition = balls[i].getPosition();
                if (oldPosition != receivedPositions[i])
                    movable = false;
            }
            balls[i].setPosition(receivedPositions[i]);
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                netThread.terminate();
            }
            if (event.type == sf::Event::MouseButtonPressed and sqrt(sfm::len2(mousePosition - ballPosition)) <= 15 and movable and
                turn) {
                cane.drawCane = true;
            }
            if (event.type == sf::Event::MouseButtonReleased and cane.drawCane) {
                sf::Vector2f releaseVelocity = sf::Vector2f(-5 * cane.vec.x, -5 * cane.vec.y);
                cane.drawCane = false;

                sf::Packet positionPacket;
                float x = releaseVelocity.x;
                float y = releaseVelocity.y;
                positionPacket << x << y;
                socket.send(positionPacket);
            }
        }

        if (!playable) {
            endText.set((std::string) (lost ? "YOU LOST" : "YOU WON"));
            endText.center(1200, 600);
        }

        window.display();
    }
}
