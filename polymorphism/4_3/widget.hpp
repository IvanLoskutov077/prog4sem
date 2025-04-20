#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Widget
{
public:
    virtual ~Widget() = default;
    virtual void draw() const = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;
};