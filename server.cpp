#include <vector>
#include <iostream>
#include <string>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "include/Ball.h"
#include "include/Hole.h"
#include "include/Cane.h"
#include "include/Text.h"

sf::Vector2f rv;
bool playable = true;
bool received = false;
sf::TcpSocket socket;

void netloop()
{
     while(1)
     {
          float x, y;
          sf::Packet packet;
          socket.receive(packet);
          packet >> x >> y;
          rv = sf::Vector2f(x,y);
          received = true;
     }
}

int main()
{
     srand(time(NULL));
     sf::Clock cl;
     float dt = 0;

     std::cout << "Waiting for connection" << std::endl;
     sf::TcpListener listener;
     listener.listen(7777);
     listener.accept(socket);
     std::cout << "Connection established" << std::endl;

     std::vector<Ball> balls(16);
     std::vector<std::vector<bool>> validcollisions(16,std::vector<bool>(1000,true));
     balls[0].setPosition(sf::Vector2f(300,300));
     balls[1].setPosition(sf::Vector2f(830,300));
     balls[14].setPosition(sf::Vector2f(857,283));
     balls[2].setPosition(sf::Vector2f(857,317));
     balls[3].setPosition(sf::Vector2f(884,268));
     balls[8].setPosition(sf::Vector2f(884,300));
     balls[12].setPosition(sf::Vector2f(884,332));
     balls[4].setPosition(sf::Vector2f(913,246));
     balls[10].setPosition(sf::Vector2f(913,282));
     balls[7].setPosition(sf::Vector2f(913,318));
     balls[15].setPosition(sf::Vector2f(913,351));
     balls[13].setPosition(sf::Vector2f(944,234));
     balls[9].setPosition(sf::Vector2f(944,267));
     balls[5].setPosition(sf::Vector2f(944,300));
     balls[11].setPosition(sf::Vector2f(944,333));
     balls[6].setPosition(sf::Vector2f(944,366));
     for(int i = 1; i <= 7; i++)
          balls[i].setFillColor(sf::Color(41, 128, 185));
     for(int i = 9; i <= 15; i++)
          balls[i].setFillColor(sf::Color(231, 76, 60));
     balls[8].setFillColor(sf::Color::Black);

     std::vector<Hole> holes(6);
     for(int i = 0; i < 6; i++)
          holes[i].setPosition(sf::Vector2f(i%3*595+5,i/3*590+5));

     Cane cane;
     Text endtext;

     bool drawcane = false;
     bool turn = true;
     bool moveable = true;
     bool scored = false;
     bool changeable = false;
     bool pickedcolor = false;
     bool isblue;
     bool lost;

     int bhole[2];
     int points[2] = {0,0};

     sf::ContextSettings settings;
     settings.antialiasingLevel = 4;

     sf::RenderWindow window(sf::VideoMode(1200,600), "Billiard - Server", sf::Style::Default, settings);
     window.setFramerateLimit(60);

     sf::Thread netthread(&netloop);
     netthread.launch();

     while(window.isOpen())
     {
          window.clear(sf::Color(10,108,3));

          for(auto h : holes)
               window.draw(h);
          for(auto b : balls)
               window.draw(b);
          if(cane.drawcane)
          {
               window.draw(cane.arm);
               window.draw(cane.aim);
          }
          if(!playable)
               window.draw(endtext);

          sf::Vector2f mpos = (sf::Vector2f)sf::Mouse::getPosition(window);
          sf::Vector2f cpos = balls[0].getPosition();
          cane.update(mpos,cpos);

          for(int i = 0; i < balls.size(); i++)
          {
               if(!balls[i].isHidden())
                    balls[i].move(dt);
               for(int j = 0; j < balls.size(); j++)
               {
                    if(j == i)
                         continue;
                    if(balls[j].collide(balls[i].getPosition()))
                    {
                         if(validcollisions[i][j])
                         {
                              sf::Vector2f v01 = balls[i].getVelocity();
                              sf::Vector2f v02 = balls[j].getVelocity();
                              sf::Vector2f x01 = balls[i].getPosition();
                              sf::Vector2f x02 = balls[j].getPosition();
                              balls[i].setVelocity( v01 - (sfm::dot(v01-v02,x01-x02)) / (sfm::len2(x01-x02)) * (x01-x02) );
                              balls[j].setVelocity( v02 - (sfm::dot(v02-v01,x02-x01)) / (sfm::len2(x01-x02)) * (x02-x01) );
                              validcollisions[i][j] = false;
                              validcollisions[j][i] = false;
                         }
                    }
                    else
                    {
                         validcollisions[i][j] = true;
                         validcollisions[j][i] = true;
                    }
               }
               sf::Packet packet;
               float x = balls[i].getPosition().x;
               float y = balls[i].getPosition().y;
               int id = 0;
               packet << id << x << y << i;
               socket.send(packet);

               for(int j = 0; j < 6; j++)
               {
                    if(holes[j].enter(balls[i].getPosition()) and i > 0)
                    {
                         balls[i].hide();
                         if(!pickedcolor)
                         {
                              pickedcolor = true;
                              if(i >= 1 and i <= 7)
                                   isblue = turn;
                              if(i >= 9 and i <= 16)
                                   isblue = !turn;
                         }
                         if(i >= 1 and i <= 7)
                         {
                              if((turn and isblue) or (!turn and !isblue))
                                   scored = true;
                              points[ !isblue ]++;
                              if(points[ !isblue ] == 7)
                                   bhole[ !isblue ] = (j + 3) % 6;
                         }
                         if(i >= 9 and i <= 16)
                         {
                              if((!turn and isblue) or (turn and !isblue))
                                   scored = true;
                              points[ isblue ]++;
                              if(points[ isblue ] == 7)
                                   bhole[ isblue ] = (j + 3) % 6;
                         }
                         if(i == 8)
                         {
                              playable = false;
                              if(points[ !turn ] == 7 and j == bhole[ !turn ])
                                   lost = !turn;
                              else
                                   lost = turn;
                              endtext.set((std::string)(lost?"YOU LOST":"YOU WON"));
                              endtext.center(1200,600);
                              sf::Packet wpacket;
                              int id = 2;
                              wpacket << id << !lost;
                              socket.send(wpacket);
                         }
                    }
               }
          }

          moveable = true;
          for(auto b : balls)
          {
               if(sfm::len2(b.getVelocity()) != 0 and !b.isHidden())
                    moveable = false;
          }

          if(moveable and !scored and changeable)
          {
               turn = !turn;
               changeable = false;
               sf::Packet tpacket;
               int id = 1;
               tpacket << id << !turn;
               socket.send(tpacket);
          }

          sf::Event event;
          while(window.pollEvent(event))
          {
               if(event.type == sf::Event::Closed)
               {
                    window.close();
                    netthread.terminate();
               }
               if(event.type == sf::Event::MouseButtonPressed and sqrt(sfm::len2(mpos-cpos)) <= balls[0].getRadius() and moveable and turn)
               {
                    cane.drawcane = true;
               }
               if(event.type == sf::Event::MouseButtonReleased and cane.drawcane)
               {
                    balls[0].setVelocity( sf::Vector2f(-5*cane.vec.x,-5*cane.vec.y) );
                    cane.drawcane = false;
                    scored = false;
                    changeable = true;
               }
          }

          if(received)
          {
               balls[0].setVelocity(rv);
               received = false;
               scored = false;
               changeable = true;
          }

          window.display();
          dt = cl.restart().asSeconds();
     }

     return 0;
}
