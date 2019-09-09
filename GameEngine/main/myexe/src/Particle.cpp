#include "../include/Particle.hpp"
#include <math.h>


Particle::Particle(double dumping, double inverseMass,  Vector3 position,  Vector3 speed,  Vector3 acceleration)
{
	this->dumping = dumping;
	this->inverseMass = inverseMass;
	this->position = position;
	this->speed = speed;
	this->acceleration = acceleration;
}

Particle::Particle(Particle const& p)
{
	dumping = p.dumping;
	inverseMass = p.inverseMass;
	position = Vector3(p.position);
	speed = Vector3(p.speed);
	acceleration = Vector3(p.acceleration);
}


void Particle::integrate(const Vector3& newAcceleration, double frameTime)
{
	position += speed.ScalarMultiplication(frameTime);
	speed = speed.ScalarMultiplication(pow(dumping, frameTime)).VectorAddition(acceleration.ScalarMultiplication(frameTime));
	this->acceleration = newAcceleration;
}

double Particle::getInverseMass() const
{
	return inverseMass;
}

Vector3 Particle::getPosition() const
{
	return position;
}

Vector3 Particle::getSpeed() const
{
	return speed;
}

Vector3 Particle::getAcceleration() const
{
	return acceleration;
}

void Particle::setPosition(const Vector3& p) 
{
	position = p;
}

void Particle::setSpeed(const Vector3& s) 
{
	speed = s;
}

void Particle::setAcceleration(const Vector3& a)
{
	acceleration = a;
}

std::string Particle::toString() const
{
	return("dumping = " + std::to_string(dumping) + " ; inverseMass = " + std::to_string(inverseMass) + " ; position = (" + position.toString() + ") ; speed = (" + speed.toString() + ") ; acceleration = (" + acceleration.toString() +")");
}

Particle::~Particle()
{
}
