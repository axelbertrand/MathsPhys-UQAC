#pragma once

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
		Vector3 VectorAddition(Vector3 const& v) const;
		Vector3 ScalarMultiplication(double const& k) const;
		Vector3 ComponentProduct(Vector3 const& v) const;
		double ScalarProduct(Vector3 const& v )const;
		Vector3 CrossProduct(Vector3 const& v) const;
	};
}