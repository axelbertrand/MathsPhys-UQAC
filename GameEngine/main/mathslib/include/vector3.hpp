#pragma once

namespace mathslib
{
	class Vector3
	{
	private :
		float x = 0;
		float y = 0;
		float z = 0;
	public :
		Vector3();
		Vector3(float i, float j, float k);
		Vector3(Vector3 const& v);
		void operator+=(const Vector3& v);
		void operator-=(const Vector3& v);
		Vector3 VectorAddition(Vector3 const& v) const;
		Vector3 ScalarMultiplication(float const& k) const;
		Vector3 ComponentProduct(Vector3 const& v) const;
		float ScalarProduct(Vector3 const& v )const;
		Vector3 CrossProduct(Vector3 const& v) const;
	};
}