#include <iostream>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Draggable {
private:
    sf::RectangleShape mShape   {};
    bool mIsDragged             {false};
    sf::Vector2f mOffset        {0.0f, 0.0f};

public:
    Draggable() {};
    Draggable(const sf::RectangleShape& shape) : mShape{shape} {}
    Draggable(sf::Vector2f position, sf::Vector2f size, sf::Color color) {
        mShape.setPosition(position);
        mShape.setSize(size);
        mShape.setFillColor(color);
    }

    bool onMousePressed(sf::Vector2f mousePosition) {
        if (mShape.getGlobalBounds().contains(mousePosition)) {
            mIsDragged = true;
            mOffset = mousePosition - mShape.getPosition();
        }
        return mIsDragged;
    }

    void onMouseReleased() {
        mIsDragged = false;
    }

    void onMouseMoved(sf::Vector2f mousePosition) {
        if (mIsDragged)
            mShape.setPosition(mousePosition - mOffset);
    }

    void setColor(sf::Color c) { mShape.setFillColor(c); }
    void setSize(sf::Vector2f sz) { mShape.setSize(sz); }
    void setPosition(sf::Vector2f p) { mShape.setPosition(p); }
    void draw(sf::RenderWindow& rw) const{ rw.draw(mShape); }
};

class DraggableWithColorChange : public Draggable {
private:
    sf::Color mBaseColor;
    sf::Color mDragColor;

public:
    DraggableWithColorChange(sf::Vector2f position, sf::Vector2f size, sf::Color baseColor, sf::Color dragColor)
        : Draggable(position, size, baseColor), mBaseColor(baseColor), mDragColor(dragColor) {}

    void onMousePressed(sf::Vector2f mousePosition) {
        if (Draggable::onMousePressed(mousePosition))
            setColor(mDragColor);
    }

    void onMouseReleased() {
        Draggable::onMouseReleased();
        setColor(mBaseColor);
    }
};

int main(void) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Draggable with Color Change");
    window.setFramerateLimit(60);

    DraggableWithColorChange a({100, 100}, {200, 400}, sf::Color(80, 80, 120), sf::Color(255, 0, 0));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                a.onMousePressed(mousePosition);
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                a.onMouseReleased();
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
                a.onMouseMoved(mousePosition);
            }
        }

        window.clear(sf::Color::Black);
        a.draw(window);
        window.display();
    }
}