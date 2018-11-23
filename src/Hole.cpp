#include "../include/Hole.h"

Hole::Hole()
{
    radius = 16;
    range = 1200;
    setRadius(radius);
    setFillColor(sf::Color(44, 62, 80));
    setOrigin(radius,radius);
}

bool Hole::enter(sf::Vector2f arg)
{
    return sfm::len2(arg-getPosition()) < range;
}
