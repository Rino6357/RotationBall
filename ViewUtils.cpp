#include "ViewUtils.h"

namespace util
{
    void updateViewViewport(sf::RenderWindow& window, sf::View& view, const sf::Vector2f& virtualSize, World* worldPtr, ui::Button* addBtn)
    {
        const auto winSize = window.getSize();
        float windowRatio = static_cast<float>(winSize.x) / static_cast<float>(winSize.y);
        float targetRatio = virtualSize.x / virtualSize.y;

        // Compute new logical view size that matches the window aspect ratio
        sf::Vector2f newSize;
        if (windowRatio > targetRatio) {
            // Window is wider -> keep virtual height, expand width
            newSize.y = virtualSize.y;
            newSize.x = virtualSize.y * windowRatio;
        } else {
            // Window is taller -> keep virtual width, expand height
            newSize.x = virtualSize.x;
            newSize.y = virtualSize.x / windowRatio;
        }

        view.setSize(newSize);
        // keep top-left at (0,0) by centering the view on half the size
        view.setCenter(newSize * 0.5f);

        // full viewport (no letterbox); view size already matches window aspect so no distortion
        view.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));

        window.setView(view);

        if (worldPtr) {
            // update world bounds so balls can go into newly-visible area
            worldPtr->setBounds(sf::FloatRect({0.f, 0.f}, newSize));
        }

        // update UI button position in pixel coordinates
        if (addBtn)
            addBtn->updatePosition(winSize);
    }
}