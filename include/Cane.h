#ifndef CANE_H
#define CANE_H
#include <cstdlib>
#include "../include/VectorMath.h"


class Cane
{
public:
    Cane();
    void update(sf::Vector2f mousePosition, sf::Vector2f ballPosition);

    sf::RectangleShape arm;
    sf::RectangleShape aim;
    sf::Vector2f vec;
    bool drawCane;

private:
    float len;
    float angle;
};

#endif
