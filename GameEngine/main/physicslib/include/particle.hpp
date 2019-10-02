#pragma once
#include <vector3.hpp>
#include <string>
namespace physicslib
{	
	class Particle
	{
	public:
		Particle(double dumping = 0, double inverseMass = 0, physicslib::Vector3 position = physicslib::Vector3(),
			physicslib::Vector3 speed = physicslib::Vector3(), physicslib::Vector3 acceleration = physicslib::Vector3());
		Particle(Particle const& anotherParticle);
		virtual ~Particle();

		physicslib::Vector3 getPosition() const;
		physicslib::Vector3 getSpeed() const;
		physicslib::Vector3 getAcceleration() const;
		double getInverseMass() const;
		double getDumping() const;

		// Updates position, speed, acceleration using Newton laws
		void integrate(double frameTime = 0.0333333);
		void addForce(const physicslib::Vector3& force);
		void clearAccumulator();
		bool isVisible(unsigned int xMax, unsigned int yMax) const;

		std::string toString() const;

	private:
		double m_dumping;
		double m_inverseMass;
		physicslib::Vector3 m_position;
		physicslib::Vector3 m_speed;
		physicslib::Vector3 m_acceleration;
		physicslib::Vector3 m_forceAccumulator;
	};
}