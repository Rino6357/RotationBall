#include "World.h"
#include <iostream>
#include <algorithm>

World::World(const sf::FloatRect& bounds, int substeps)
    : m_bounds(bounds), m_substeps(std::max(1, substeps))
{
}

Ball& World::emplaceBall(float radius, sf::Color color, sf::Vector2f position)
{
    m_balls.emplace_back(radius, color, position);
    return m_balls.back();
}

void World::handleEvent(const std::optional<sf::Event>& event)
{
    if (!event)
        return;

    // Example: print positions when Num9 pressed (keeps behaviour from previous main)
    if (event->is<sf::Event::KeyPressed>()) {
        auto keyEvent = event->getIf<sf::Event::KeyPressed>();
        if (keyEvent && keyEvent->code == sf::Keyboard::Key::Num9) {
            for (size_t i = 0; i < m_balls.size(); ++i) {
                auto p = m_balls[i].getPosition();
                std::cout << "ball[" << i << "] = " << p.x << ", " << p.y << '\n';
            }
        }
    }
}

void World::update(float dtSeconds)
{
    if (dtSeconds <= 0.f)
        return;

    float subdt = dtSeconds / static_cast<float>(m_substeps);

    for (int step = 0; step < m_substeps; ++step)
    {
        // 1) update velocities (input / AI)
        updateVelocities(subdt);

        // 2) resolve collisions pairwise
        resolveCollisions();

        // 3) integrate and constrain to bounds
        integrateAndConstrain(subdt);
    }
}

void World::draw(sf::RenderWindow& window) const
{
    for (auto& b : m_balls)
        b.draw(window);
}

// --- Modular implementation details ---

void World::updateVelocities(float dt)
{
    for (auto& b : m_balls)
        b.update(dt);
}

void World::resolveCollisions()
{
    const size_t n = m_balls.size();
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = i + 1; j < n; ++j)
        {
            if (m_balls[i].isCollision(m_balls[j]))
                m_balls[i].resolveCollision(m_balls[j]);
        }
    }
}

void World::integrateAndConstrain(float dt)
{
    for (auto& b : m_balls) {
        b.applyMovement(dt);
        b.constrainToBounds(m_bounds);
    }
}