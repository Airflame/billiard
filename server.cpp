#include <vector>
#include <iostream>
#include "include/Ball.h"
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <string>

sf::Vector2f rv;
bool turn = false;
bool received = false;
sf::TcpSocket socket;

void netthread()
{
     while(1)
     {
          float x, y;
          sf::Packet packet;
          socket.receive(packet);
          packet >> x >> y;
          rv = sf::Vector2f(x,y);

          turn = true;
          sf::Packet tpacket;
          int id = 1;
          tpacket << id << !turn;
          socket.send(tpacket);
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

     std::vector<Ball> balls;
     std::vector<std::vector<bool>> validcollisions(16,std::vector<bool>(1000, true));
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
          balls[i].entity.setFillColor(sf::Color::Blue);
     for( int i = 9; i <= 15; i++ )
          balls[i].entity.setFillColor(sf::Color::Red);
     balls[8].entity.setFillColor(sf::Color::Black);

     sf::RectangleShape cane;
     cane.setFillColor(sf::Color(100,100,100));
     cane.setSize(sf::Vector2f(100,5));

     sf::CircleShape holes[6];
     for( int i = 0; i < 6; i++ )
     {
          holes[i].setRadius(16);
          holes[i].setFillColor(sf::Color(40,40,40));
          holes[i].setOrigin(16,16);
          holes[i].setPosition(sf::Vector2f(i%3*595+5,i/3*590+5));
     }

     bool drawcane = false;
     bool moveable = true;

     sf::RenderWindow window( sf::VideoMode(1200,600), "Billiard - Server" );
     sf::Thread thread(&netthread);
     thread.launch();

     while( window.isOpen() )
     {
          window.clear(sf::Color(10,108,3));

          moveable = true;
          for( auto h : holes )
          {
               window.draw(h);
          }
          for( auto b : balls )
          {
               window.draw(b.entity);
               if( sfm::len2(b.velocity) != 0 )
                    moveable = false;
          }
          if( drawcane )
               window.draw(cane);

          sf::Vector2f mpos = (sf::Vector2f)sf::Mouse::getPosition(window);
          sf::Vector2f cpos = balls[0].position;
          sf::Vector2f canevec = mpos-cpos;
          float caneangle = atan2(canevec.y,canevec.x) / M_PI * 180;
          float canelen = sqrt(sfm::len2(canevec));
          if( canelen > 200 )
          {
               canevec.x = 200 * canevec.x/canelen;
               canevec.y = 200 * canevec.y/canelen;
               canelen = 200;
          }
          cane.setSize(sf::Vector2f(canelen,6));
          cane.setOrigin(sf::Vector2f(0,3));
          cane.setPosition(cpos);
          cane.setRotation(caneangle);

          for( int i = 0; i < balls.size(); i++ )
          {
               balls[i].move(dt);
               for( int j = 0; j < balls.size(); j++ )
               {
                    if( j == i )
                         continue;
                    if( balls[j].collision(balls[i].position) )
                    {
                         if( validcollisions[i][j] )
                         {
                              sf::Vector2f v01 = balls[i].velocity;
                              sf::Vector2f v02 = balls[j].velocity;
                              sf::Vector2f x01 = balls[i].position;
                              sf::Vector2f x02 = balls[j].position;
                              balls[i].velocity = v01 - (sfm::dot(v01-v02,x01-x02)) / (sfm::len2(x01-x02)) * (x01-x02);
                              balls[j].velocity = v02 - (sfm::dot(v02-v01,x02-x01)) / (sfm::len2(x01-x02)) * (x02-x01);
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
               float x = balls[i].position.x;
               float y = balls[i].position.y;
               int id = 0;
               packet << id << x << y << i;
               socket.send(packet);

               for( int j = 0; j < 6; j++ )
               {
                    if( sfm::len2( balls[i].position-holes[j].getPosition() ) < 300 )
                         balls[i].hide();
               }
          }


          sf::Event event;
          while( window.pollEvent(event) )
          {
               if( event.type == sf::Event::Closed )
               {
                    window.close();
                    thread.terminate();
               }
               if( event.type == sf::Event::MouseButtonPressed and sqrt(sfm::len2(mpos-cpos)) <= 10 and moveable and turn )
               {
                    drawcane = true;
               }
               if( event.type == sf::Event::MouseButtonReleased and drawcane )
               {
                    balls[0].velocity = sf::Vector2f(-5*canevec.x,-5*canevec.y);
                    drawcane = false;

                    turn = false;
                    sf::Packet tpacket;
                    int id = 1;
                    tpacket << id << !turn;
                    socket.send(tpacket);
               }
          }

          if( received )
          {
               balls[0].velocity = rv;
               received = false;
          }

          window.display();
          dt = cl.restart().asSeconds();
     }

     return 0;
}
