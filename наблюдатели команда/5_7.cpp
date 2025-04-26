#include <memory>
#include <random>
#include <cstdlib>
#include "SFML/Graphics.hpp"
#include "button.hpp"

float getRandomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> d(min, max);
    return d(gen);
}

float getRandomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> d(min, max);
    return d(gen);
}

sf::Color getRandomColor()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<sf::Uint8> d;
    return sf::Color(d(gen), d(gen), d(gen));
}

class ShapePool
{
protected:
    sf::RectangleShape mField;
    std::vector<std::unique_ptr<sf::Shape>> mShapes;
    sf::RenderWindow& mRenderWindow;

public:
    ShapePool(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size)
        : mRenderWindow(window)
    {
        mField.setPosition(position);
        mField.setSize(size);
        mField.setFillColor(sf::Color(0, 100, 200, 40));
    }

    void addShape(std::unique_ptr<sf::Shape> p)
    {
        p->setOrigin(p->getGlobalBounds().getSize() / 2.0f);
        sf::Vector2f min = p->getGlobalBounds().getSize() / 2.0f;
        sf::Vector2f max = mField.getSize() - p->getGlobalBounds().getSize() / 2.0f;
        p->setPosition(mField.getPosition() + sf::Vector2f(getRandomFloat(min.x, max.x), getRandomFloat(min.x, max.y)));
        p->setFillColor(getRandomColor());
        mShapes.push_back(std::move(p));
    }

    size_t getNumbeorOfShapes() const { return mShapes.size(); }
    sf::Vector2f getPosition() const { return mField.getPosition(); }
    sf::Vector2f getSize() const { return mField.getSize(); }

    sf::Shape& getShape(int index)
    {
        return *mShapes.at(index);
    }

    void draw() const
    {
        for (const auto& p : mShapes)
            mRenderWindow.draw(*p);
        mRenderWindow.draw(mField);
    }
};

class Command
{
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

class NoCommand : public Command
{
public:
    void execute() override {};
};

class RotateCommand : public Command
{
protected:
    ShapePool& mPool;
    size_t mIndex;
    float mAngle;

public:
    RotateCommand(ShapePool& pool, size_t index, float angle)
        : mPool(pool), mIndex(index), mAngle(angle) {}

    void execute() override
    {
        mPool.getShape(mIndex).rotate(mAngle);
    }
};

class RandomColorCommand : public Command
{
protected:
    ShapePool& mPool;
    size_t mIndex;

public:
    RandomColorCommand(ShapePool& pool, size_t index)
        : mPool(pool), mIndex(index) {}

    void execute() override
    {
        mPool.getShape(mIndex).setFillColor(getRandomColor());
    }
};

class RandomAllPositionsCommand : public Command
{
protected:
    ShapePool& mPool;

public:
    RandomAllPositionsCommand(ShapePool& pool)
        : mPool(pool) {}

    void execute() override
    {
        for (size_t i = 0; i < mPool.getNumbeorOfShapes(); ++i)
        {
            sf::Shape& shape = mPool.getShape(i);
            sf::Vector2f min = shape.getGlobalBounds().getSize() / 2.0f;
            sf::Vector2f max = mPool.getSize() - shape.getGlobalBounds().getSize() / 2.0f;
            shape.setPosition(mPool.getPosition() + sf::Vector2f(getRandomFloat(min.x, max.x), getRandomFloat(min.x, max.y)));
        }
    }
};

class AddNewRandomShape : public Command
{
protected:
    ShapePool& mPool;
    float mMinSize {};
    float mMaxSize {};

public:
    AddNewRandomShape(ShapePool& pool, float minSize, float maxSize)
        : mPool(pool), mMinSize(minSize), mMaxSize(maxSize) {}

    void execute() override
    {
        int r = getRandomInt(0, 2);
        std::unique_ptr<sf::Shape> ps;
        if (r == 0)
            ps = std::make_unique<sf::RectangleShape>(sf::Vector2f{ getRandomFloat(mMinSize, mMaxSize), getRandomFloat(mMinSize, mMaxSize) });
        else if (r == 1)
            ps = std::make_unique<sf::CircleShape>(getRandomFloat(mMinSize / 2, mMaxSize / 2));
        else
            ps = std::make_unique<sf::CircleShape>(getRandomFloat(mMinSize / 2, mMaxSize / 2), 3);

        ps->setFillColor(getRandomColor());
        ps->rotate(getRandomFloat(0, 360));
        sf::FloatRect bounds = ps->getGlobalBounds();
        ps->setPosition({ mPool.getPosition().x + getRandomFloat(0, mPool.getSize().x - bounds.width),
                          mPool.getPosition().y + getRandomFloat(0, mPool.getSize().y - bounds.height) });
        mPool.addShape(std::move(ps));
    }
};

class ControlPanel
{
protected:
    std::vector<std::unique_ptr<Button>> mButtons;
    std::vector<std::unique_ptr<Command>> mCommands;
    sf::RenderWindow& mRenderWindow;

public:
    ControlPanel(sf::RenderWindow& window) : mRenderWindow(window) {}

    void addButton(std::unique_ptr<Button> p)
    {
        mButtons.push_back(std::move(p));
        mCommands.push_back(std::make_unique<NoCommand>());
    }

    void addButton(std::unique_ptr<Button> pb, std::unique_ptr<Command> pc)
    {
        mButtons.push_back(std::move(pb));
        mCommands.push_back(std::move(pc));
    }

    size_t getNumberOfButtons() const { return mButtons.size(); }

    void draw()
    {
        for (const auto& p : mButtons)
            p->draw();
    }

    void handleEvent(sf::Event event)
    {
        for (std::size_t i = 0; i < mButtons.size(); ++i)
        {
            if (mButtons.at(i)->handleEvent(event))
                mCommands.at(i)->execute();
        }
    }
};

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Shapes and Command", sf::Style::Default, settings);

    sf::Font font;
    if (!font.loadFromFile("sourceCodePro.ttf"))
    {
        std::cerr << "Error. Can't load font" << std::endl;
        std::exit(1);
    }

    ShapePool pool(window, {300, 50}, {450, 700});
    pool.addShape(std::make_unique<sf::RectangleShape>(sf::Vector2f(getRandomFloat(50, 150), getRandomFloat(50, 150))));
    pool.addShape(std::make_unique<sf::RectangleShape>(sf::Vector2f(getRandomFloat(50, 150), getRandomFloat(50, 150))));
    pool.addShape(std::make_unique<sf::CircleShape>(getRandomFloat(30, 100), 3));
    pool.addShape(std::make_unique<sf::CircleShape>(getRandomFloat(30, 100)));
    pool.addShape(std::make_unique<sf::CircleShape>(getRandomFloat(30, 100)));

    ControlPanel control(window);
    control.addButton(std::make_unique<Button>(window, sf::FloatRect{40, 80, 240, 40}, font, "Rotate First"), std::make_unique<RotateCommand>(pool, 0, 30));
    control.addButton(std::make_unique<Button>(window, sf::FloatRect{40, 140, 240, 40}, font, "Rotate Second"), std::make_unique<RotateCommand>(pool, 1, 45));
    control.addButton(std::make_unique<Button>(window, sf::FloatRect{40, 200, 240, 40}, font, "One Random Color"), std::make_unique<RandomColorCommand>(pool, 3));
    control.addButton(std::make_unique<Button>(window, sf::FloatRect{40, 260, 240, 40}, font, "All Random Positions"), std::make_unique<RandomAllPositionsCommand>(pool));
    control.addButton(std::make_unique<Button>(window, sf::FloatRect{40, 320, 240, 40}, font, "New Random Shape"), std::make_unique<AddNewRandomShape>(pool, 10, 150));
    control.addButton(std::make_unique<Button>(window, sf::FloatRect{40, 380, 240, 40}, font, "no"));
    control.addButton(std::make_unique<Button>(window, sf::FloatRect{40, 440, 240, 40}, font, "no"));
    control.addButton(std::make_unique<Button>(window, sf::FloatRect{40, 500, 240, 40}, font, "no"));
    control.addButton(std::make_unique<Button>(window, sf::FloatRect{40, 700, 240, 40}, font, "Undo"));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            control.handleEvent(event);
        }

        window.clear(sf::Color::Black);
        pool.draw();
        control.draw();
        window.display();
    }
}

