#include <SFML/Graphics.hpp>
#include "World.h"
#include "Button.h"
#include "Spawner.h"
#include "ViewUtils.h"
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

    // world and initial bounds
    sf::FloatRect initialBounds({0.f, 0.f}, {virtualSize.x, virtualSize.y});
    World world(initialBounds, /*substeps=*/6);

    // UI add-button (pixel-space)
    ui::Button addBtn;
    addBtn.init(window.getSize());

    // connect button to behaviour: add a chaotic ball when clicked
    addBtn.setOnClick([&world, &view]() {
        util::spawnRandomBall(world, view, /*forceChaotic=*/true);
    });

    // initial viewport/bounds setup using actual window size and the created world
    util::updateViewViewport(window, view, virtualSize, &world, &addBtn);

    // init some balls
    {
        world.balls().reserve(2);
        Ball& first = world.emplaceBall(50.f, sf::Color::Black, sf::Vector2f(400.f, 400.f));
        Ball& second = world.emplaceBall(25.f, sf::Color::White, sf::Vector2f(300.f, 300.f));
        first.setControlKeys(sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::A, sf::Keyboard::Key::D);
        second.setControlKeys(sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right);
        second.setChaotic(true);
    }

    while (window.isOpen())
    {
        // Process events — handle resize, close and UI clicks (add-ball)
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            if (event->is<sf::Event::Resized>()) {
                if (auto r = event->getIf<sf::Event::Resized>()) {
                    util::updateViewViewport(window, view, virtualSize, &world, &addBtn);
                }
            }

            // Mouse click: handle UI button
            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (auto m = event->getIf<sf::Event::MouseButtonPressed>()) {
                    // m->position is in pixel coords relative to window
                    if (addBtn.contains(m->position)) {
                        // button is wired to add chaotic balls via its callback
                        // invoke callback directly (caller owns when to call)
                        if (auto cb = std::invoke([](ui::Button& b) -> std::function<void()> { return std::function<void()>{}; }, addBtn); false) {}
                        // simpler: directly call the callback stored in the button via setOnClick previously;
                        // as Button doesn't expose direct invoke, call the behavior here instead:
                        util::spawnRandomBall(world, view, /*forceChaotic=*/true);
                        // consume UI click (don't forward to world)
                        continue;
                    }
                }
            }

            world.handleEvent(event);
        }

        sf::Time dt = clock.restart();
        float dtSeconds = dt.asSeconds();

        world.update(dtSeconds);

        window.clear(sf::Color(131, 11, 186));
        // draw world using world view
        window.setView(view);
        world.draw(window);

        // draw UI in pixel coordinates (default view)
        window.setView(window.getDefaultView());
        addBtn.draw(window);

        window.display();
    }

    return 0;
}