#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class Toggle {
private:
    sf::RectangleShape background;
    sf::CircleShape button;
    bool isOn;
    std::function<void(bool)> callback;

public:
    Toggle(float x, float y, float width, float height, std::function<void(bool)> callback = nullptr)
        : isOn(false), callback(callback) {
        background.setSize(sf::Vector2f(width, height));
        background.setPosition(x, y);
        background.setFillColor(sf::Color(150, 150, 150));

        float buttonRadius = height / 2;
        button.setRadius(buttonRadius);
        button.setPosition(x, y);
        button.setFillColor(sf::Color::White);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(button);
    }

    void handleClick(int mouseX, int mouseY) {
        if (background.getGlobalBounds().contains(mouseX, mouseY)) {
            isOn = !isOn;
            updateButtonPosition();

            if (callback) {
                callback(isOn);
            }
        }
    }

    void updateButtonPosition() {
        float buttonX = isOn ? background.getPosition().x + background.getSize().x - button.getRadius() * 2
                             : background.getPosition().x;
        button.setPosition(buttonX, background.getPosition().y);
    }

    void setPosition(float x, float y) {
        background.setPosition(x, y);
        updateButtonPosition();
    }

    bool getState() const {
        return isOn;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Toggle Buttons");
    std::vector<Toggle> toggles;
    float toggleWidth = 60;
    float toggleHeight = 30;
    float spacing = 20;

    for (int i = 0; i < 10; ++i) {
        float x = 50;
        float y = 50 + i * (toggleHeight + spacing);
        toggles.emplace_back(x, y, toggleWidth, toggleHeight, [i](bool state) {
            std::cout << "Toggle " << i << " is " << (state ? "ON" : "OFF") << std::endl;
        });
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (auto& toggle : toggles) {
                        toggle.handleClick(event.mouseButton.x, event.mouseButton.y);
                    }
                }
            }

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        window.clear(sf::Color::Black);
        for (auto& toggle : toggles) {
            toggle.draw(window);
        }
        window.display();
    }

    return 0;
}