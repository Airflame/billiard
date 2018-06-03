#include <string>
#include "../include/VectorMath.h"

class Endtext
{
public:
     Endtext();

     void set( std::string arg );
     void center( float wx, float wy );

     sf::Text entity;

private:
     int charsize;
     sf::Font font;
     sf::FloatRect rect;
};
