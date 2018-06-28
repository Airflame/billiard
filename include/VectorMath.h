#ifndef VECTORMATH_H
#define VECTORMATH_H
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <SFML/Graphics.hpp>
#include <cmath>


namespace sfm
{
     float dot(sf::Vector2f v1, sf::Vector2f v2);
     float len2(sf::Vector2f v);
}

#endif
