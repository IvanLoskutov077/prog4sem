#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Circle Follows Mouse (b)");
    sf::CircleShape circle(50.0f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(350.0f, 250.0f);

    sf::Vector2f velocity(0.0f, 0.0f);
    float acceleration = 0.1f;
    float maxSpeed = 5.0f;
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
                velocity += direction * acceleration;
            }

            float currentSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
            if (currentSpeed > maxSpeed) {
                velocity = velocity / currentSpeed * maxSpeed;
            }
        }

        circle.move(velocity);
        window.clear();
        window.draw(circle);
        window.display();
    }

    return 0;
}