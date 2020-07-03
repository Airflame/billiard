#include "../include/GameServer.h"

GameServer::GameServer() = default;

void GameServer::netLoop() {
    while (playable) {
        float x, y;
        sf::Packet packet;
        socket.receive(packet);
        packet >> x >> y;
        receivedVelocity = sf::Vector2f(x, y);
        received = true;
    }
}

void GameServer::connect() {
    std::cout << "Waiting for connection" << std::endl;
    sf::TcpListener listener;
    listener.listen(7777);
    listener.accept(socket);
    std::cout << "Connection established" << std::endl;
}

void GameServer::run() {
    srand(time(nullptr));
    sf::Clock cl;
    float dt = 0;

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

    bool drawCane = false;
    bool turn = true;
    bool movable = true;
    bool scored = false;
    bool changeable = false;
    bool pickedColor = false;
    bool isBlue;
    bool lost;

    int finalHoles[2];
    int points[2] = {0, 0};

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(1200, 600), "Billiard - Server", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::Thread netThread(&GameServer::netLoop, this);
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

        for (int i = 0; i < balls.size(); i++) {
            if (!balls[i].isHidden())
                balls[i].move(dt);
            for (int j = 0; j < balls.size(); j++) {
                if (j == i)
                    continue;
                if (balls[j].collide(balls[i].getPosition())) {
                    if (validCollisions[i][j]) {
                        sf::Vector2f v01 = balls[i].getVelocity();
                        sf::Vector2f v02 = balls[j].getVelocity();
                        sf::Vector2f x01 = balls[i].getPosition();
                        sf::Vector2f x02 = balls[j].getPosition();
                        balls[i].setVelocity(
                                v01 - (sfm::dot(v01 - v02, x01 - x02)) / (sfm::len2(x01 - x02)) * (x01 - x02));
                        balls[j].setVelocity(
                                v02 - (sfm::dot(v02 - v01, x02 - x01)) / (sfm::len2(x01 - x02)) * (x02 - x01));
                        validCollisions[i][j] = false;
                        validCollisions[j][i] = false;
                    }
                } else {
                    validCollisions[i][j] = true;
                    validCollisions[j][i] = true;
                }
            }
            sf::Packet packet;
            float x = balls[i].getPosition().x;
            float y = balls[i].getPosition().y;
            int id = 0;
            packet << id << x << y << i;
            socket.send(packet);

            for (int j = 0; j < 6; j++) {
                if (holes[j].entered(balls[i].getPosition()) and i > 0) {
                    balls[i].hide();
                    if (!pickedColor) {
                        pickedColor = true;
                        if (i >= 1 and i <= 7)
                            isBlue = turn;
                        if (i >= 9 and i <= 16)
                            isBlue = !turn;
                    }
                    if (i >= 1 and i <= 7) {
                        if ((turn and isBlue) or (!turn and !isBlue))
                            scored = true;
                        points[!isBlue]++;
                        if (points[!isBlue] == 7)
                            finalHoles[!isBlue] = (j + 3) % 6;
                    }
                    if (i >= 9 and i <= 16) {
                        if ((!turn and isBlue) or (turn and !isBlue))
                            scored = true;
                        points[isBlue]++;
                        if (points[isBlue] == 7)
                            finalHoles[isBlue] = (j + 3) % 6;
                    }
                    if (i == 8) {
                        playable = false;
                        lost = (points[!turn] == 7 and j == finalHoles[!turn]) == !turn;
                        endText.set((std::string) (lost ? "YOU LOST" : "YOU WON"));
                        endText.center(1200, 600);
                        sf::Packet winPacket;
                        id = 2;
                        winPacket << id << !lost;
                        socket.send(winPacket);
                    }
                }
            }
        }

        movable = true;
        for (auto b : balls) {
            if (sfm::len2(b.getVelocity()) != 0 and !b.isHidden())
                movable = false;
        }

        if (movable and !scored and changeable) {
            turn = !turn;
            changeable = false;
            sf::Packet turnPacket;
            int id = 1;
            turnPacket << id << !turn;
            socket.send(turnPacket);
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                netThread.terminate();
            }
            if (event.type == sf::Event::MouseButtonPressed and sqrt(sfm::len2(mousePosition - ballPosition)) <= balls[0].getRadius() and
                movable and turn) {
                cane.drawCane = true;
            }
            if (event.type == sf::Event::MouseButtonReleased and cane.drawCane) {
                balls[0].setVelocity(sf::Vector2f(-5 * cane.vec.x, -5 * cane.vec.y));
                cane.drawCane = false;
                scored = false;
                changeable = true;
            }
        }

        if (received) {
            balls[0].setVelocity(receivedVelocity);
            received = false;
            scored = false;
            changeable = true;
        }

        window.display();
        dt = cl.restart().asSeconds();
    }
}
