#include "../include/Ball.h"

Ball::Ball()
{
     hidden = false;
     setRadius(15);
     setOrigin(15,15);
     setFillColor(sf::Color(236, 240, 241));
     for( int i = 0; i < 4; i++ )
          wallbounce[i] = true;
     velocity = sf::Vector2f(0,0);
}

void Ball::setVelocity( sf::Vector2f arg )
{
     velocity = arg;
}

sf::Vector2f Ball::getVelocity()
{
     return velocity;
}

bool Ball::collide( sf::Vector2f arg )
{

     return sfm::len2(getPosition()-arg) <= 4*getRadius()*getRadius()+1;
}

void Ball::move( float dt )
{
     float loss = 0.8;
     sf::Vector2f position = getPosition();
     if( position.x > 1200-getRadius() )
     {
          if( wallbounce[0] )
          {
               velocity.x *= -1;
               velocity.x *= loss;
               velocity.y *= loss;
               wallbounce[0] = false;
          }
     }
     else
          wallbounce[0] = true;
     if( position.x < getRadius() )
     {
          if( wallbounce[2] )
          {
               velocity.x *= -1;
               velocity.x *= loss;
               velocity.y *= loss;
               wallbounce[2] = false;
          }
     }
     else
          wallbounce[2] = true;
     if( position.y > 600-getRadius() )
     {
          if( wallbounce[4] )
          {
               velocity.y *= -1;
               velocity.x *= loss;
               velocity.y *= loss;
               wallbounce[4] = false;
          }
     }
     else
          wallbounce[4] = true;
     if( position.y < getRadius() )
     {
          if( wallbounce[1] )
          {
               velocity.y *= -1;
               velocity.x *= loss;
               velocity.y *= loss;
               wallbounce[1] = false;
          }
     }
     else
          wallbounce[1] = true;
     position.x += velocity.x*dt;
     position.y += velocity.y*dt;
     setPosition(position);
     accelerate(dt,-100);
}

void Ball::accelerate( float dt, float value )
{
     float x = 0;
     float y = 0;
     if( sfm::len2(velocity) != 0 )
     {
          x = fabs( velocity.x/sqrt(velocity.x*velocity.x+velocity.y*velocity.y) );
          y = fabs( velocity.y/sqrt(velocity.x*velocity.x+velocity.y*velocity.y) );
     }

     if( fabs(value*x*dt) > fabs(velocity.x) )
          velocity.x = 0;
     else
     {
          if( velocity.x > 0 )
          {
               velocity.x += value*x*dt;
          }
          else
               velocity.x -= value*x*dt;
     }

     if( fabs(value*y*dt) > fabs(velocity.y) )
          velocity.y = 0;
     else
     {
          if( velocity.y > 0 )
          {
               velocity.y += value*y*dt;
          }
          else
               velocity.y -= value*y*dt;
     }
}

void Ball::hide()
{
     hidden = true;
     setPosition(sf::Vector2f(-100,100));
     velocity = sf::Vector2f(0,0);
}
