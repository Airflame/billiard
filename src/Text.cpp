#include "../include/Text.h"

Text::Text()
{
    charsize = 200;
    font.loadFromFile("res/Roboto-Regular.ttf");
    setFont(font);
    setFillColor(sf::Color::White);
    setCharacterSize(100);
}

void Text::set(std::string arg)
{
    setString(arg);
    rect = getLocalBounds();
}

void Text::center(float wx, float wy)
{
    setOrigin(rect.left+rect.width/2.0f,rect.top+rect.height/2.0f);
    setPosition(wx/2,wy/2);
}
