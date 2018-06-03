#include <cstdlib>
#include "../include/VectorMath.h"


class Cane
{
public:
     Cane();

     void update( sf::Vector2f mpos, sf::Vector2f cpos );

     sf::RectangleShape entity;
     sf::Vector2f vec;
     float len;
     bool drawcane;
};
