#pragma once
#include <vector3.hpp>
#include <string>

class Particle
{
public:
	Particle(double inverseMass = 0, mathslib::Vector3 position = mathslib::Vector3(), 
		mathslib::Vector3 speed = mathslib::Vector3(), mathslib::Vector3 acceleration = mathslib::Vector3());
	Particle(Particle const& anotherParticle);
	virtual ~Particle();

	mathslib::Vector3 getPosition() const;
	mathslib::Vector3 getSpeed() const;
	mathslib::Vector3 getAcceleration() const;
	double getInverseMass() const;

	// Updates position, speed, acceleration using Newton laws
	void integrate(double frameTime = 0.0333333);
	void addForce(const mathslib::Vector3& force);
	void clearAccumulator();
	bool isVisible(unsigned int xMax, unsigned int yMax) const;

	std::string toString() const;

private:
	double m_inverseMass;
	mathslib::Vector3 m_position;
	mathslib::Vector3 m_speed;
	mathslib::Vector3 m_acceleration;
	mathslib::Vector3 m_forceAccumulator;
};