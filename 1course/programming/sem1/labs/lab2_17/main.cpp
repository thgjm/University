#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;
using namespace sf;



int main()
{

    srand(time(NULL));

    // Create the main window
    RenderWindow app(sf::VideoMode(1000, 800), "SFML window");

    CircleShape circles[40];
    Vector2f velocities[40];

    for(int i=0; i<40; i++)
    {
        circles[i].setRadius(10.0f);
        circles[i].setFillColor(sf::Color::White);
        float x=rand() % 980;
        float y=rand() % 780;
        circles[i].setPosition(x,y);
        float randomSpeedX = (rand() % 21 + 10) / 100.0f;
        float randomSpeedY = (rand() % 21 + 10) / 100.0f;
        velocities[i] = sf::Vector2f(randomSpeedX, randomSpeedY);
    }


    // Start the game loop
    while (app.isOpen())
    {
        // Process events
        Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        for(int i=0; i<40; i++)
        {
            circles[i].move(velocities[i]);

            if (circles[i].getPosition().x < 0 || circles[i].getPosition().x + circles[i].getRadius() * 2 > app.getSize().x)

                velocities[i].x = -velocities[i].x;

            if (circles[i].getPosition().y < 0 || circles[i].getPosition().y + circles[i].getRadius() * 2 > app.getSize().y)

                velocities[i].y = -velocities[i].y;

            for(int j=0; j<40; j++)

                if(j!=i)
                {
                    if(sqrt(pow(circles[i].getPosition().x-circles[j].getPosition().x, 2)+pow(circles[i].getPosition().y-circles[j].getPosition().y, 2)) <= ( circles[i].getRadius() + circles[i].getRadius() ) )
                    {
                        Vector2f center1=circles[i].getPosition() + Vector2f(circles[i].getRadius(), circles[i].getRadius());
                        Vector2f center2=circles[j].getPosition() + Vector2f(circles[j].getRadius(), circles[j].getRadius());
                        Vector2f distanceCircles=center2-center1;
                        Vector2f basis1=distanceCircles;
                        Vector2f basis2=distanceCircles/sqrt(distanceCircles.x*distanceCircles.x+distanceCircles.y*distanceCircles.y);
                        Vector2f vel1X(velocities[i].x, 0.0f), vel1Y(0.0f, velocities[i].y), vel2X(velocities[j].x, 0.0f), vel2Y(0.0f, velocities[j].y);
                        Vector2f velNew1X(vel1X.x*basis1.x + vel1X.y*basis2.x, vel1X.x*basis1.y + vel1X.y*basis2.y), velNew2X(vel2X.x*basis1.x + vel2X.y*basis2.x, vel2X.x*basis1.y + vel2X.y*basis2.y);
                        vel1X.x=velNew2X.x*1.0f+velNew2X.y*0.0f;
                        vel1X.y= velNew2X.x*0.0f+velNew2X.y*1.0f;
                        vel2X.x=velNew1X.x*1.0f+velNew1X.y*0.0f;
                        vel2X.y = velNew1X.x*0.0f+velNew1X.y*1.0f;
                        velocities[i]=vel1X+vel1Y;
                        velocities[j]=vel2X + vel2Y;

                    }
                }

        }


        // Clear screen
        app.clear();

        for(int i=0; i<40; i++)
            app.draw(circles[i]);

        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;

}
