#include <cstdlib>
#include <iostream>
#include "../include/VectorMath.h"


class Ball
{
public:
     Ball();

     bool collision( sf::Vector2f arg );
     void move( float dt );
     void accelerate( float dt, float value );
     void hide();

     int radius = 15;
     sf::CircleShape entity;
     sf::Vector2f position;
     sf::Vector2f velocity;

private:
     bool wallbounce[4];
};
