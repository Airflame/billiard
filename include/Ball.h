#include <cstdlib>
#include <iostream>
#include "../include/VectorMath.h"


class Ball
{
public:
     Ball();

     bool collision( sf::Vector2f );
     void move( float dt );
     void accelerate( float dt, float value );

     int radius = 10;
     bool wallbounce[4];
     sf::CircleShape entity;
     sf::Vector2f position;
     sf::Vector2f velocity;
};
