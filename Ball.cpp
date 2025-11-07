#include "Ball.h"
#include <iostream>

Ball::Ball(float radius, sf::Color color, sf::Vector2f position)
	: m_radius{ radius }, m_color{ color }, m_position{ position } 
{
	this->m_structure.setRadius(m_radius);
	this->m_structure.setFillColor(m_color);
	this->m_structure.setPosition(m_position);
    this->m_structure.setOrigin(sf::Vector2f(m_radius, m_radius));
}

void Ball::draw(sf::RenderWindow& window) {
	window.draw(m_structure);
}

sf::Vector2f Ball::getPosition() const {
	return m_structure.getPosition();
}

void Ball::move(sf::Vector2f vector) {
	m_structure.move(vector);
}

void Ball::update(float dt) {
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed * dt;

    // Store for collision response
    m_velocity = movement;

    this->move(sf::Vector2f(movement.x, 0.f));

    this->move(sf::Vector2f(0.f, movement.y));
}

bool Ball::isCollision(Ball ball) {

    sf::Vector2f vectorDistance{ std::abs(this->getPosition().x - ball.getPosition().x), std::abs(this->getPosition().y - ball.getPosition().y) }; // Vector distance between the two balls
    float magnitudeDistance{ std::sqrt(vectorDistance.x * vectorDistance.x + vectorDistance.y * vectorDistance.y) }; // Magnitude of the distance between the two balls
    float requiredDistance{ this->m_radius + ball.m_radius }; // The minimum distance required for the balls to be considered colliding

    if (magnitudeDistance <= requiredDistance) {
        return true;
    }
    return false;
}