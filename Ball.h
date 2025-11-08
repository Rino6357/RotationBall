#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
public:
	Ball(float radius, sf::Color color, sf::Vector2f position);

	void draw(sf::RenderWindow& window) const;

	sf::Vector2f getPosition() const;

	void move(sf::Vector2f vector);

	// Update velocity from input / AI
	void update(float dt);

	bool isCollision(const Ball& other) const;

	// Resolve overlap and adjust velocities (physically based impulse)
	void resolveCollision(Ball& other);

	// Integrate position from velocity
	void applyMovement(float dt);

	// Assign control keys; use sf::Keyboard::Key::Unknown to disable a direction
	void setControlKeys(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right);

	// Make this ball chaotic (random wandering + wall bounces)
	void setChaotic(bool chaotic);
	bool isChaotic() const;

	// Constrain to bounds and bounce off walls (use after applyMovement)
	void constrainToBounds(const sf::FloatRect& bounds);

private:
	float m_radius{};
	sf::Color m_color{};
	sf::Vector2f m_origin{};
	sf::Vector2f m_position{};
	sf::Vector2f m_velocity{}; // units per second
	sf::CircleShape m_structure{};
	float speed{ 300.f };

	// Mass for physics (proportional to area)
	float m_mass{ 1.f };

	// Per-ball control keys (default disabled)
	sf::Keyboard::Key m_keyUp{ sf::Keyboard::Key::Unknown };
	sf::Keyboard::Key m_keyDown{ sf::Keyboard::Key::Unknown };
	sf::Keyboard::Key m_keyLeft{ sf::Keyboard::Key::Unknown };
	sf::Keyboard::Key m_keyRight{ sf::Keyboard::Key::Unknown };

	// Chaotic behavior
	bool m_chaotic{ false };
};

