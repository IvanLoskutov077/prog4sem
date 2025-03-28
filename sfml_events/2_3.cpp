#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

sf::Color getRandomColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> d(0, 255);
    return sf::Color{d(gen), d(gen), d(gen)};
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Circle Creator");
    window.setFramerateLimit(60);

    std::vector<sf::CircleShape> circles;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::CircleShape circle(25);
                    circle.setFillColor(sf::Color::White);
                    circle.setOrigin(circle.getRadius(), circle.getRadius());
                    circle.setPosition(event.mouseButton.x, event.mouseButton.y);                     circles.push_back(circle);
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    for (auto& circle : circles) {
                        circle.setFillColor(getRandomColor());
                    }
                }
            }

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        window.clear();
        for (const auto& circle : circles) {
            window.draw(circle);
        }
        window.display();
    }

    return 0;
}