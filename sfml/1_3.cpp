#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Circle Moving Circle");
    sf::CircleShape circle(20);
    circle.setFillColor(sf::Color::Green);
    sf::Vector2f center(400, 300);
    float radius = 150;
    float angle = 0.0f;
    float angularSpeed = 1.0f;
    sf::Clock clock;
    
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        sf::Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        angle += angularSpeed * deltaTime.asSeconds();
        float x = center.x + radius * std::cos(angle);
        float y = center.y + radius * std::sin(angle);
        circle.setPosition(x - circle.getRadius(), y - circle.getRadius());
        window.clear();
        window.draw(circle);
        window.display();
    }

    return 0;
}