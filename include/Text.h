#include <string>
#include "../include/VectorMath.h"

class Text
{
public:
     Text();
     void set( std::string arg );
     void center( float wx, float wy );

     sf::Text entity;

private:
     int charsize;
     sf::Font font;
     sf::FloatRect rect;
};
