#include <SFML/Graphics.hpp>
#include <iostream>

int main(void) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rotating Square");

    sf::RectangleShape square(sf::Vector2f(100, 100));
    square.setFillColor(sf::Color::Green);
    square.setPosition(350, 250);
    float rotationAngle = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        square.setRotation(rotationAngle);
        rotationAngle += 1.0f;
        window.draw(square);
        window.display();
    }

    return 0;
}