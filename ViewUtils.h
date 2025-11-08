#pragma once
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Button.h"

namespace util
{
    // Resize helper: adjust the view logical size so scale is uniform (no distortion)
    // and optionally update world bounds; also updates the button screen position.
    void updateViewViewport(sf::RenderWindow& window, sf::View& view, const sf::Vector2f& virtualSize, World* worldPtr, ui::Button* addBtn);
}