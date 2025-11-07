#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
public:
	Ball(float radius, sf::Color color, sf::Vector2f position);

	void draw(sf::RenderWindow& window);

	sf::Vector2f getPosition() const;

	void move(sf::Vector2f vector);

	void update(float dt);

	bool isCollision(Ball ball);

private:
	float m_radius{};
	sf::Color m_color{};
	sf::Vector2f m_origin{};
	sf::Vector2f m_position{};
	sf::Vector2f m_velocity{};
	sf::CircleShape m_structure{};
	float speed{ 300 };

	void move() {

	}

};

