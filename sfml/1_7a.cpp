#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Circle Follows Mouse (a)");
    sf::CircleShape circle(50.0f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(350.0f, 250.0f);

    float speed = 2.0f;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f direction = mousePos - circle.getPosition();
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance > 0) {
                direction /= distance;
                circle.move(direction * speed);
            }
        }

        window.clear();
        window.draw(circle);
        window.display();
    }

    return 0;
}