#include <SFML/Graphics.hpp>
#include "Ball.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "SFML works!");
    sf::Clock deltaClock{};

    Ball first{ 50.f, sf::Color::Black, sf::Vector2f(400, 400)};
    Ball second{ 25.f, sf::Color::White, sf::Vector2f(300, 300) };


    while (window.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();
        float dtSeconds = deltaTime.asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Num9)
                    std::cout << first.getPosition().x << ", " << first.getPosition().y << '\n';
            }
        }

        first.update(dtSeconds);

        window.clear(sf::Color(131, 11, 186));
        first.draw(window);
        second.draw(window);
        window.display();
    }
}