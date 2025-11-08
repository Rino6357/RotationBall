#include "ui/Slider.h"

Slider::Slider(float x, float y, float width, float min, float max, float initial)
    : m_min(min), m_max(max), m_value(initial)
{
    m_bar.setPosition(x, y);
    m_bar.setSize({ width, 6 });
    m_bar.setFillColor(sf::Color(180, 180, 180));

    m_knob.setRadius(10);
    m_knob.setOrigin(10, 10);
    m_knob.setFillColor(sf::Color::Red);
    float knobX = x + ((initial - min) / (max - min)) * width;
    m_knob.setPosition(knobX, y + 3);
}

void Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (m_knob.getGlobalBounds().contains(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y })))
            m_dragging = true;
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        m_dragging = false;
    }
    if (event.type == sf::Event::MouseMoved && m_dragging) {
        float x = m_bar.getPosition().x;
        float width = m_bar.getSize().x;
        float mx = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }).x;
        mx = std::max(x, std::min(x + width, mx));
        m_knob.setPosition(mx, m_knob.getPosition().y);
        m_value = m_min + ((mx - x) / width) * (m_max - m_min);
    }
}

void Slider::draw(sf::RenderWindow& window) const {
    window.draw(m_bar);
    window.draw(m_knob);
}

float Slider::getValue() const {
    return m_value;
}