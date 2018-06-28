#include "../include/Cane.h"

Cane::Cane()
{
     arm.setFillColor(sf::Color(150,150,100));
     arm.setSize(sf::Vector2f(100,5));
     aim.setFillColor(sf::Color(150,150,150,100));
     aim.setSize(sf::Vector2f(1500,2));
     drawcane = false;
}

void Cane::update(sf::Vector2f mpos, sf::Vector2f cpos)
{
     vec = mpos-cpos;
     len = sqrt(sfm::len2(vec));
     if(len > 200)
     {
          vec.x = 200 * vec.x/len;
          vec.y = 200 * vec.y/len;
          len = 200;
     }
     angle = atan2(vec.y,vec.x)/M_PI*180;
     arm.setSize(sf::Vector2f(len,6));
     arm.setOrigin(sf::Vector2f(0,3));
     arm.setPosition(cpos);
     arm.setRotation(angle);
     aim.setOrigin(sf::Vector2f(0,1));
     aim.setPosition(cpos);
     aim.setRotation(180+angle);
}
