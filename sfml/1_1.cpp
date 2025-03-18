#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Moving Circle");
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(0, 300);
    float speed = 200.0f;
    sf::Clock clock;
    
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float moveAmount = speed * deltaTime.asSeconds();
        circle.move(moveAmount, 0);

        if (circle.getPosition().x > window.getSize().x) {
            circle.setPosition(-circle.getRadius() * 2, circle.getPosition().y);
        }

        window.clear();
        window.draw(circle);
        window.display();
    }
    return 0;
}