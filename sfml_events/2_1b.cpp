#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Circle Color (b)");
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(350, 250);

    bool isSpacePressed = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                isSpacePressed = true;
                circle.setFillColor(sf::Color::Red);
            }

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                isSpacePressed = false;
                circle.setFillColor(sf::Color::Green);
            }
        }

        window.clear();
        window.draw(circle);
        window.display();
    }

    return 0;
}