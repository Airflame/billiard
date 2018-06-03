#include "../include/Cane.h"

Cane::Cane()
{
     entity.setFillColor(sf::Color(100,100,100));
     entity.setSize(sf::Vector2f(100,5));
     drawcane = false;
}


void Cane::update( sf::Vector2f mpos, sf::Vector2f cpos )
{
     vec = mpos-cpos;
     len = sqrt(sfm::len2(vec));
     if( len > 200 )
     {
          vec.x = 200 * vec.x/len;
          vec.y = 200 * vec.y/len;
          len = 200;
     }
     entity.setSize(sf::Vector2f(len,6));
     entity.setOrigin(sf::Vector2f(0,3));
     entity.setPosition(cpos);
     entity.setRotation(atan2(vec.y,vec.x)/M_PI*180);
}
