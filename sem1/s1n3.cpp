#include <iostream>
#include <cmath>
#include <list>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include "ContextMenu.hpp"

float distance(sf::Vector2f start, sf::Vector2f finish) {
    return std::sqrt((start.x - finish.x)*(start.x - finish.x) + (start.y - finish.y)*(start.y - finish.y));
}

void drawLine(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f finish, sf::Color color = sf::Color::White) {
    sf::Vertex line_vertices[2] = {sf::Vertex(start, color), sf::Vertex(finish, color)};
    window.draw(line_vertices, 2, sf::Lines);
}

struct Ball {
    sf::Vector2f position;
    float radius;
    bool isChoosen;
    sf::Color color;

    Ball(sf::Vector2f position, float radius) : position(position), radius(radius), color(sf::Color::White) {
        isChoosen = false;
    }

    void draw(sf::RenderWindow& window) const {
        sf::CircleShape circle(radius);
        circle.setFillColor(color);
        circle.setOrigin({radius, radius});
        circle.setPosition(position);
        window.draw(circle);

        if (isChoosen) {
            const float fraction = 0.7;
            drawLine(window, {position.x - radius, position.y + radius}, {position.x - radius, position.y + radius*fraction});
            drawLine(window, {position.x - radius, position.y + radius}, {position.x - fraction * radius, position.y + radius});

            drawLine(window, {position.x + radius, position.y + radius}, {position.x + radius, position.y + radius*fraction});
            drawLine(window, {position.x + radius, position.y + radius}, {position.x + radius*fraction, position.y + radius});

            drawLine(window, {position.x + radius, position.y - radius}, {position.x + radius*fraction, position.y - radius});
            drawLine(window, {position.x + radius, position.y - radius}, {position.x + radius, position.y - radius*fraction});

            drawLine(window, {position.x - radius, position.y - radius}, {position.x - radius*fraction, position.y - radius});
            drawLine(window, {position.x - radius, position.y - radius}, {position.x - radius, position.y - radius*fraction});
        }
    }
};

sf::Color randomColor() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return sf::Color(dis(gen), dis(gen), dis(gen));
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Select, Move, Delete!", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    std::list<Ball> balls;
    balls.push_back(Ball({200, 200}, 26));
    balls.push_back(Ball({400, 300}, 20));
    balls.push_back(Ball({500, 100}, 16));
    balls.push_back(Ball({200, 400}, 18));
    balls.push_back(Ball({350, 150}, 22));
    balls.push_back(Ball({750, 400}, 21));

    std::list<Ball> copiedBalls;

    sf::RectangleShape selectionRect;
    selectionRect.setFillColor(sf::Color(150, 150, 240, 50));
    selectionRect.setOutlineColor(sf::Color(200, 200, 255));
    selectionRect.setOutlineThickness(1);

    bool isSelecting = false;
    bool isMoving = false;
    sf::Vector2f lastMousePosition;

    ContextMenu contextMenu(window);
    contextMenu.addButton("Delete", [&balls]() {
        for (auto it = balls.begin(); it != balls.end(); ) {
            if (it->isChoosen)
                it = balls.erase(it);
            else
                ++it;
        }
    });
    contextMenu.addButton("Create", [&balls, &window]() {
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        balls.push_back(Ball(mousePosition, 5 + rand() % 40));
    });
    contextMenu.addButton("Random Color", [&balls]() {
        for (Ball& b : balls) {
            if (b.isChoosen)
                b.color = randomColor();
        }
    });
    contextMenu.addButton("Increase", [&balls]() {
        for (Ball& b : balls) {
            if (b.isChoosen)
                b.radius *= 1.25;
        }
    });
    contextMenu.addButton("Decrease", [&balls]() {
        for (Ball& b : balls) {
            if (b.isChoosen)
                b.radius *= 0.75;
        }
    });
    contextMenu.addButton("Copy", [&balls, &copiedBalls]() {
        copiedBalls.clear();
        for (const Ball& b : balls) {
            if (b.isChoosen)
                copiedBalls.push_back(b);
        }
    });
    contextMenu.addButton("Paste", [&balls, &copiedBalls, &window]() {
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (const Ball& b : copiedBalls) {
            Ball newBall(b);
            newBall.position = mousePosition;
            balls.push_back(newBall);
        }
    });
    contextMenu.addButton("Cut", [&balls, &copiedBalls]() {
        copiedBalls.clear();
        for (auto it = balls.begin(); it != balls.end(); ) {
            if (it -> isChoosen) {
                copiedBalls.push_back(*it);
                it = balls.erase(it);
            } else {
                ++it;
            }
        }
    });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});

                if (isSelecting) {
                    selectionRect.setSize(mousePosition - selectionRect.getPosition());
                    for (Ball& b : balls) {
                        if (selectionRect.getGlobalBounds().contains(b.position))
                            b.isChoosen = true;
                    }
                }

                if (isMoving) {
                    sf::Vector2f delta = mousePosition - lastMousePosition;
                    for (Ball& b : balls) {
                        if (b.isChoosen) b.position += delta;
                    }
                }

                lastMousePosition = mousePosition;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        for (Ball& b : balls) b.isChoosen = false;
                    }

                    for (Ball& b : balls) {
                        if (distance(mousePosition, b.position) < b.radius) {
                            b.isChoosen = true;
                            break;
                        }
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
                        balls.push_back(Ball(mousePosition, 5 + rand() % 40));

                    isSelecting = true;
                    selectionRect.setPosition(mousePosition);
                    selectionRect.setSize({0, 0});
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    contextMenu.show(mousePosition);
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                isSelecting = false;
                isMoving = false;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    for (Ball& b : balls) {
                        if (b.isChoosen) b.color = randomColor();
                    }
                }
                else if (event.key.code == sf::Keyboard::Delete) {
                    for (auto it = balls.begin(); it != balls.end(); ) {
                        if (it->isChoosen) it = balls.erase(it);
                        else ++it;
                    }
                }
                else if (event.key.code == sf::Keyboard::C && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    copiedBalls.clear();
                    for (const Ball& b : balls) {
                        if (b.isChoosen)
                            copiedBalls.push_back(b);
                    }
                }
                else if (event.key.code == sf::Keyboard::V && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    for (const Ball& b : copiedBalls) {
                        Ball newBall(b);
                        newBall.position = mousePosition;
                        balls.push_back(newBall);
                    }
                }
                else if (event.key.code == sf::Keyboard::X && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                    copiedBalls.clear();
                    for (auto it = balls.begin(); it != balls.end(); ) {
                        if (it->isChoosen) {
                            copiedBalls.push_back(*it);
                            it = balls.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }
            }
        }
        window.clear(sf::Color::Black);

        for (Ball& b : balls)
            b.draw(window);

        if (isSelecting)
            window.draw(selectionRect);
        contextMenu.draw();
        window.display();
    }

    return 0;
}
