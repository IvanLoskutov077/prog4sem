#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rectangle Hover (a)");
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
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (rectangle.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            rectangle.setFillColor(sf::Color::Red);
        } else {
            rectangle.setFillColor(sf::Color::Green);
        }

        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}