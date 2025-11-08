#include "Ball.h"
#include <cmath>
#include <algorithm>
#include <random>

Ball::Ball(float radius, sf::Color color, sf::Vector2f position)
	: m_radius{ radius }, m_color{ color }, m_position{ position }
{
	this->m_structure.setRadius(m_radius);
	// set origin first so .setPosition() refers to the center
	this->m_structure.setOrigin(sf::Vector2f(m_radius, m_radius));
	this->m_structure.setFillColor(m_color);
	this->m_structure.setPosition(m_position);

	// Mass proportional to area (pi factor not needed for relative behaviour)
	m_mass = m_radius * m_radius;
	if (m_mass <= 0.f) m_mass = 1.f;
}

void Ball::draw(sf::RenderWindow& window) const
{
    window.draw(m_structure);
}

sf::Vector2f Ball::getPosition() const {
	return m_structure.getPosition();
}

void Ball::move(sf::Vector2f vector) {
	m_structure.move(vector);
}

void Ball::update(float dt) {
    if (m_chaotic) {
        // Chaotic balls ignore control keys and maintain a wandering velocity.
        // Use a per-call random jitter to slightly change direction while keeping speed.
        static thread_local std::mt19937 rng{ std::random_device{}() };
        static std::uniform_real_distribution<float> angleJitter(-0.6f, 0.6f); // radians/sec jitter magnitude

        // If velocity nearly zero, give a random initial direction
        const float minSpeed = 10.f;
        if (std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y) < minSpeed) {
            std::uniform_real_distribution<float> ang(0.f, 2.f * 3.14159265f);
            float a = ang(rng);
            m_velocity.x = std::cos(a) * speed;
            m_velocity.y = std::sin(a) * speed;
        } else {
            // rotate velocity by a small jitter proportional to dt
            float a = std::atan2(m_velocity.y, m_velocity.x);
            float jitter = angleJitter(rng) * dt;
            a += jitter;
            float vlen = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
            if (vlen <= 0.f) vlen = speed;
            // keep target speed close to configured speed
            vlen = std::clamp(vlen, speed * 0.6f, speed * 1.2f);
            m_velocity.x = std::cos(a) * vlen;
            m_velocity.y = std::sin(a) * vlen;
        }

        // small damping so chaotic balls don't accelerate unboundedly
        const float damping = 0.5f;
        m_velocity -= m_velocity * std::min(damping * dt, 1.f);

        return;
    }

    // Determine input direction
    sf::Vector2f inputDir(0.f, 0.f);
    if (m_keyUp != sf::Keyboard::Key::Unknown && sf::Keyboard::isKeyPressed(m_keyUp))
        inputDir.y -= 1.f;
    if (m_keyDown != sf::Keyboard::Key::Unknown && sf::Keyboard::isKeyPressed(m_keyDown))
        inputDir.y += 1.f;
    if (m_keyLeft != sf::Keyboard::Key::Unknown && sf::Keyboard::isKeyPressed(m_keyLeft))
        inputDir.x -= 1.f;
    if (m_keyRight != sf::Keyboard::Key::Unknown && sf::Keyboard::isKeyPressed(m_keyRight))
        inputDir.x += 1.f;

    // Normalize input direction if needed
    if (inputDir.x != 0.f || inputDir.y != 0.f) {
        float len = std::sqrt(inputDir.x * inputDir.x + inputDir.y * inputDir.y);
        inputDir.x /= len;
        inputDir.y /= len;
        // Set velocity directly from input direction (player-controlled)
        m_velocity = inputDir * speed;
    } else {
        // No input: simple damping to gradually stop
        const float damping = 8.f; // larger -> quicker stop
        m_velocity -= m_velocity * std::min(damping * dt, 1.f);
    }

    // Note: Do not integrate position here; call applyMovement(dt) after collision resolution
}

void Ball::applyMovement(float dt) {
    sf::Vector2f displacement = m_velocity * dt;
    this->move(displacement);
}

bool Ball::isCollision(const Ball& other) const {
    sf::Vector2f a = this->getPosition();
    sf::Vector2f b = other.getPosition();
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float r = this->m_radius + other.m_radius;
    return (dx * dx + dy * dy) <= (r * r);
}

void Ball::resolveCollision(Ball& other) {
    if (!this->isCollision(other))
        return;

    sf::Vector2f posA = this->getPosition();
    sf::Vector2f posB = other.getPosition();

    sf::Vector2f normal = posB - posA;
    float dist2 = normal.x * normal.x + normal.y * normal.y;
    float dist = std::sqrt(dist2);

    if (dist == 0.f) {
        // Avoid division by zero — pick arbitrary normal
        normal = sf::Vector2f(1.f, 0.f);
        dist = 1.f;
    } else {
        normal.x /= dist;
        normal.y /= dist;
    }

    float penetration = (this->m_radius + other.m_radius) - dist;
    if (penetration > 0.f) {
        // Positional correction (Baumgarte-like): correct proportionally to inverse mass
        const float percent = 0.8f; // how much to correct (0..1)
        const float slop = 0.01f;   // small allowance
        float invMassA = 1.f / m_mass;
        float invMassB = 1.f / other.m_mass;
        float invMassSum = invMassA + invMassB;
        float correctionMag = std::max(penetration - slop, 0.f) / invMassSum * percent;
        sf::Vector2f correction = normal * correctionMag;
        // Move objects out of overlap according to inverse mass
        this->move(-correction * invMassA);
        other.move(correction * invMassB);
    }

    // --- Impulse resolution on velocities ---
    // relative velocity: vB - vA
    sf::Vector2f rv = other.m_velocity - this->m_velocity;
    float velAlongNormal = rv.x * normal.x + rv.y * normal.y;

    // Do not resolve if velocities are separating
    if (velAlongNormal > 0.f)
        return;

    float e = 0.9f; // restitution (0 = inelastic, 1 = perfectly elastic)

    float invMassA = 1.f / m_mass;
    float invMassB = 1.f / other.m_mass;
    float j = -(1.0f + e) * velAlongNormal;
    j = j / (invMassA + invMassB);

    sf::Vector2f impulse = normal * j;

    // Apply impulse
    this->m_velocity -= impulse * invMassA;
    other.m_velocity += impulse * invMassB;
}

void Ball::setControlKeys(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right) {
    m_keyUp = up;
    m_keyDown = down;
    m_keyLeft = left;
    m_keyRight = right;
}

void Ball::setChaotic(bool chaotic) {
    m_chaotic = chaotic;
    if (m_chaotic) {
        // give an initial random velocity
        static thread_local std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> ang(0.f, 2.f * 3.14159265f);
        float a = ang(rng);
        m_velocity.x = std::cos(a) * speed;
        m_velocity.y = std::sin(a) * speed;
    }
}

bool Ball::isChaotic() const {
    return m_chaotic;
}

void Ball::constrainToBounds(const sf::FloatRect& bounds) {
        // Use correct sf::FloatRect members: position/size (not left/top/width/height)
    float left = bounds.position.x;
    float top = bounds.position.y;
    float right = left + bounds.size.x;
    float bottom = top + bounds.size.y;

    sf::Vector2f pos = this->getPosition();
    bool bounced = false;
    const float restitution = 0.9f;

    if (pos.x - m_radius < left) {
        pos.x = left + m_radius;
        m_velocity.x = std::abs(m_velocity.x) * restitution;
        bounced = true;
    } else if (pos.x + m_radius > right) {
        pos.x = right - m_radius;
        m_velocity.x = -std::abs(m_velocity.x) * restitution;
        bounced = true;
    }

    if (pos.y - m_radius < top) {
        pos.y = top + m_radius;
        m_velocity.y = std::abs(m_velocity.y) * restitution;
        bounced = true;
    } else if (pos.y + m_radius > bottom) {
        pos.y = bottom - m_radius;
        m_velocity.y = -std::abs(m_velocity.y) * restitution;
        bounced = true;
    }

    if (bounced) {
        // apply positional correction and update shape position
        m_structure.setPosition(pos);
    }
}