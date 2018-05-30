#include <vector>
#include <iostream>
#include "include/Ball.h"


int main()
{
     srand( time(NULL) );
     sf::Clock cl;
     float dt = 0;

     std::vector< Ball > balls;
     std::vector< std::vector<bool> > validcollisions( 16, std::vector<bool>(1000, true) );
     for( int i = 0; i < 16; i++ )
     {
          Ball temp;
          balls.push_back(temp);
     }
     balls[0].position = sf::Vector2f(300,300);
     balls[1].position = sf::Vector2f(854,300);
     balls[2].position = sf::Vector2f(873,289);
     balls[3].position = sf::Vector2f(873,311);
     balls[4].position = sf::Vector2f(892,279);
     balls[5].position = sf::Vector2f(892,300);
     balls[6].position = sf::Vector2f(892,321);
     balls[7].position = sf::Vector2f(911,266);
     balls[8].position = sf::Vector2f(911,288);
     balls[9].position = sf::Vector2f(911,312);
     balls[10].position = sf::Vector2f(911,334);
     balls[11].position = sf::Vector2f(930,256);
     balls[12].position = sf::Vector2f(930,278);
     balls[13].position = sf::Vector2f(930,300);
     balls[14].position = sf::Vector2f(930,322);
     balls[15].position = sf::Vector2f(930,344);
     for( int i = 1; i < 16; i++ )
          balls[i].entity.setFillColor(sf::Color::Blue);

     sf::RectangleShape cane;
     cane.setFillColor(sf::Color(100,100,100));
     cane.setSize(sf::Vector2f(100,5));

     bool drawcane = false;
     bool moveable = true;

     sf::RenderWindow window( sf::VideoMode( 1200, 600 ), "Billiard" );

     while( window.isOpen() )
     {
          window.clear( sf::Color::Black );

          moveable = true;
          for( Ball b : balls )
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
          if( canelen > 100 )
               canelen = 100;
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
          }

          sf::Event event;
          while (window.pollEvent(event))
          {
               if (event.type == sf::Event::Closed)
                    window.close();
               if( event.type == sf::Event::MouseButtonPressed and sqrt(sfm::len2(mpos-cpos)) <= 10 and moveable )
               {
                    drawcane = true;
               }
               if( event.type == sf::Event::MouseButtonReleased and drawcane )
               {
                    sf::Vector2f releasevel = sf::Vector2f(-5*canevec.x,-5*canevec.y);
                    balls[0].velocity = releasevel;
                    drawcane = false;
               }
          }
          window.display();
          dt = cl.restart().asSeconds();
     }

     return 0;
}
