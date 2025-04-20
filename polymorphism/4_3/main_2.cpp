#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "button.hpp"
#include "slider.hpp"
#include "draggable.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Widgets");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("sourceCodePro.ttf"))
    {
        std::cout << "Error: Font file not found." << std::endl;
        std::exit(1);
    }

    std::vector<std::unique_ptr<Widget>> widgets;
    widgets.push_back(std::make_unique<Button>(window, sf::FloatRect{350, 300, 200, 80}, font, "Cat"));
    widgets.push_back(std::make_unique<Button>(window, sf::FloatRect{200, 450, 100, 80}, font, "Dog"));
    widgets.push_back(std::make_unique<Button>(window, sf::FloatRect{600, 600, 150, 70}, font, "Mouse"));
    widgets.push_back(std::make_unique<Slider>(window, sf::Vector2f{500, 500}, sf::Vector2f{200, 10}, sf::Vector2f{20, 40}));
    widgets.push_back(std::make_unique<Slider>(window, sf::Vector2f{300, 100}, sf::Vector2f{250, 20}, sf::Vector2f{30, 60}));
    widgets.push_back(std::make_unique<Draggable>(window, sf::Vector2f{550, 100}, sf::Vector2f{200, 120}, sf::Color{20, 120, 50}));
    widgets.push_back(std::make_unique<Draggable>(window, sf::Vector2f{50, 550}, sf::Vector2f{200, 200}, sf::Color{170, 20, 50}));
    widgets.push_back(std::make_unique<Draggable>(window, sf::Vector2f{50, 200}, sf::Vector2f{100, 100}, sf::Color{100, 20, 150}));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            for (auto& widget : widgets)
                widget->handleEvent(event);
        }

        window.clear(sf::Color::Black);
        for (auto& widget : widgets)
            widget->draw();

        window.display();
    }
}
