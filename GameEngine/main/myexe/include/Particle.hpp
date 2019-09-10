#pragma once
#include <vector3.hpp>
#include <string>
using namespace mathslib;

class Particle
{
public:
	Particle(double dumping = 0, double inverseMass = 0,  Vector3 position = Vector3(),  Vector3 speed = Vector3(),  Vector3 acceleration = Vector3());
	Particle(Particle const& p);
	void integrate(const Vector3& newAcceleration = Vector3(), double frameTime = 0.0333333);//updates position, speed, acceleration using Newton laws
	double getInverseMass() const;
	Vector3 getPosition() const;
	Vector3 getSpeed() const;
	Vector3 getAcceleration() const;
	std::string toString() const;
	virtual ~Particle();

	bool isVisible(unsigned int xMax, unsigned int yMax) const;

private:
	double dumping;
	double inverseMass;
	Vector3 position;
	Vector3 speed;
	Vector3 acceleration;

	void setPosition(const Vector3& position);
	void setSpeed(const Vector3& speed);
	void setAcceleration(const Vector3& acceleration);

};