#include <SFML/Graphics.hpp>
#include <algorithm>

class Slider {
protected:
    sf::RectangleShape mTrackShape {};
    sf::RectangleShape mThumbShape {};
    bool mIsPressed {false};
    sf::RenderWindow& mRenderWindow;

public:
    Slider(sf::RenderWindow& window, sf::Vector2f centerPosition, sf::Vector2f trackSize, sf::Vector2f thumbSize)
        : mRenderWindow(window) {
        mTrackShape.setSize(trackSize);
        mTrackShape.setOrigin(trackSize / 2.0f);
        mTrackShape.setPosition(centerPosition);
        mTrackShape.setFillColor({200, 200, 220});

        mThumbShape.setSize(thumbSize);
        mThumbShape.setOrigin(thumbSize / 2.0f);
        mThumbShape.setPosition(centerPosition);
        mThumbShape.setFillColor({150, 150, 240});
    }

    void draw() {
        mRenderWindow.draw(mTrackShape);
        mRenderWindow.draw(mThumbShape);
    }

    void setRestrictedThumbPosition(sf::Vector2f position) {
        float min = mTrackShape.getPosition().x - mTrackShape.getSize().x / 2.0f;
        float max = mTrackShape.getPosition().x + mTrackShape.getSize().x / 2.0f;
        mThumbShape.setPosition({std::clamp(position.x, min, max), mThumbShape.getPosition().y});
    }

    void onMousePressed(const sf::Event& event) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePosition = mRenderWindow.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            if (mThumbShape.getGlobalBounds().contains(mousePosition) || mTrackShape.getGlobalBounds().contains(mousePosition)) {
                mIsPressed = true;
                setRestrictedThumbPosition({mousePosition.x, mThumbShape.getPosition().y});
            }
        }
    }

    void onMouseMove(const sf::Event& event) {
        if (!mIsPressed) return;
        sf::Vector2f mousePosition = mRenderWindow.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
        setRestrictedThumbPosition(mousePosition);
    }

    void onMouseReleased(const sf::Event& event) {
        mIsPressed = false;
    }

    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseMoved)
            onMouseMove(event);
        else if (event.type == sf::Event::MouseButtonPressed)
            onMousePressed(event);
        else if (event.type == sf::Event::MouseButtonReleased)
            onMouseReleased(event);
    }

    bool isPressed() const {
        return mIsPressed;
    }

    float getValue() const {
        float start = mTrackShape.getPosition().x - mTrackShape.getSize().x / 2.0f;
        float finish = mTrackShape.getPosition().x + mTrackShape.getSize().x / 2.0f;
        float position = mThumbShape.getPosition().x;
        return 100.0f * (position - start) / (finish - start);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Color Circle with Sliders");
    window.setFramerateLimit(60);

    sf::CircleShape circle(100);
    circle.setPosition(300, 200);
    circle.setFillColor(sf::Color::Black);

    Slider redSlider(window, {200, 500}, {300, 20}, {20, 40});
    Slider greenSlider(window, {200, 550}, {300, 20}, {20, 40});
    Slider blueSlider(window, {200, 600}, {300, 20}, {20, 40});

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            redSlider.handleEvent(event);
            greenSlider.handleEvent(event);
            blueSlider.handleEvent(event);
        }

        sf::Uint8 red = static_cast<sf::Uint8>(redSlider.getValue() * 2.55f);
        sf::Uint8 green = static_cast<sf::Uint8>(greenSlider.getValue() * 2.55f);
        sf::Uint8 blue = static_cast<sf::Uint8>(blueSlider.getValue() * 2.55f);
        circle.setFillColor({red, green, blue});

        window.clear(sf::Color::White);
        window.draw(circle);
        redSlider.draw();
        greenSlider.draw();
        blueSlider.draw();
        window.display();
    }

    return 0;
}