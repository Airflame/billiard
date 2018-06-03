#include "../include/Hole.h"

Hole::Hole()
{
     radius = 16;
     range = 1200;
     entity.setRadius(radius);
     entity.setFillColor(sf::Color(44, 62, 80));
     entity.setOrigin(radius,radius);
}

bool Hole::enter( sf::Vector2f arg )
{
     return sfm::len2(arg-entity.getPosition()) < range;
}
