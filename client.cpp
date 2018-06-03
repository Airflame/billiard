#include <vector>
#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "include/Ball.h"
#include "include/Hole.h"
#include "include/Cane.h"
#include "include/Endtext.h"

sf::Vector2f rp[16];
bool playable = true;
bool lost;
bool turn = false;
bool received = false;
sf::TcpSocket socket;

void netloop()
{
     while(1)
     {
          int id;
          sf::Packet packet;
          socket.receive(packet);
          packet >> id;

          switch(id)
          {
               case 0:
               float x, y;
               int n;
               packet >> x >> y >> n;
               rp[n] = sf::Vector2f(x,y);
               break;

               case 1:
               packet >> turn;
               break;

               case 2:
               packet >> lost;
               playable = false;
               break;
          }
     }
}

int main()
{
     srand(time(NULL));
     sf::Clock cl;
     float dt = 0;

     std::cout << "Enter IP address:" << std::endl;
     std::string strip;
     std::cin >> strip;
     sf::IpAddress ip(strip);
     socket.connect(ip,7777);
     std::cout << "Connection established" << std::endl;

     std::vector<Ball> balls;
     std::vector<std::vector<bool>> validcollisions(16,std::vector<bool>(1000,true));
     for( int i = 0; i < 16; i++ )
     {
          Ball temp;
          balls.push_back(temp);
     }
     balls[0].position = sf::Vector2f(300,300);
     balls[1].position = sf::Vector2f(830,300);
     balls[14].position = sf::Vector2f(857,283);
     balls[2].position = sf::Vector2f(857,317);
     balls[3].position = sf::Vector2f(884,268);
     balls[8].position = sf::Vector2f(884,300);
     balls[12].position = sf::Vector2f(884,332);
     balls[4].position = sf::Vector2f(913,246);
     balls[10].position = sf::Vector2f(913,282);
     balls[7].position = sf::Vector2f(913,318);
     balls[15].position = sf::Vector2f(913,351);
     balls[13].position = sf::Vector2f(944,234);
     balls[9].position = sf::Vector2f(944,267);
     balls[5].position = sf::Vector2f(944,300);
     balls[11].position = sf::Vector2f(944,333);
     balls[6].position = sf::Vector2f(944,366);
     for( int i = 1; i <= 7; i++ )
          balls[i].entity.setFillColor(sf::Color(41, 128, 185));
     for( int i = 9; i <= 15; i++ )
          balls[i].entity.setFillColor(sf::Color(231, 76, 60));
     balls[8].entity.setFillColor(sf::Color::Black);

     std::vector<Hole> holes;
     for( int i = 0; i < 6; i++ )
     {
          Hole temp;
          holes.push_back(temp);
          holes[i].entity.setPosition(sf::Vector2f(i%3*595+5,i/3*590+5));
     }

     Cane cane;
     Endtext endtext;

     bool drawcane = false;
     bool moveable = true;

     sf::ContextSettings settings;
     settings.antialiasingLevel = 4;

     sf::RenderWindow window( sf::VideoMode(1200,600), "Billiard - Client", sf::Style::Default, settings );
     window.setFramerateLimit(60);

     sf::Thread netthread(&netloop);
     netthread.launch();

     while( window.isOpen() )
     {
          window.clear(sf::Color(10,108,3));

          for( auto h : holes )
               window.draw(h.entity);
          for( auto b : balls )
               window.draw(b.entity);
          if( cane.drawcane )
               window.draw(cane.entity);
          if( !playable )
               window.draw(endtext.entity);

          sf::Vector2f mpos = (sf::Vector2f)sf::Mouse::getPosition(window);
          sf::Vector2f cpos = balls[0].position;
          cane.update(mpos,cpos);

          moveable = true;
          for( int i = 0; i < balls.size(); i++ )
          {
               if( balls[i].position.x > 0 and balls[i].position.y > 0 )
               {
                    sf::Vector2f oldpos = balls[i].position;
                    if( oldpos != rp[i] )
                         moveable = false;
               }
               balls[i].entity.setPosition(rp[i]);
               balls[i].position = rp[i];
          }

          sf::Event event;
          while (window.pollEvent(event))
          {
               if( event.type == sf::Event::Closed )
               {
                    window.close();
                    netthread.terminate();
               }
               if( event.type == sf::Event::MouseButtonPressed and sqrt(sfm::len2(mpos-cpos)) <= 15 and moveable and turn )
               {
                    cane.drawcane = true;
               }
               if( event.type == sf::Event::MouseButtonReleased and cane.drawcane )
               {
                    sf::Vector2f releasevel = sf::Vector2f(-5*cane.vec.x,-5*cane.vec.y);
                    cane.drawcane = false;

                    sf::Packet packet;
                    float x = releasevel.x;
                    float y = releasevel.y;
                    packet << x << y;
                    socket.send(packet);
               }
          }

          if( !playable )
          {
               endtext.set((std::string)(lost?"YOU LOST":"YOU WON"));
               endtext.center(1200,600);
          }

          window.display();
          dt = cl.restart().asSeconds();
     }

     return 0;
}
