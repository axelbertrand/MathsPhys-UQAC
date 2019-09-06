#include <iostream>
#include <string>
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

	Vector3 operator+(Vector3 const& u, Vector3 const& v)
	{
		Vector3 res;
		res = u.VectorAddition(v);
		return res;
	};

	Vector3 operator*(double k, Vector3 const& v)
	{
		Vector3 res;
		res = v.ScalarMultiplication(k);
		return res;
	};

	float operator*(Vector3 const& u, Vector3 const& v)
	{
		return u.ScalarProduct(v);
	}

	Vector3 operator^(Vector3 const& u, Vector3 const& v)
	{
		return u.CrossProduct(v);
	}
}
