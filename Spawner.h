#pragma once
#include <SFML/Graphics.hpp>
#include "World.h"

namespace util
{
    // Spawn a new ball with random size/color at a random position inside the current view.
    // Keep this in a separate translation unit so main.cpp remains focused on wiring.
    void spawnRandomBall(World& world, const sf::View& view, bool forceChaotic = false);
}