#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include <vector>
#include <optional>

class World
{
public:
    explicit World(const sf::FloatRect& bounds, int substeps = 6);

    // create a ball in-place and return a reference so the caller can configure it
    Ball& emplaceBall(float radius, sf::Color color, sf::Vector2f position);

    void handleEvent(const std::optional<sf::Event>& event);
    void update(float dtSeconds);
    void draw(sf::RenderWindow& window) const;

    // access to balls for configuration/tests
    std::vector<Ball>& balls() { return m_balls; }
    const std::vector<Ball>& balls() const { return m_balls; }

    void setSubsteps(int s) { m_substeps = std::max(1, s); }

    // Allow adjusting the world bounds at runtime (used when the view expands)
    void setBounds(const sf::FloatRect& bounds) { m_bounds = bounds; }

private:
    // Modularized steps used by update()
    void updateVelocities(float dt);
    void resolveCollisions();
    void integrateAndConstrain(float dt);

    sf::FloatRect m_bounds;
    std::vector<Ball> m_balls;
    int m_substeps{ 6 };
};