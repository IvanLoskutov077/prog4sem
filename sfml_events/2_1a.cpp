#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Circle Color (a)");

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(350, 250);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            circle.setFillColor(sf::Color::Red);
        } else {
            circle.setFillColor(sf::Color::Green);
        }

        window.clear();
        window.draw(circle);
        window.display();
    }

    return 0;
}