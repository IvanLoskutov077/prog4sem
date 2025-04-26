#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <set>

class Observer
{
public:
    virtual void updateRotation(float value) = 0;
    virtual void updateColor(float value) = 0;
    virtual ~Observer() = default;
};

class Subject
{
public:
    virtual void addObserver(Observer* p) = 0;
    virtual void removeObserver(Observer* p) = 0;
    virtual void notifyObservers() = 0;
    virtual ~Subject() = default;
};

class Drawable
{
public:
    virtual void draw() const = 0;
    virtual ~Drawable() = default;
};

class Slider : public Drawable
{
protected:
    sf::RectangleShape mTrackShape {};
    sf::RectangleShape mThumbShape {};

    sf::Color mTrackColor           {200, 200, 220};
    sf::Color mThumbColor           {150, 150, 240};

    sf::RenderWindow& mRenderWindow;
    bool mIsPressed {false};

    std::set<Observer*> mObservers;

public:
    Slider(sf::RenderWindow& window, sf::Vector2f centerPosition, sf::Vector2f trackSize, sf::Vector2f thumbSize)
        : mRenderWindow(window)
    {
        mTrackShape.setSize(trackSize);
        mTrackShape.setOrigin(trackSize / 2.0f);
        mTrackShape.setPosition(centerPosition);
        mTrackShape.setFillColor(mTrackColor);

        mThumbShape.setSize(thumbSize);
        mThumbShape.setOrigin(thumbSize / 2.0f);
        mThumbShape.setPosition(centerPosition);
        mThumbShape.setFillColor(mThumbColor);
    }

    void draw() const
    {
        mRenderWindow.draw(mTrackShape);
        mRenderWindow.draw(mThumbShape);
    }

    void onMouseMove(const sf::Event& event)
    {
        if (!mIsPressed)
            return;

        sf::Vector2f mousePosition = mRenderWindow.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
        float min = mTrackShape.getPosition().x - mTrackShape.getSize().x / 2.0f;
        float max = mTrackShape.getPosition().x + mTrackShape.getSize().x / 2.0f;

        mThumbShape.setPosition({std::clamp(mousePosition.x, min, max), mThumbShape.getPosition().y});
        notifyObservers();
    }

    void onMousePressed(const sf::Event& event)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePosition = mRenderWindow.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            if (mThumbShape.getGlobalBounds().contains(mousePosition) || mTrackShape.getGlobalBounds().contains(mousePosition))
            {
                mIsPressed = true;
                mThumbShape.setPosition({mousePosition.x, mThumbShape.getPosition().y});
                notifyObservers();
            }
        }
    }

    void onMouseReleased(const sf::Event& event)
    {
        mIsPressed = false;
    }

    void handleEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::MouseMoved)
            onMouseMove(event);
        else if (event.type == sf::Event::MouseButtonPressed)
            onMousePressed(event);
        else if (event.type == sf::Event::MouseButtonReleased)
            onMouseReleased(event);
    }

    float getValue() const
    {
        float part = mThumbShape.getPosition().x - mTrackShape.getPosition().x + mTrackShape.getSize().x / 2.0f;
        return part / mTrackShape.getSize().x * 100.0f;
    }

    void addObserver(Observer* p)
    {
        mObservers.insert(p);
    }

    void removeObserver(Observer* p)
    {
        mObservers.erase(p);
    }

    virtual void notifyObservers() = 0;
};

class RotationSlider : public Slider {
public:
    using Slider::Slider;

    void notifyObservers() override {
        for (auto p : mObservers)
            p -> updateRotation(getValue());
    }
};

class ColorSlider : public Slider {
public:
    using Slider::Slider;

    void notifyObservers() override {
        for (auto p : mObservers)
            p -> updateColor(getValue());
    }
};

class Circle : public Observer, public Drawable
{
protected:
    sf::CircleShape mShape;
    sf::RenderWindow& mRenderWindow;


public:
    Circle(sf::RenderWindow& window, sf::Vector2f position, float radius)
        : mRenderWindow(window), mShape(radius)
    {
        mShape.setOrigin({radius, radius});
        mShape.setPosition(position);
        mShape.setFillColor(sf::Color::Green);
    }

    void draw() const
    {
        mRenderWindow.draw(mShape);
    }

    void updateRotation(float value) override
    {
        mShape.setRotation(3 * value);
    }

    void updateColor(float value) override
    {
        mShape.setFillColor({0, static_cast<sf::Uint8>(value * 2.55), 0});
    }
};

class Square : public Observer, public Drawable
{
protected:
    sf::RectangleShape mShape;
    sf::RenderWindow& mRenderWindow;

public:
    Square(sf::RenderWindow& window, sf::Vector2f position, float size)
        : mRenderWindow(window), mShape({size, size})
    {
        mShape.setOrigin({size / 2, size / 2});
        mShape.setPosition(position);
        mShape.setFillColor(sf::Color::Red);
    }

    void draw() const
    {
        mRenderWindow.draw(mShape);
    }

    void updateRotation(float value) override
    {
        mShape.setRotation(3 * value);
    }

    void updateColor(float value) override
    {
        mShape.setFillColor({static_cast<sf::Uint8>(value * 2.55), 0, 0});
    }
};

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 800), "Multiple Observers", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    RotationSlider rotationSlider(window, {400, 500}, {500, 20}, {25, 90});
    ColorSlider colorSlider(window, {400, 600}, {500, 20}, {25, 90});

    std::vector<Observer*> observers
    {
        new Circle{window, {100, 300}, 50},
        new Square{window, {400, 300}, 100},
    };

    for (auto p : observers)
    {
        rotationSlider.addObserver(p);
        colorSlider.addObserver(p);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            rotationSlider.handleEvent(event);
            colorSlider.handleEvent(event);
        }

        window.clear(sf::Color::Black);
        rotationSlider.draw();
        colorSlider.draw();
        for (auto p : observers)
            (dynamic_cast<Drawable*>(p))->draw();
        window.display();
    }

    for (auto p : observers)
        delete p;
}