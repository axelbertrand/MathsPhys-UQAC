#pragma once
#include <string>

namespace mathslib
{
	class Vector3
	{
	private :
		double x = 0;
		double y = 0;
		double z = 0;
	public :
		Vector3();
		Vector3(double i, double j, double k);
		Vector3(Vector3 const& v);
		void operator+=(const Vector3& v);
		void operator-=(const Vector3& v);
		Vector3 operator+(const Vector3& v) const;
		Vector3 operator-(const Vector3& v) const;
		Vector3 operator*(double k) const;
		double operator*(const Vector3& v) const;
		Vector3 operator/(double k) const;
		Vector3 operator^(const Vector3& v) const;
		Vector3 VectorAddition(Vector3 const& v) const;
		Vector3 ScalarMultiplication(double const& k) const;
		Vector3 ComponentProduct(Vector3 const& v) const;
		double ScalarProduct(Vector3 const& v )const;
		Vector3 CrossProduct(Vector3 const& v) const;
		std::string toString() const;
		double getX() const { return x; };
		double getY() const { return y; };
		double getZ() const { return z; };
		double getNorm() const;
		double getSquaredNorm() const;
		Vector3 getNormalizedVector() const;
	};
}

static mathslib::Vector3 operator* (double d, mathslib::Vector3 v)
{
	return v * d;
}