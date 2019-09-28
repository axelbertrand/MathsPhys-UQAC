#pragma once
#include <vector3.hpp>
#include <string>

class Particle
{
public:
	Particle(double dumping = 0, double inverseMass = 0, mathslib::Vector3 position = mathslib::Vector3(), 
		mathslib::Vector3 speed = mathslib::Vector3(), mathslib::Vector3 acceleration = mathslib::Vector3());
	Particle(Particle const& anotherParticle);
	virtual ~Particle();

	mathslib::Vector3 getPosition() const;
	mathslib::Vector3 getSpeed() const;
	mathslib::Vector3 getAcceleration() const;
	double getInverseMass() const;
	double getDumping() const;

	// Updates position, speed, acceleration using Newton laws
	void integrate(const mathslib::Vector3& newAcceleration = mathslib::Vector3(), double frameTime = 0.0333333);
	bool isVisible(unsigned int xMax, unsigned int yMax) const;

	std::string toString() const;

private:
	double m_dumping;
	double m_inverseMass;
	mathslib::Vector3 m_position;
	mathslib::Vector3 m_speed;
	mathslib::Vector3 m_acceleration;
};