#include "Spawner.h"
#include <random>
#include <cstdint>

namespace util
{
    void spawnRandomBall(World& world, const sf::View& view, bool forceChaotic)
    {
        static thread_local std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> rdist(12.f, 48.f);
        std::uniform_real_distribution<float> ux(0.f, view.getSize().x);
        std::uniform_real_distribution<float> uy(0.f, view.getSize().y);
        std::uniform_int_distribution<int> c(50, 230);

        float radius = rdist(rng);
        sf::Color color(static_cast<std::uint8_t>(c(rng)),
                        static_cast<std::uint8_t>(c(rng)),
                        static_cast<std::uint8_t>(c(rng)));

        // view origin is [0,0] (we center it at size*0.5f), so position in world coords is just the random uv
        sf::Vector2f pos(ux(rng), uy(rng));
        world.balls().reserve(world.balls().size() + 1);
        Ball& b = world.emplaceBall(radius, color, pos);

        // Optionally give newly created ball some behavior (randomly chaotic)
        if (forceChaotic || ((c(rng) % 3) == 0))
            b.setChaotic(true);
    }
}