#include "Button.h"

namespace ui
{
    Button::Button(float size, float margin)
        : m_size(size)
        , m_margin(margin)
    {
    }

    void Button::init(const sf::Vector2u& winSize)
    {
        bg.setSize({m_size, m_size});
        bg.setFillColor(sf::Color(50, 50, 50, 220));
        bg.setOutlineThickness(2.f);
        bg.setOutlineColor(sf::Color::White);

        // horizontal and vertical thin rectangles for the plus sign
        hor.setSize({m_size * 0.6f, m_size * 0.12f});
        hor.setFillColor(sf::Color::White);
        ver.setSize({m_size * 0.12f, m_size * 0.6f});
        ver.setFillColor(sf::Color::White);

        updatePosition(winSize);
    }

    void Button::updatePosition(const sf::Vector2u& winSize)
    {
        const float x = static_cast<float>(winSize.x) - m_margin - m_size;
        const float y = m_margin;
        bg.setPosition(sf::Vector2f(x, y));

        // center plus sign inside bg
        const auto center = bg.getPosition() + bg.getSize() * 0.5f;
        hor.setOrigin(hor.getSize() * 0.5f);
        ver.setOrigin(ver.getSize() * 0.5f);
        hor.setPosition(center);
        ver.setPosition(center);
    }

    bool Button::contains(const sf::Vector2i& pixelPos) const
    {
        return bg.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y)));
    }

    void Button::draw(sf::RenderWindow& window) const
    {
        // Draw in default (pixel) coordinates — caller must set default view
        window.draw(bg);
        window.draw(hor);
        window.draw(ver);
    }

    void Button::setOnClick(std::function<void()> cb)
    {
        m_onClick = std::move(cb);
    }
}