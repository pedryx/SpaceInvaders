#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "TextureManager.hpp"

/**
 * Represent an abstract class for sprite.
 */
class IRenderable
{
public:
    /**
     * Create new sprite.
     * 
     * \param pos Sprite's position.
     * \param size Sprite's size.
     */
    IRenderable(sf::Vector2f pos, sf::Vector2f size)
        : pos_(pos), size_(size) {}
    virtual ~IRenderable() {}

    /**
     * Render sprite to window.
     * 
     * \param window WIndow to which will be sprite rendered.
     */
    virtual void render(sf::RenderWindow& window) const = 0;

    /**
     * Set position of sprite.
     * 
     */
    void setPos(sf::Vector2f pos) { pos_ = pos; }
    /**
     * Get position of sprite.
     * 
     */
    sf::Vector2f getPos() { return pos_; }
    /**
     * Set size of sprite.
     * 
     */
    void setSize(sf::Vector2f size) {size_ = size; }
    /**
     * Get size of sprite.
     * 
     */
    sf::Vector2f getSize() { return size_; }
protected:
    sf::Vector2f pos_;
    sf::Vector2f size_;
};

using IRenderPtr = std::shared_ptr<IRenderable>;

/**
 * Represent a image sprite.
 */
class ImageSprite final : public IRenderable
{
public:
    /**
     * Create new image sprite.
     * 
     * \param pos Position of sprite.
     * \param size Size of sprite.
     * \param name Name of the texture which will sprite use.
     * \param rects Frames used for sprite's animation (rectangles form sprite sheet).
     */
    ImageSprite(sf::Vector2f pos, sf::Vector2f size, const std::string& name, std::vector<sf::IntRect> rects)
        : IRenderable{ pos, size }, rects_{rects}, name_(name) {}

    void render(sf::RenderWindow& window) const override
    {
        sf::Sprite sprite;
        sprite.setTexture(TextureManager::getInstance().get(name_));
        sprite.setTextureRect(rects_[current_]);
        sprite.setOrigin(sf::Vector2f((float)rects_[current_].width, (float)rects_[current_].height) / 2.f);
        sprite.setPosition(pos_);
        sprite.setScale(size_.x / rects_[current_].width, size_.y / rects_[current_].height);

        window.draw(sprite);
    }

    /**
     * Switch to the next frame of animation.
     * 
     */
    void NextFrame()
    {
        current_++;
        if (current_ == rects_.size())
            current_ = 0;
    }

private:
    /**
     * Name of texture.
     */
    std::string name_;
    /**
     * Contains frames used for animation.
     */
    std::vector<sf::IntRect> rects_;
    /**
     * Index of current frame.
     */
    std::size_t current_{ 0 };
};

/**
 * Represent a rectangle sprite.
 */
class RectSprite final : public IRenderable
{
public:
    /**
     * Create a new rectangle sprite.
     * 
     * \param pos Position of rectangle sprite.
     * \param size Size of recntagle sprite.
     * \param col Color of rectangle sprite.
     */
    RectSprite(sf::Vector2f pos, sf::Vector2f size, sf::Color col)
        : IRenderable{pos, size}, col_(col) {}

    void render(sf::RenderWindow& window) const override
    {
        sf::RectangleShape shape;
        shape.setSize(size_);
        shape.setOrigin(size_ / 2.f);
        shape.setPosition(pos_);
        shape.setFillColor(col_);

        window.draw(shape);
    }
private:
    sf::Color col_;
};

/**
 * Represent a text sprite..
 */
class TextSprite final : public IRenderable
{
public:
    /**
     * Create new text sprite.
     * 
     * \param pos Position of sprite.
     * \param font Font whoch sprite's use.
     * \param string Text which will be displayed.
     * \param charSize Size of characters.
     * \param col Color of text.
     */
    TextSprite(sf::Vector2f pos , const sf::Font& font, const std::string& string,
        int charSize, sf::Color col)
        : IRenderable{pos, sf::Vector2f()}, font_(font), string_(string),
        charSize_(charSize), col_(col) {}

    void render(sf::RenderWindow& window) const override
    {
        sf::Text text;
        text.setPosition(pos_);
        text.setFont(font_);
        text.setString(string_);
        text.setCharacterSize(charSize_);
        text.setFillColor(col_);

        window.draw(text);
    }

    /**
     * Get display string.
     * 
     */
    std::string getString() { return string_; }
    /**
     * Set display string.
     * 
     */
    void setString(const std::string& string) { string_ = string; }

private:
    /**
     * Font of text.
     */
    sf::Font font_;
    /**
     * Display text.
     */
    std::string string_;
    /**
     * Size of text's characters.
     */
    int charSize_;
    /**
     * Color of text.
     */
    sf::Color col_;
};

using TextPtr = std::shared_ptr<TextSprite>;

#endif // !SPRITE_HPP