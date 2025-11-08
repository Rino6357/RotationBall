#pragma once
#include <SFML/Graphics.hpp>

class Slider {
public:
    Slider(float x, float y, float width, float min, float max, float initial);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
    float getValue() const;

private:
    float m_min, m_max, m_value;
    sf::RectangleShape m_bar;
    sf::CircleShape m_knob;
    bool m_dragging = false;
};