#include "../include/Particle.hpp"
#include <math.h>


Particle::Particle(double dumping, double inverseMass,  mathslib::Vector3 position, mathslib::Vector3 speed, mathslib::Vector3 acceleration):
	m_dumping(dumping), m_inverseMass(inverseMass), m_position(position), m_speed(speed), m_acceleration(acceleration), m_forceAccumulator()
{
}

Particle::Particle(Particle const& anotherParticle):
	m_dumping(anotherParticle.getDumping()), m_inverseMass(anotherParticle.getInverseMass()), m_position(mathslib::Vector3(anotherParticle.getPosition())),
	m_speed(mathslib::Vector3(anotherParticle.getSpeed())), m_acceleration(mathslib::Vector3(anotherParticle.getAcceleration()))
{
}

Particle::~Particle()
{
}

//indicates whether or not a particle is visible or not on the screen
bool Particle::isVisible(unsigned int xMax, unsigned int yMax) const
{
	double x = getPosition().getX();
	double y = getPosition().getY();
	if (x - 5 >= xMax || x + 5 <= 0 || y - 5 >= yMax || y + 5 <= 0)
	{
		return false;
	}
	return true;
}

// Updates position, speed, acceleration using Newton laws
void Particle::integrate(double frameTime)
{
	m_position += m_speed * frameTime;
	m_speed = m_speed * pow(m_dumping, frameTime) + m_acceleration * frameTime;
	m_acceleration = m_forceAccumulator;
	clearAccumulator();
}

void Particle::addForce(const mathslib::Vector3& force)
{
	m_forceAccumulator += force;
}

void Particle::clearAccumulator()
{
	m_forceAccumulator = mathslib::Vector3();
}

double Particle::getInverseMass() const
{
	return m_inverseMass;
}

mathslib::Vector3 Particle::getPosition() const
{
	return m_position;
}

mathslib::Vector3 Particle::getSpeed() const
{
	return m_speed;
}

double Particle::getDumping() const
{
	return m_dumping;
}

mathslib::Vector3 Particle::getAcceleration() const
{
	return m_acceleration;
}

std::string Particle::toString() const
{
	std::string result = "dumping = " + std::to_string(m_dumping) + "\n";
	result += "inverseMass = " + std::to_string(m_inverseMass) + "\n";
	result += "position = (" + m_position.toString() + ")\n";
	result += "speed = (" + m_speed.toString() + ")\n";
	result += "acceleration = (" + m_acceleration.toString() + ")\n";
	return(result);
}
