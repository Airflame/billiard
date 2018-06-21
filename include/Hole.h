#ifndef HOLE_H
#define HOLE_H
#include "../include/VectorMath.h"


class Hole
{
public:
     Hole();
     bool enter( sf::Vector2f arg );

     sf::CircleShape entity;

private:
     int radius;
     float range;
};

#endif
