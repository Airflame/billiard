#ifndef CANE_H
#define CANE_H
#include <cstdlib>
#include "../include/VectorMath.h"


class Cane
{
public:
     Cane();
     void update(sf::Vector2f mpos, sf::Vector2f cpos);

     sf::RectangleShape arm;
     sf::RectangleShape aim;
     sf::Vector2f vec;
     bool drawcane;

private:
     float len;
     float angle;
};

#endif
