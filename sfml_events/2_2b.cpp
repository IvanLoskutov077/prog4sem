#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rectangle Hover (b)");
    window.setFramerateLimit(60);

    sf::RectangleShape rectangle(sf::Vector2f(200, 100));
    rectangle.setPosition(300, 250);
    rectangle.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            if (event.type == sf::Event::MouseMoved) {
                if (rectangle.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    rectangle.setFillColor(sf::Color::Red);
                } else {
                    rectangle.setFillColor(sf::Color::Green);
                }
            }
        }

        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}