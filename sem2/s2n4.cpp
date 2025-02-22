#include <iostream>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "button.hpp"


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

    void onMouseReleased() { mIsDragged = false; }

    void onMouseMoved(sf::Vector2f mousePosition) {
        if (mIsDragged) {
            mShape.setPosition(mousePosition - mOffset);
        }
    }

    void setColor(sf::Color c) {
        mShape.setFillColor(c);
    }

    void setSize(sf::Vector2f sz) {
        mShape.setSize(sz);
    }

    void setPosition(sf::Vector2f p) {
        mShape.setPosition(p);
    }

    sf::Vector2f getPosition() const {
        return mShape.getPosition();
    }

    bool isDragged() const { return mIsDragged; }
    void draw(sf::RenderWindow& rw) const { rw.draw(mShape); }
};



class BaseWindow : public Draggable {
private:
    sf::RectangleShape mBaseRect {};
    sf::Vector2f mOffset;

public:
    BaseWindow(const sf::RectangleShape& baseRect, const sf::RectangleShape& dragRect)
        : Draggable(dragRect), mBaseRect(baseRect) {
        mOffset = getPosition() - mBaseRect.getPosition();
    }

    bool onMousePressed(sf::Vector2f mousePosition) override {
        return Draggable::onMousePressed(mousePosition);
    }

    void onMouseReleased() override {
        Draggable::onMouseReleased();
    }

    void onMouseMoved(sf::Vector2f mousePosition) override {
        Draggable::onMouseMoved(mousePosition);
        if (isDragged()) {
            mBaseRect.setPosition(getPosition() - mOffset);
        }
    }

    void setBaseColor(sf::Color c) {
        mBaseRect.setFillColor(c);
    }

    void setBaseSize(sf::Vector2f sz) {
        mBaseRect.setSize(sz);
    }

    void setPosition(sf::Vector2f p) override {
        mBaseRect.setPosition(p);
        Draggable::setPosition(mBaseRect.getPosition() + mOffset);
    }

    void draw(sf::RenderWindow& rw) const override {
        rw.draw(mBaseRect);
        Draggable::draw(rw);
    }
};


class QuestionWindow : public BaseWindow {
private:
    sf::Text mText;
    Button mOkButton;
    Button mCancelButton;

public:
    QuestionWindow(const sf::RectangleShape& baseRect, const Draggable& dragRect, sf::RenderWindow& window, sf::Font& font, const sf::String& textData)
        : BaseWindow(baseRect, dragRect),
          mText(textData, font),
          mOkButton(window, {baseRect.getPosition().x + 50, baseRect.getPosition().y + baseRect.getSize().y - 40, 100, 30}, font, 20, "Ok"),
          mCancelButton(window, {baseRect.getPosition().x + 200, baseRect.getPosition().y + baseRect.getSize().y - 40, 100, 30}, font, 20, "Cancel") {
        mText.setPosition(baseRect.getPosition().x + 10, baseRect.getPosition().y + 10);
        mText.setFillColor(sf::Color::Black);
    }

    void draw(sf::RenderWindow& rw) const override {
        BaseWindow::draw(rw);
        rw.draw(mText);
        mOkButton.draw();
        mCancelButton.draw();
    }

    bool handleEvent(const sf::Event& event) {
        if (mOkButton.handleEvent(event)) {
            std::cout << "Ok" << std::endl;
            return true;
        }
        if (mCancelButton.handleEvent(event)) {
            std::cout << "Cancel" << std::endl;
            return true;
        }
        return false;
    }
};


class MessageWindow : public BaseWindow {
public:
    enum class Type {
        Error,
        Done
    };

private:
    sf::Text mText;
    Type mType;

public:
    MessageWindow(const sf::RectangleShape& baseRect, const Draggable& dragRect, Type type, const sf::String& textData, sf::Font& font)
        : BaseWindow(baseRect, dragRect), mType(type), mText(textData, font) {
        mText.setPosition(baseRect.getPosition().x + 10, baseRect.getPosition().y + 10);
        mText.setFillColor(sf::Color::Black);
        if (type == Type::Error) {
            setBaseColor(sf::Color(255, 100, 100)); // Оттенок красного
        } else if (type == Type::Done) {
            setBaseColor(sf::Color(100, 255, 100)); // Оттенок зелёного
        }
    }

    void draw(sf::RenderWindow& rw) const override {
        BaseWindow::draw(rw);
        rw.draw(mText);
    }
};



int main(void) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Windows");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("path/to/font.ttf")) {
        return -1;
    }

    sf::RectangleShape rs;
    rs.setPosition({100, 100});
    rs.setSize({300, 300});
    rs.setFillColor({100, 50, 90});

    Draggable d;
    d.setPosition({120, 250});
    d.setSize({200, 20});
    d.setColor({200, 200, 150});

    BaseWindow b1(rs, d);

    rs.setPosition({300, 300});
    rs.setSize({300, 200});
    rs.setFillColor({240, 240, 240});

    d.setPosition({300, 300});
    d.setSize({300, 30});
    d.setColor({150, 150, 150});

    BaseWindow b2(rs, d);

    QuestionWindow qw(rs, d, window, font, "Are you sure?");
    MessageWindow mw(rs, d, MessageWindow::Type::Error, "Error occurred!", font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                b1.onMousePressed(mousePosition);
                b2.onMousePressed(mousePosition);
                qw.onMousePressed(mousePosition);
                mw.onMousePressed(mousePosition);
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                b1.onMouseReleased();
                b2.onMouseReleased();
                qw.onMouseReleased();
                mw.onMouseReleased();
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
                b1.onMouseMoved(mousePosition);
                b2.onMouseMoved(mousePosition);
                qw.onMouseMoved(mousePosition);
                mw.onMouseMoved(mousePosition);
            }

            qw.handleEvent(event);
        }

        window.clear(sf::Color::Black);
        b1.draw(window);
        b2.draw(window);
        qw.draw(window);
        mw.draw(window);
        window.display();
    }
}
