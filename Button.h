#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

namespace ui
{
    // Small pixel-space add-ball button (no font required)
    // Extracted from main.cpp to be reusable for future UI elements.
    class Button
    {
    public:
        Button(float size = 48.f, float margin = 10.f);

        void init(const sf::Vector2u& winSize);

        void updatePosition(const sf::Vector2u& winSize);

        bool contains(const sf::Vector2i& pixelPos) const;

        void draw(sf::RenderWindow& window) const;

        // Set callback invoked when the button is activated by the caller
        void setOnClick(std::function<void()> cb);

    private:
        sf::RectangleShape bg;
        sf::RectangleShape hor;
        sf::RectangleShape ver;
        float m_size;
        float m_margin;
        std::function<void()> m_onClick;
    };
}