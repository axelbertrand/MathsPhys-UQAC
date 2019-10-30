namespace physicslib
{
	template<int N>
	Matrix<N>::Matrix(double fillNumber)
		: m_data(fillNumber, N * N)
	{
	}

	// ------------------------------
	// Matrix mathematical operations
	// ------------------------------
	template<int N>
	Matrix<N>& Matrix<N>::operator+=(const Matrix<N>& anotherMatrix)
	{
		m_data += anotherMatrix.m_data;

		return *this;
	}

	template<int N>
	Matrix<N> Matrix<N>::operator+(const Matrix<N>& anotherMatrix)
	{
		Matrix newMatrix(*this);
		newMatrix *= anotherMatrix;

		return newMatrix;
	}

	template<int N>
	Matrix<N>& Matrix<N>::operator-=(const Matrix<N>& anotherMatrix)
	{
		m_data -= anotherMatrix.m_data;

		return *this;
	}

	template<int N>
	Matrix<N> Matrix<N>::operator-(const Matrix<N>& anotherMatrix)
	{
		Matrix newMatrix(*this);
		newMatrix *= anotherMatrix;

		return newMatrix;
	}

	template<int N>
	Matrix<N>& Matrix<N>::operator*=(const Matrix<N>& anotherMatrix)
	{
		Matrix<N> result;

		for (unsigned int i = 0; i < N; ++i)
		{
			for (unsigned int j = 0; j < N; ++j)
			{
				// [ a(i, 1), a(i, 2), a(i, 3), ... ]
				std::valarray<double> dataSelection1(m_data[std::slice(N * i, N, 1)]);

				// [ b(1, j), b(2, j), b(3, j), ... ]
				std::valarray<double> dataSelection2(anotherMatrix.m_data[std::slice(j, N, N)]);

				// c(i, j) = a(i, 1) * b(1, j) + a(i, 2) * b(2, j) + a(i, 3) * b(3, j) + ...
				result(i, j) = (dataSelection1 * dataSelection2).sum();
			}
		}

		*this = result;

		return *this;
	}

	template<int N>
	Matrix<N> Matrix<N>::operator*(const Matrix<N>& anotherMatrix)
	{
		Matrix<N> newMatrix(*this);
		newMatrix *= anotherMatrix;

		return newMatrix;
	}

	// ------------------------
	// Matrix/scalar operations
	// ------------------------
	template<int N>
	Matrix<N> Matrix<N>::operator+(const double& scalar)
	{
		Matrix<N> newMatrix(*this);
		newMatrix.m_data += scalar;

		return newMatrix;
	}

	template<int N>
	Matrix<N> Matrix<N>::operator-(const double& scalar)
	{
		Matrix<N> newMatrix(*this);
		newMatrix.m_data -= scalar;

		return newMatrix;
	}

	template<int N>
	Matrix<N> Matrix<N>::operator*(const double& scalar)
	{
		Matrix<N> newMatrix(*this);
		newMatrix.m_data *= scalar;

		return newMatrix;
	}

	template<int N>
	Matrix<N> Matrix<N>::operator/(const double& scalar)
	{
		Matrix<N> newMatrix(*this);
		newMatrix.m_data /= scalar;

		return newMatrix;
	}

	// ---------------
	// Getters/Setters
	// ---------------
	template<int N>
	double& Matrix<N>::operator()(const unsigned int row, const unsigned int column)
	{
		return m_data[N * row + column];
	}

	template<int N>
	const double& Matrix<N>::operator()(const unsigned int row, const unsigned int column) const
	{
		return m_data[N * row + column];
	}

	template<int N>
	unsigned int Matrix<N>::getSize() const
	{
		return N;
	}
}