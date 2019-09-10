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

bool Particle::isVisible(unsigned int xMax, unsigned int yMax) const
{
	float x = getPosition().getX();
	float y = getPosition().getY();
	if (x - 5 >= xMax || x + 5 <= 0 || y - 5 >= yMax || y + 5 <= 0)
	{
		return false;
	}
	return true;
}


void Particle::integrate(const Vector3& newAcceleration, double frameTime)
{
	position += speed * frameTime;
	speed = speed * pow(dumping, frameTime) + acceleration * frameTime;
	acceleration = newAcceleration;
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
