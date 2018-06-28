#ifndef TEXT_H
#define TEXT_H
#include <string>
#include "../include/VectorMath.h"

class Text : public sf::Text
{
public:
     Text();
     void set(std::string arg);
     void center(float wx, float wy);

private:
     int charsize;
     sf::Font font;
     sf::FloatRect rect;
};

#endif
