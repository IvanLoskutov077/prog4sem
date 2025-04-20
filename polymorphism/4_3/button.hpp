#pragma once
#include "widget.hpp"
#include <iostream>

class Button : public Widget
{
private:
    sf::Color mDefaultColor {179, 230, 255};
    sf::Color mHoverColor   {128, 212, 255};
    sf::Color mPressedColor {  0, 136, 204};
    sf::RectangleShape mShape {};
    sf::Text mText {};
    bool mIsPressed {false};
    sf::RenderWindow& mRenderWindow;

public:
    Button(sf::RenderWindow& window, sf::FloatRect rect, sf::Font& font, const sf::String& textData)
            : mRenderWindow{window}
    {
        mShape.setPosition({rect.left, rect.top});
        mShape.setSize({rect.width, rect.height});
        mShape.setFillColor(mDefaultColor);

        mText.setFont(font);
        mText.setString(textData);
        mText.setFillColor(sf::Color::Black);

        mText.setCharacterSize(30);
        auto calculateOptimalFontSize = [this]()
        {
            float widthFraction = 0.6;
            float heightFraction = 0.4;

            sf::FloatRect textBounds = mText.getLocalBounds();
            float widthRatio = mShape.getSize().x / textBounds.width * widthFraction;
            float heightRatio = mShape.getSize().y / textBounds.height * heightFraction;
            float minRatio = std::min(widthRatio, heightRatio);

            unsigned int newSize = static_cast<unsigned int>(mText.getCharacterSize() * minRatio);
            return newSize;
        };
        mText.setCharacterSize(calculateOptimalFontSize());
        centerText();
    }

    void setFontSize(unsigned int fontSize)
    {
        mText.setCharacterSize(fontSize);
        centerText();
    }

    void draw() const override
    {
        mRenderWindow.draw(mShape);
        mRenderWindow.draw(mText);
    }

    bool handleEvent(const sf::Event& event) override
    {
        if (event.type == sf::Event::MouseMoved)
            onMouseMove(event);
        else if (event.type == sf::Event::MouseButtonPressed)
            onMousePressed(event);
        else if (event.type == sf::Event::MouseButtonReleased)
            return onMouseReleased(event);
        return false;
    }

private:
    void centerText()
    {
        sf::FloatRect textBounds = mText.getLocalBounds();
        mText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        sf::Vector2f rectCenter = mShape.getPosition() + mShape.getSize() / 2.0f;
        mText.setPosition(rectCenter);
    }

    void onMousePressed(const sf::Event& event)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePosition = mRenderWindow.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            if (mShape.getGlobalBounds().contains(mousePosition))
            {
                mIsPressed = true;
                mShape.setFillColor(mPressedColor);
            }
        }
    }

    void onMouseMove(const sf::Event& event)
    {
        if (mIsPressed)
            return;

        sf::Vector2f mousePosition = mRenderWindow.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
        if (mShape.getGlobalBounds().contains(mousePosition))
            mShape.setFillColor(mHoverColor);
        else
            mShape.setFillColor(mDefaultColor);
    }

    bool onMouseReleased(const sf::Event& event)
    {
        if (!mIsPressed)
            return false;

        mIsPressed = false;

        sf::Vector2f mousePosition = mRenderWindow.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        if (mShape.getGlobalBounds().contains(mousePosition))
        {
            mShape.setFillColor(mHoverColor);
            return true;
        }
        else
        {
            mShape.setFillColor(mDefaultColor);
            return false;
        }
    }
};
