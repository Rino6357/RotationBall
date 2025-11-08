#include <SFML/Graphics.hpp>
#include "World.h"
#include <iostream>

int main()
{
    // Virtual coordinate space used by your game logic / world
    const sf::Vector2f virtualSize(600.f, 600.f);

    sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "SFML works!");
    sf::Clock clock{};

    // Setup a view that maps the virtual coordinate space (600x600) to the window.
    // We'll update the view's logical size on resize to preserve aspect ratio (no distortion)
    // and expand the world bounds so balls can travel into newly visible areas.
    sf::View view(sf::FloatRect({0.f, 0.f}, {virtualSize.x, virtualSize.y}));
    window.setView(view);

    // Resize helper: when the window aspect changes, adjust the view size so
    // the scale is uniform (no distortion). Keep the smaller axis equal to
    // virtualSize and expand the larger axis to match window aspect.
    // If 'worldPtr' is non-null we also update the world's bounds to the new view size.
    auto updateViewViewport = [&](unsigned winW, unsigned winH, World* worldPtr = nullptr) {
        float windowRatio = static_cast<float>(winW) / static_cast<float>(winH);
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
    };

    // bounds built using sf::FloatRect(position, size)
    sf::FloatRect initialBounds({0.f, 0.f}, {virtualSize.x, virtualSize.y});

    World world(initialBounds, /*substeps=*/6);

    // initial viewport/bounds setup using actual window size and the created world
    updateViewViewport(window.getSize().x, window.getSize().y, &world);

    // Reserve space so references returned by emplaceBall remain valid
    world.balls().reserve(2);

    // create balls and configure them
    Ball& first = world.emplaceBall(50.f, sf::Color::Black, sf::Vector2f(400.f, 400.f));
    Ball& second = world.emplaceBall(25.f, sf::Color::White, sf::Vector2f(300.f, 300.f));

    first.setControlKeys(sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::A, sf::Keyboard::Key::D);
    second.setControlKeys(sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right);

    second.setChaotic(true);

    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        float dtSeconds = dt.asSeconds();

        // event handling kept minimal; world receives events it cares about
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            // respond to resize events so the view scales content to the new window size
            if (event->is<sf::Event::Resized>()) {
                if (auto r = event->getIf<sf::Event::Resized>()) {
                    updateViewViewport(r->size.x, r->size.y, &world);
                }
            }

            world.handleEvent(event);
        }

        world.update(dtSeconds);

        window.clear(sf::Color(131, 11, 186));
        world.draw(window);
        window.display();
    }
}