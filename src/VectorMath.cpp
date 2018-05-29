#include "../include/VectorMath.h"


float sfm::dot( sf::Vector2f v1, sf::Vector2f v2 )
{
     return v1.x*v2.x+v1.y*v2.y;
}

float sfm::len2( sf::Vector2f v )
{
     return v.x*v.x+v.y*v.y;
}
