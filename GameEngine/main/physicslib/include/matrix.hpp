#pragma once

#include <valarray>

namespace physicslib
{
	using Matrix3 = Matrix<3>;
	using Matrix4 = Matrix<4>;

	template<int N>
	class Matrix
	{
	public:
		Matrix(double fillNumber = 0.);
		Matrix(const Matrix& anotherMatrix) = default;
		virtual ~Matrix() = default;

		Matrix& operator=(const Matrix& anotherMatrix) = default;

		// Matrix mathematical operations
		Matrix& operator+=(const Matrix& anotherMatrix);
		Matrix operator+(const Matrix& anotherMatrix);
		Matrix& operator-=(const Matrix& anotherMatrix);
		Matrix operator-(const Matrix& anotherMatrix);
		Matrix& operator*=(const Matrix& anotherMatrix);
		Matrix operator*(const Matrix& anotherMatrix);

		// Matrix/scalar operations
		Matrix operator+(const double& scalar);
		Matrix operator-(const double& scalar);
		Matrix operator*(const double& scalar);
		Matrix operator/(const double& scalar);

		// Getters/Setters
		double& operator()(const unsigned int row, const unsigned int column);
		const double& operator()(const unsigned int row, const unsigned int column) const;

		unsigned int getSize() const;

	private:
		std::valarray<double> m_data;
	};
}

#include "matrixImplementation.hpp"