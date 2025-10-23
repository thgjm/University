#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>

int TriangleArray(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
{
    float S = 0.5*abs( a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y) );
    return 2*S;
}

bool isPointOnTheLine ( sf::Vector2f a, sf::Vector2f b, sf::Vector2f p )
{
    if( (p.y-a.y)/(p.x-a.x) == (b.y-a.y)/(b.x-a.x) ) return true;
    else return false;
}

bool isPointInside ( sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f p )
{
    int arr1=TriangleArray(a, b, p);
    int arr2=TriangleArray(b, c, p);
    int arr3=TriangleArray(a, c, p);
    int arr=TriangleArray(a, b, c);
    if(arr==arr1+arr2+arr3) return true;
    else return false;
}


int main()
{
    int x1,x2,x3,y1,y2,y3,px1,py1;
    std::cout<<"Enter 4 pairs of numbers: 3 for triangle vertexes and one for a point"<<std::endl;
    std::cin>>x1>>y1>>x2>>y2>>x3>>y3>>px1>>py1;

    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1000, 800), "lab2_1");

    sf::VertexArray triangle(sf::Triangles, 3);

    triangle[0].position = sf::Vector2f(x1, y1);
    triangle[1].position = sf::Vector2f(x2, y2);
    triangle[2].position = sf::Vector2f(x3, y3);

    triangle[0].color = sf::Color::White;
    triangle[1].color = sf::Color::White;
    triangle[2].color = sf::Color::White;


    sf::CircleShape point(3);
    point.setPosition(px1, py1);
    point.setFillColor(sf::Color::Red);


    if(isPointInside(triangle[0].position, triangle[1].position, triangle[2].position, point.getPosition()))
    {
        if(isPointOnTheLine(triangle[0].position, triangle[1].position, point.getPosition()) || isPointOnTheLine(triangle[1].position, triangle[2].position, point.getPosition()) || isPointOnTheLine(triangle[0].position, triangle[2].position, point.getPosition())) std::cout<<"point on the line of the triangle";
        else std::cout<<"point is inside the triangle";
    }
    else std::cout<<"point is outside of the triangle";



    // Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        // Clear screen
        app.clear();


        app.draw(triangle);

        app.draw(point);


        // Update the window
        app.display();



    }

    return EXIT_SUCCESS;
}
