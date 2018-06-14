#include "../include/Text.h"

Text::Text()
{
     charsize = 200;
     font.loadFromFile("res/Roboto-Regular.ttf");
     entity.setFont(font);
     entity.setFillColor(sf::Color::White);
     entity.setCharacterSize(100);
}

void Text::set( std::string arg )
{
     entity.setString(arg);
     rect = entity.getLocalBounds();
}

void Text::center( float wx, float wy )
{
     entity.setOrigin(rect.left+rect.width/2.0f,rect.top+rect.height/2.0f);
     entity.setPosition(wx/2,wy/2);
}
