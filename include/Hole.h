#ifndef HOLE_H
#define HOLE_H
#include "../include/VectorMath.h"


class Hole : public sf::CircleShape
{
public:
    Hole();
    bool enter(sf::Vector2f arg);

private:
    int radius;
    float range;
};

#endif
