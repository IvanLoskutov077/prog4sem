#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class DropList {
public:
    DropList(float x, float y, float width, float height, sf::Font& font, std::vector<std::string> items)
        : m_items(items), m_selectedItem(0), m_isOpen(false) {
        m_background.setSize(sf::Vector2f(width, height));
        m_background.setPosition(x, y);
        m_background.setFillColor(sf::Color::White);
        m_background.setOutlineThickness(1);
        m_background.setOutlineColor(sf::Color::Black);

        m_text.setFont(font);
        m_text.setCharacterSize(20);
        m_text.setFillColor(sf::Color::Black);
        m_text.setPosition(x + 10, y + (height - 20) / 2);

        m_dropDown.setSize(sf::Vector2f(width, height * items.size()));
        m_dropDown.setPosition(x, y + height);
        m_dropDown.setFillColor(sf::Color::White);
        m_dropDown.setOutlineThickness(1);
        m_dropDown.setOutlineColor(sf::Color::Black);

        for (size_t i = 0; i < items.size(); ++i) {
            sf::Text itemText;
            itemText.setFont(font);
            itemText.setCharacterSize(20);
            itemText.setFillColor(sf::Color::Black);
            itemText.setPosition(x + 10, y + height + i * height + (height - 20) / 2);
            itemText.setString(items[i]);
            m_itemTexts.push_back(itemText);
        }

        updateText();
    }

    void handleEvent(sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (m_background.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                m_isOpen = !m_isOpen;
            } else if (m_isOpen) {
                for (size_t i = 0; i < m_itemTexts.size(); ++i) {
                    if (m_itemTexts[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        m_selectedItem = i;
                        m_isOpen = false;
                        updateText();
                        break;
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(m_background);
        window.draw(m_text);

        if (m_isOpen) {
            window.draw(m_dropDown);
            for (auto& itemText : m_itemTexts) {
                window.draw(itemText);
            }
        }
    }

    int getSelectedItem() const {
        return m_selectedItem;
    }

private:
    void updateText() {
        m_text.setString(m_items[m_selectedItem]);
    }

    sf::RectangleShape m_background;
    sf::RectangleShape m_dropDown;
    sf::Text m_text;
    std::vector<sf::Text> m_itemTexts;
    std::vector<std::string> m_items;
    int m_selectedItem;
    bool m_isOpen;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Drop List");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    std::vector<std::string> items = {"Круг", "Квадрат", "Треугольник"};
    DropList dropList(50, 50, 150, 30, font, items);

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(300, 250);

    sf::RectangleShape square(sf::Vector2f(100, 100));
    square.setFillColor(sf::Color::Blue);
    square.setPosition(300, 250);

    sf::CircleShape triangle(50, 3);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition(300, 250);

    sf::Shape* currentShape = &circle;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            dropList.handleEvent(event);

            switch (dropList.getSelectedItem()) {
                case 0:
                    currentShape = &circle;
                    break;
                case 1:
                    currentShape = &square;
                    break;
                case 2:
                    currentShape = &triangle;
                    break;
            }
        }


        window.clear(sf::Color::White);
        dropList.draw(window);
        window.draw(*currentShape);
        window.display();
    }

    return 0;
}