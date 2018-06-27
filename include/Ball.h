#ifndef BALL_H
#define BALL_H
#include <cstdlib>
#include <iostream>
#include "../include/VectorMath.h"


class Ball : public sf::CircleShape
{
public:
     Ball();
     void setVelocity( sf::Vector2f arg );
     sf::Vector2f getVelocity();
     bool collide( sf::Vector2f arg );
     void move( float dt );
     void accelerate( float dt, float value );
     void hide();

     bool hidden;

private:
     sf::Vector2f velocity;
     bool wallbounce[4];
};

#endif
