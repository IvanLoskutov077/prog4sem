#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <iostream>


class Line
{
private:
    sf::Vertex vertices[4]  {};
    float thickness         {};
    sf::Color color         {};

public:
    Line(sf::Vector2f point1, sf::Vector2f point2, sf::Color color, float thickness):
        color{color}, thickness{thickness}
    {
        if (point1 == point2)
            return;
        
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular {-unitDirection.y, unitDirection.x};

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i = 0; i < 4; ++i)
            vertices[i].color = color;
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(vertices, 4, sf::Quads);
    }
};


class Node
{
public:
    enum class State
    {
        Blocked,
        Unblocked,
        Activated
    };

protected:
    sf::Vector2f mPosition {0, 0};
    State mState {State::Blocked};

    sf::Color sBlockedColor   {40, 40, 40};
    sf::Color sUnlockedColor  {80, 80, 40};
    sf::Color sActivatedColor {160, 160, 40};

    std::vector<std::shared_ptr<Node>> mChildren {};

public:
    Node(sf::Vector2f position) : mPosition{position} {}

    void addChild(std::shared_ptr<Node> child)
    {
        mChildren.push_back(child);
    }

    void unblock()
    {
        mState = State::Unblocked;
    }

    void block()
    {
        mState = State::Blocked;
        for (const auto& child : mChildren)
            child->block();
    }

    void onMousePressed(sf::Vector2f mouseCoords)
    {
        if (mState == State::Blocked)
            return;

        if (collisionTest(mouseCoords)) 
        {
            if (mState == State::Unblocked) 
            {
                mState = State::Activated;
                for (const auto& child : mChildren)
                    child->unblock();
            }

            else if (mState == State::Activated)
            {
                mState = State::Unblocked;
                for (const auto& child : mChildren)
                    child->block();
            }
        }

        for (const auto& child : mChildren)
        {
            child->onMousePressed(mouseCoords);
        }
    }

    sf::Vector2f getPosition() const {return mPosition;}
    void setPosition(sf::Vector2f position) {mPosition = position;}

    virtual bool collisionTest(sf::Vector2f mouseCoords) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};


class HitNode : public Node
{
protected:
    sf::Texture mTexture {};
    sf::Sprite mSprite   {}; 
    float mRadius        {24};
    bool mIsActivated    {false};

public:
    HitNode(sf::Vector2f position) : Node{position} {}

    virtual sf::String getIconPath() = 0;

    void loadTexture()
    {
        sf::String texturePath = getIconPath();
        if (!mTexture.loadFromFile(texturePath))
        {
            std::cout << "Error! Can't load file " << texturePath.toAnsiString() << std::endl;
            std::exit(1);
        }
        mSprite.setTexture(mTexture);
        mSprite.setOrigin({mRadius, mRadius});
        mSprite.setPosition(mPosition);
    }

    sf::Color getCurrentColor() const
    {
        if (mState == State::Unblocked)
            return sUnlockedColor;
        else if (mState == State::Activated)
            return sActivatedColor;
        return sBlockedColor;
    }

    bool collisionTest(sf::Vector2f mouseCoords) override
    {
        sf::Vector2f d = mPosition - mouseCoords;
        return d.x * d.x + d.y * d.y < mRadius * mRadius;
    }

    void draw(sf::RenderWindow& window) const override
    {
        for (const auto& el : mChildren)
        {
            Line connectionLine {mPosition, el->getPosition(), getCurrentColor(), 2};
            connectionLine.draw(window);
            el->draw(window);
        }

        static sf::CircleShape shape {mRadius};
        shape.setOrigin({mRadius, mRadius});
        shape.setFillColor(getCurrentColor());
        shape.setPosition(mPosition);
        window.draw(shape);

        window.draw(mSprite);
    }
};



class BombSkillNode : public HitNode
{
public:
    BombSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_bomb.png"};
    }
};

class SpikesSkillNode : public HitNode
{
public:
    SpikesSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_spikes.png"};
    }
};


class LightningSkillNode : public HitNode
{
public:
    LightningSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_lightning.png"};
    }
};


class EyeSkillNode : public HitNode
{
public:
    EyeSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_eye.png"};
    }
};


class ClawsSkillNode : public HitNode
{
public:
    ClawsSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_claws.png"};
    }
};

class ShieldSkillNode : public HitNode
{
public:
    ShieldSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_shield.png"};
    }
};


class SwordSkillNode : public HitNode
{
public:
    SwordSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_sword.png"};
    }
};


class ShurikenSkillNode : public HitNode
{
public:
    ShurikenSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_shuriken.png"};
    }
};

class WindSkillNode : public HitNode
{
public:
    WindSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_shuriken.png"};
    }
};


class MeteoriteSkillNode : public HitNode
{
public:
    MeteoriteSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_meteorite.png"};
    }
};

class HandSkillNode : public HitNode
{
public:
    HandSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_hand.png"};
    }
};

class EarthquakeSkillNode : public HitNode
{
public:
    EarthquakeSkillNode(sf::Vector2f position) : HitNode{position} 
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_earthquake.png"};
    }
};


class FireballSkillNode : public HitNode
{
public:
    FireballSkillNode(sf::Vector2f position) : HitNode{position}
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_fireball.png"};
    }
};


class AccumulativeNode : public Node
{
protected:
    sf::Texture mTexture {};
    sf::Sprite mSprite   {};
    sf::RectangleShape mRectangle {};
    sf::Text mText {};
    sf::Font mFont {};
    int mCurrentLevel {0};
    int mMaxLevel {5};
    float mSize {48};

public:
    AccumulativeNode(sf::Vector2f position) : Node{position}
    {
        mRectangle.setSize({mSize, mSize});
        mRectangle.setOrigin({mSize / 2, mSize / 2});
        mRectangle.setPosition(mPosition);

        if (!mFont.loadFromFile("arial.ttf"))
        {
            std::cout << "Error! Can't load font file" << std::endl;
            std::exit(1);
        }

        mText.setFont(mFont);
        mText.setCharacterSize(12);
        mText.setFillColor(sf::Color::White);
        updateText();
    }

    virtual sf::String getIconPath() = 0;

    void loadTexture()
    {
        sf::String texturePath = getIconPath();
        if (!mTexture.loadFromFile(texturePath))
        {
            std::cout << "Error! Can't load file " << texturePath.toAnsiString() << std::endl;
            std::exit(1);
        }
        mSprite.setTexture(mTexture);
        mSprite.setOrigin({mSize / 2, mSize / 2});
        mSprite.setPosition(mPosition);
    }

    sf::Color getCurrentColor() const
    {
        if (mState == State::Unblocked)
            return sUnlockedColor;
        else if (mState == State::Activated)
            return sActivatedColor;
        return sBlockedColor;
    }

    bool collisionTest(sf::Vector2f mouseCoords) override
    {
        sf::Vector2f d = mPosition - mouseCoords;
        return std::abs(d.x) < mSize / 2 && std::abs(d.y) < mSize / 2;
    }

    void updateText()
    {
        mText.setString(std::to_string(mCurrentLevel) + " / " + std::to_string(mMaxLevel));
        mText.setPosition(mPosition.x - mSize / 2, mPosition.y + mSize / 2 + 5);
    }

    void onMousePressed(sf::Vector2f mouseCoords) override
    {
        if (mState == State::Blocked)
            return;

        if (collisionTest(mouseCoords))
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (mState == State::Unblocked)
                {
                    mState = State::Activated;
                    for (const auto& child : mChildren)
                        child->unblock();
                    mCurrentLevel = 1;
                }
                else if (mState == State::Activated && mCurrentLevel < mMaxLevel)
                {
                    mCurrentLevel++;
                }
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mState == State::Activated)
            {
                mCurrentLevel--;
                if (mCurrentLevel == 0)
                {
                    mState = State::Unblocked;
                    for (const auto& child : mChildren)
                        child->block();
                }
            }
            updateText();
        }

        for (const auto& child : mChildren)
        {
            child->onMousePressed(mouseCoords);
        }
    }

    void draw(sf::RenderWindow& window) const override
    {
        for (const auto& el : mChildren)
        {
            Line connectionLine {mPosition, el->getPosition(), getCurrentColor(), 2};
            connectionLine.draw(window);
            el->draw(window);
        }

        mRectangle.setFillColor(getCurrentColor());
        window.draw(mRectangle);
        window.draw(mSprite);
        window.draw(mText);
    }
};


class SwordAccumulativeNode : public AccumulativeNode
{
public:
    SwordAccumulativeNode(sf::Vector2f position) : AccumulativeNode{position}
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_rect_sword.png"};
    }
};

class ChainAccumulativeNode : public AccumulativeNode
{
public:
    ChainAccumulativeNode(sf::Vector2f position) : AccumulativeNode{position}
    {
        loadTexture();
    }

    sf::String getIconPath() override
    {
        return sf::String{"icons/icon_rect_chain.png"};
    }
};



std::shared_ptr<Node> createSkillTree()
{
    std::shared_ptr<Node> root = std::make_shared<LightningSkillNode>(sf::Vector2f{400, 500});
    std::shared_ptr<Node> a    = std::make_shared<ShurikenSkillNode>(sf::Vector2f{200, 400});
    std::shared_ptr<Node> b    = std::make_shared<BombSkillNode>(sf::Vector2f{400, 400});
    std::shared_ptr<Node> c    = std::make_shared<EyeSkillNode>(sf::Vector2f{600, 400});
    root->addChild(a);
    root->addChild(b);
    root->addChild(c);

    a->addChild(std::make_shared<ShieldSkillNode>(sf::Vector2f{100, 200}));
    a->addChild(std::make_shared<SwordSkillNode>(sf::Vector2f{200, 200}));
    a->addChild(std::make_shared<EarthquakeSkillNode>(sf::Vector2f{300, 200}));

    b->addChild(std::make_shared<HandSkillNode>(sf::Vector2f{400, 200}));
    b->addChild(std::make_shared<MeteoriteSkillNode>(sf::Vector2f{500, 200}));
    std::shared_ptr<Node> e = std::make_shared<BombSkillNode>(sf::Vector2f{600, 200});
    b->addChild(e);
    e->addChild(std::make_shared<WindSkillNode>(sf::Vector2f{500, 100}));
    e->addChild(std::make_shared<SwordSkillNode>(sf::Vector2f{600, 100}));

    c->addChild(std::make_shared<EyeSkillNode>(sf::Vector2f{700, 200}));
    c->addChild(std::make_shared<FireballSkillNode>(sf::Vector2f{800, 200}));
    c->addChild(std::make_shared<SwordAccumulativeNode>(sf::Vector2f{900, 200}));
    c->addChild(std::make_shared<ChainAccumulativeNode>(sf::Vector2f{1000, 200}));
    return root;
}


class SkillTree
{
protected:
    std::shared_ptr<Node> mRoot;
    int mFreePoints {5};

public:
    SkillTree(std::shared_ptr<Node> root) : mRoot{root} {}

    virtual void draw(sf::RenderWindow& window) const
    {
        mRoot->draw(window);
        drawFreePoints(window);
    }

    virtual void onMousePressed(sf::Vector2f mouseCoords)
    {
        mRoot->onMousePressed(mouseCoords);
    }

    void drawFreePoints(sf::RenderWindow& window) const
    {
        sf::Font font;
        if (!font.loadFromFile("arial.ttf"))
        {
            std::cout << "Error! Can't load font file" << std::endl;
            std::exit(1);
        }

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setString("Free Points: " + std::to_string(mFreePoints));
        text.setPosition(10, 10);
        window.draw(text);
    }

    void activateNode(std::shared_ptr<Node> node)
    {
        if (mFreePoints > 0)
        {
            node->unblock();
            mFreePoints--;
        }
    }

    void deactivateNode(std::shared_ptr<Node> node)
    {
        node->block();
        mFreePoints++;
    }
};

class MageSkillTree : public SkillTree
{
public:
    MageSkillTree() : SkillTree{createSkillTree()} {}
};


int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Skill Tree", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    MageSkillTree skillTree;
    skillTree.activateNode(skillTree.getRoot());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mouseCoords = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                skillTree.onMousePressed(mouseCoords);
            }
        }

        window.clear(sf::Color::Black);
        skillTree.draw(window);
        window.display();
    }
}
