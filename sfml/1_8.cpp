#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <cmath>

float getRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> d(min, max);
    return d(gen);
}

struct Particle {
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

bool checkCollision(const Particle& p1, const Particle& p2) {
    float dx = p1.shape.getPosition().x - p2.shape.getPosition().x;
    float dy = p1.shape.getPosition().y - p2.shape.getPosition().y;
    float distance = std::sqrt(dx * dx + dy * dy);
    float radiusSum = p1.shape.getRadius() + p2.shape.getRadius();
    return distance < radiusSum;
}

void handleCollision(Particle& p1, Particle& p2) {
    p1.velocity = -p1.velocity;
    p2.velocity = -p2.velocity;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Brownian Simulation");

    const int numParticles = 100;
    std::vector<Particle> particles;
    for (int i = 0; i < numParticles; ++i) {
        Particle p;
        p.shape.setRadius(5);
        p.shape.setFillColor(sf::Color::Green);
        p.shape.setPosition(getRandomFloat(0, 800), getRandomFloat(0, 600));
        p.velocity = sf::Vector2f(getRandomFloat(-1, 1), getRandomFloat(-1, 1));
        particles.push_back(p);
    }

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (auto& p : particles) {
            p.velocity.x += getRandomFloat(-0.1f, 0.1f);
            p.velocity.y += getRandomFloat(-0.1f, 0.1f);
            float speed = std::sqrt(p.velocity.x * p.velocity.x + p.velocity.y * p.velocity.y);
            if (speed > 2.0f) {
                p.velocity.x /= speed;
                p.velocity.y /= speed;
            }

            p.shape.move(p.velocity);
            sf::Vector2f pos = p.shape.getPosition();
            if (pos.x < 0 || pos.x > 800) p.velocity.x = -p.velocity.x;
            if (pos.y < 0 || pos.y > 600) p.velocity.y = -p.velocity.y;
        }
        
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                if (checkCollision(particles[i], particles[j])) {
                    handleCollision(particles[i], particles[j]);
                }
            }
        }

        window.clear();
        for (const auto& p : particles) {
            window.draw(p.shape);
        }
        window.display();
    }

    return 0;
}