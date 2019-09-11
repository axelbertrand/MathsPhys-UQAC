#include <iostream>
#include <math.h>
#include "..\include\vector3.hpp"
#include <iostream>//to delete 
namespace mathslib
{
	Vector3::Vector3() {};

	Vector3::Vector3(double x, double y, double z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	Vector3::Vector3(Vector3 const& anotherVector)
	{
		m_x = anotherVector.getX();
		m_y = anotherVector.getY();
		m_z = anotherVector.getZ();
	}

	Vector3& Vector3::operator+=(const Vector3& anotherVector) 
	{
		m_x += anotherVector.getX();
		m_y += anotherVector.getY();
		m_z += anotherVector.getZ();
		return *this;
	}
	Vector3& Vector3::operator-=(Vector3 const& anotherVector)
	{
		m_x -= anotherVector.getX();
		m_y -= anotherVector.getY();
		m_z -= anotherVector.getZ();
		return *this;
	}

	/*static mathslib::Vector3 operator* (double d, mathslib::Vector3 v)
	{
		return v * d;
	}*/

	Vector3 Vector3::VectorAddition(Vector3 const& anotherVector) const
	{
		Vector3 res;
		res.setX(m_x + anotherVector.getX());
		res.setY(m_y + anotherVector.getY());
		res.setZ(m_z + anotherVector.getZ());
		return res;
	}

	Vector3 Vector3::operator+(Vector3 const& anotherVector) const
	{
		return VectorAddition(anotherVector);
	}

	Vector3 Vector3::VectorSubtraction(Vector3 const& anotherVector) const
	{
		Vector3 res;
		res.setX(m_x - anotherVector.getX());
		res.setY(m_y - anotherVector.getY());
		res.setZ(m_z - anotherVector.getZ());
		return res;
	}
	Vector3 Vector3::operator-(Vector3 const& anotherVector) const
	{
		return VectorSubtraction(anotherVector);
	}

	double Vector3::ScalarProduct(Vector3 const& anotherVector) const
	{
		return m_x * anotherVector.getX() + m_y * anotherVector.getY() + m_z * anotherVector.getZ();
	}

	double Vector3::operator*(Vector3 const& anotherVector) const
	{
		return this->ScalarProduct(anotherVector);
	}

	Vector3 Vector3::ScalarMultiplication(double scalar) const
	{
		Vector3 res;
		res.setX(m_x * scalar);
		res.setY(m_y * scalar);
		res.setZ(m_z * scalar);
		return res;
	}

	Vector3 Vector3::operator*(double scalar) const
	{
		return this->ScalarMultiplication(scalar);
	}

	Vector3 Vector3::ScalarDivision(double scalar) const
	{
		if (scalar != 0)
		{
			return ScalarMultiplication(1 / scalar);
		}
		else
		{
			return Vector3(); // in case of division by 0, return (0, 0, 0) vector
		}
	}

	Vector3 Vector3::operator/(double scalar) const
	{
		return ScalarDivision(scalar);
	}

	Vector3 Vector3::CrossProduct(Vector3 const& anotherVector) const
	{
		Vector3 res;
		res.setX((m_y * anotherVector.getZ()) - (m_z * anotherVector.getY()));
		res.setY((m_z * anotherVector.getX()) - (m_x * anotherVector.getZ()));
		res.setZ((m_x * anotherVector.getY()) - (m_y * anotherVector.getX()));
		return res;
	}

	Vector3 Vector3::operator^(Vector3 const& anotherVector) const
	{
		return this->CrossProduct(anotherVector);
	}

	Vector3 Vector3::ComponentProduct(Vector3 const& anotherVector) const
	{
		Vector3 res;
		res.setX(m_x * anotherVector.getX());
		res.setY(m_y * anotherVector.getY());
		res.setZ(m_z * anotherVector.getZ());
		return res;
	}

	double Vector3::getNorm() const
	{
		return pow(this->getSquaredNorm(), 0.5);
	}

	double Vector3::getSquaredNorm() const
	{
		return (m_x * m_x + m_y * m_y + m_z * m_z);
	}

	Vector3 Vector3::getNormalizedVector() const
	{
		return (*this / (this->getNorm()));//be careful here
	}

	std::string Vector3::toString() const
	{
		return("x = " + std::to_string(m_x) + " ; y = " + std::to_string(m_y) + " ; z = " + std::to_string(m_z));
	}

	
}

