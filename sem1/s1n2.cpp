#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

const float MIN_DISTANCE = 10.0f;
const float G = 1.0f;

class Ball {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float mass;
    float charge;
    sf::CircleShape shape;

    Ball(float x, float y, float mass, float charge)
        : position(x, y), velocity(0, 0), acceleration(0, 0), mass(mass), charge(charge) {
        shape.setRadius(5);
        shape.setFillColor(charge > 0 ? sf::Color::Red : sf::Color::Blue);
        shape.setPosition(position);
    }

    void update(float dt) {
        velocity += acceleration * dt;
        position += velocity * dt;
        shape.setPosition(position);
        acceleration = sf::Vector2f(0, 0);
    }

    void applyForce(const sf::Vector2f& force) {
        acceleration += force / mass;
    }
};

void applyCoulombForce(Ball& ball1, Ball& ball2) {
    sf::Vector2f delta = ball1.position - ball2.position;
    float distance = std::max(std::sqrt(delta.x * delta.x + delta.y * delta.y), MIN_DISTANCE);
    sf::Vector2f force = G * (ball1.charge * ball2.charge) / (distance * distance) * (delta / distance);
    ball1.applyForce(force);
    ball2.applyForce(-force);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Charged Balls Simulation");
    window.setFramerateLimit(60);

    std::vector<Ball> balls;
    for (int i = 0; i < 20; ++i) {
        float x = static_cast<float>(rand() % 800);
        float y = static_cast<float>(rand() % 600);
        float mass = static_cast<float>(rand() % 100 + 10);
        float charge = static_cast<float>(rand() % 200 - 100);
        balls.emplace_back(x, y, mass, charge);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                float x = static_cast<float>(event.mouseButton.x);
                float y = static_cast<float>(event.mouseButton.y);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    balls.emplace_back(x, y, 10.0f, -50.0f);
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    balls.emplace_back(x, y, 1000.0f, 50.0f);
                }
            }
        }

        for (auto& ball : balls) {
            ball.acceleration = sf::Vector2f(0, 0);
        }

        for (size_t i = 0; i < balls.size(); ++i) {
            for (size_t j = i + 1; j < balls.size(); ++j) {
                applyCoulombForce(balls[i], balls[j]);
            }
        }

        for (auto& ball : balls) {
            ball.update(1.0f / 60.0f);

            if (ball.position.x <= 0 || ball.position.x >= 800) {
                ball.velocity.x = -ball.velocity.x;
            }
            if (ball.position.y <= 0 || ball.position.y >= 600) {
                ball.velocity.y = -ball.velocity.y;
            }
        }

        window.clear();
        for (const auto& ball : balls) {
            window.draw(ball.shape);
        }
        window.display();
    }

    return 0;
}