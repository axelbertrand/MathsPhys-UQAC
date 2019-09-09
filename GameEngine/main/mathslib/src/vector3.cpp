#include <iostream>
#include "..\include\vector3.hpp"
namespace mathslib
{
	Vector3::Vector3() {};
	Vector3::Vector3(double i, double j, double k)
	{
		x = i;
		y = j;
		z = k;
	};
	Vector3::Vector3(Vector3 const& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	};
	void Vector3::operator+=(const Vector3& v) 
	{
		x += v.x;
		y += v.y;
		z += v.z;
	};
	void Vector3::operator-=(Vector3 const& v) 
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	};


	/*Vector3 operator+(Vector3 const& u, Vector3 const& v)
	{
		Vector3 res;
		res = u.VectorAddition(v);
		return res;
	};*/

	Vector3 Vector3::operator+(Vector3 const& v) const
	{
		Vector3 res;
		res = this->VectorAddition(v);
		return res;
	};

	Vector3 Vector3::operator-(Vector3 const& v) const
	{
		Vector3 res;
		res.x = x - v.x;
		res.y = y - v.y;
		res.z = z - v.z;
		return res;
	};
	/*double operator*(Vector3 const& u, Vector3 const& v)
	{
		return u.ScalarProduct(v);
	}*/

	double Vector3::operator*(Vector3 const& v) const
	{
		return this->ScalarProduct(v);
	};

	Vector3 Vector3::operator*(double k) const
	{
		Vector3 res;
		res = this->ScalarMultiplication(k);
		return res;
	};

	/*Vector3 operator^(Vector3 const& u, Vector3 const& v)
	{
		return u.CrossProduct(v);
	}*/

	Vector3 Vector3::operator^(Vector3 const& v) const
	{
		return this->CrossProduct(v);
	}
	Vector3 Vector3::VectorAddition(Vector3 const& v) const
	{
		Vector3 res;
		res.x = v.x + x;
		res.y = v.y + y;
		res.z = v.z + z;
		return res;
	};
	Vector3 Vector3::ScalarMultiplication(double const& k) const
	{
		Vector3 res = Vector3(*this);
		res.x *= k;
		res.y *= k;
		res.z *= k;
		return res;
	};
	Vector3 Vector3::ComponentProduct(Vector3 const& v) const
	{
		Vector3 res;
		res.x = x * v.x;
		res.y = y * v.y;
		res.z = z * v.z;
		return res;
	};
	double Vector3::ScalarProduct(Vector3 const& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	};
	Vector3 Vector3::CrossProduct(Vector3 const& v) const
	{
		Vector3 res;
		res.x = (y * v.z) - (z * v.y);
		res.y = (z * v.x) - (x * v.z);
		res.z = (x * v.y) - (y * v.x);
		return res;
	};

	std::string Vector3::toString() const
	{
		return("Vector3(x = " + std::to_string(x) + " ; y = " + std::to_string(y) + " ; z = " + std::to_string(z) + ")");
	}
}

