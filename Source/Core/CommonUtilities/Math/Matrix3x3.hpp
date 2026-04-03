#pragma once
#include <array>
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Quaternion;

	//template<typename _Ty>
	//class Matrix4x4;

	template<typename T>
	class Matrix3x3
	{
	public:
		// Creates the identity matrix.
		Matrix3x3();
		// Creates a matrix filled with the choosen value.
		Matrix3x3(T aValue);
		// Creates a matrix with the chosen values.
		Matrix3x3(T(&aValues)[3][3]);
		// Copy Constructor.
		Matrix3x3(const Matrix3x3<T>& aMatrix) = default;
		//// Copies the top left 3x3 part of the Matrix4x4.
		//Matrix3x3(const Matrix4x4<T>& aMatrix);
		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		// Creates a transposed copy of the matrix.
		Matrix3x3<T> GetTranspose() const;
		// Creates an Inverse copy of the translation matrix.
		Matrix3x3<T> GetFastInverse() const;

		void Compose(const Quaternion<T>& aRotation, const Vector3<T>& aScale);
		void Compose(const T& aRotation, const Vector2<T>& aPosition, const Vector2<T>& aScale);

		//TODO: Add decompose method. Preferably an affine transform decompose.
		void Decompose(Quaternion<T>& outRotation, Vector3<T>& outScale) const;
		void Decompose(T& outRotation, Vector2<T>& outPosition, Vector2<T>& outScale) const;

		Matrix3x3<T> operator+(const Matrix3x3<T>& aRhs) const;
		Matrix3x3<T> operator-(const Matrix3x3<T>& aRhs) const;
		Matrix3x3<T> operator*(const Matrix3x3<T>& aRhs) const;

		bool operator==(const Matrix3x3<T>& aRhs) const;

		Matrix3x3<T>& operator+=(const Matrix3x3<T>& aRhs);
		Matrix3x3<T>& operator-=(const Matrix3x3<T>& aRhs);
		Matrix3x3<T>& operator*=(const Matrix3x3<T>& aRhs);

		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(const T aAngleInRadians);
		//static Vector3<T> operator*(const Vector3<T>& aLhs, const Matrix3x3<T>& aRhs);

	private:
		union
		{
			T myData[9];
			T myElements[3][3];
			Vector3<T> myRows[3];
		};
	};

	using Matrix3x3f = Matrix3x3<float>;
	using Matrix3x3d = Matrix3x3<double>;
	using Matrix3x3i = Matrix3x3<int>;
	using Matrix3x3ui = Matrix3x3<unsigned int>;

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3()
	{
		memset(myElements, 0, sizeof(myElements));
		this->myElements[0][0] = 1;
		this->myElements[1][1] = 1;
		this->myElements[2][2] = 1;
	}
	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(T aValue)
	{
		for (size_t i = 0; i < sizeof(this->myElements) / sizeof(this->myElements[0]); i++)
		{
			for (size_t j = 0; j < sizeof(this->myElements[0]) / sizeof(T); j++)
			{
				this->myElements[i][j] = aValue;
			}
		}
	}
	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(T(&aValues)[3][3])
	{
		for (size_t i = 0; i < 3; i++)
		{
			std::copy(std::begin(aValues[i]), std::end(aValues[i]), myElements[i]);
		}
	}
	template<typename T>
	inline T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		return this->myElements[aRow - 1][aColumn - 1];
	}
	template<typename T>
	inline const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		return this->myElements[aRow - 1][aColumn - 1];
	}
	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetTranspose() const
	{
		Matrix3x3<T> matrix(static_cast<T>(0));
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				matrix.myElements[i][j] = this->myElements[j][i];
			}
		}
		return matrix;
	}
	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetFastInverse() const
	{
		Matrix3x3<T> matrix(0);
		matrix.myElements[0][0] = this->myElements[0][0];
		matrix.myElements[1][1] = this->myElements[1][1];

		matrix.myElements[0][1] = this->myElements[1][0];
		matrix.myElements[1][0] = this->myElements[0][1];

		matrix.myElements[2][0] = -this->myElements[2][0] * this->myElements[0][0] + this->myElements[2][1] * this->myElements[1][0];
		matrix.myElements[2][0] = this->myElements[2][0] * this->myElements[1][1] - this->myElements[2][1] * this->myElements[0][0];

		matrix.myElements[2][2] = 1;
		return matrix;
	}

	template<typename T>
	inline void Matrix3x3<T>::Compose(const Quaternion<T>& aRotation, const Vector3<T>& aScale)
	{
		const T x = aRotation.x, y = aRotation.y, z = aRotation.z, w = aRotation.w;
		const T x2 = x + x, y2 = y + y, z2 = z + z;
		const T xx = x * x2, xy = x * y2, xz = x * z2;
		const T yy = y * y2, yz = y * z2, zz = z * z2;
		const T wx = w * x2, wy = w * y2, wz = w * z2;

		const T sx = aScale.x, sy = aScale.y, sz = aScale.z;

		myElements[0][0] = (static_cast<T>(1) - (yy + zz)) * sx;
		myElements[0][1] = (xy + wz) * sx;
		myElements[0][2] = (xz - wy) * sx;

		myElements[1][0] = (xy - wz) * sy;
		myElements[1][1] = (static_cast<T>(1) - (xx + zz)) * sy;
		myElements[1][2] = (yz + wx) * sy;

		myElements[2][0] = (xz + wy) * sz;
		myElements[2][1] = (yz - wx) * sz;
		myElements[2][2] = (static_cast<T>(1) - (xx + yy)) * sz;
	}

	template<typename T>
	inline void Matrix3x3<T>::Compose(const T& aRotation, const Vector2<T>& aPosition, const Vector2<T>& aScale)
	{
		const T c = std::cos(aRotation);
		const T s = std::sin(aRotation);
		myElements[0][0] = c * aScale.x;
		myElements[0][1] = -s * aScale.x;
		myElements[1][0] = s * aScale.y;
		myElements[1][1] = c * aScale.y;
		myElements[2][0] = aPosition.x;
		myElements[2][1] = aPosition.y;

		myElements[0][2] = 0.f;
		myElements[1][2] = 0.f;
		myElements[2][2] = 1.f;
	}

	template<typename T>
	inline void Matrix3x3<T>::Decompose(Quaternion<T>& outRotation, Vector3<T>& outScale) const
	{
		outScale.x = std::sqrt(myElements[0][0] * myElements[0][0] + myElements[0][1] * myElements[0][1] + myElements[0][2] * myElements[0][2]);
		outScale.y = std::sqrt(myElements[1][0] * myElements[1][0] + myElements[1][1] * myElements[1][1] + myElements[1][2] * myElements[1][2]);
		outScale.z = std::sqrt(myElements[2][0] * myElements[2][0] + myElements[2][1] * myElements[2][1] + myElements[2][2] * myElements[2][2]);

		const T m11 = myElements[0][0] / outScale.x;
		const T m12 = myElements[0][1] / outScale.x;
		const T m13 = myElements[0][2] / outScale.x;

		const T m21 = myElements[1][0] / outScale.y;
		const T m22 = myElements[1][1] / outScale.y;
		const T m23 = myElements[1][2] / outScale.y;

		const T m31 = myElements[2][0] / outScale.z;
		const T m32 = myElements[2][1] / outScale.z;
		const T m33 = myElements[2][2] / outScale.z;

		const T trace = m11 + m22 + m33;

		if (trace > static_cast<T>(0.f))
		{

			const T s = static_cast<T>(0.5f) / std::sqrt(trace + static_cast<T>(1.0f));

			outRotation.w = 0.25f / s;
			outRotation.x = (m32 - m23) * s;
			outRotation.y = (m13 - m31) * s;
			outRotation.z = (m21 - m12) * s;

		}
		else if (m11 > m22 && m11 > m33)
		{

			const T s = static_cast<T>(2.0f) * std::sqrt(static_cast<T>(1.0f) + m11 - m22 - m33);

			outRotation.w = (m32 - m23) / s;
			outRotation.x = static_cast<T>(0.25f) * s;
			outRotation.y = (m12 + m21) / s;
			outRotation.z = (m13 + m31) / s;

		}
		else if (m22 > m33)
		{

			const T s = static_cast<T>(2.0f) * std::sqrt(static_cast<T>(1.0f) + m22 - m11 - m33);

			outRotation.w = (m13 - m31) / s;
			outRotation.x = (m12 + m21) / s;
			outRotation.y = static_cast<T>(0.25f) * s;
			outRotation.z = (m23 + m32) / s;

		}
		else
		{

			const T s = static_cast<T>(2.0f) * std::sqrt(static_cast<T>(1.0f) + m33 - m11 - m22);

			outRotation.w = (m21 - m12) / s;
			outRotation.x = (m13 + m31) / s;
			outRotation.y = (m23 + m32) / s;
			outRotation.z = static_cast<T>(0.25f) * s;

		}
	}

	template<typename T>
	inline void Matrix3x3<T>::Decompose(T& outRotation, Vector2<T>& outPosition, Vector2<T>& outScale) const
	{
		outScale.x = std::sqrt(myElements[0][0] * myElements[0][0] + myElements[0][1] * myElements[0][1]);
		outScale.y = std::sqrt(myElements[1][0] * myElements[1][0] + myElements[1][1] * myElements[1][1]);

		outRotation = std::acos(myElements[0][0] / outScale.x);

		outPosition.x = myElements[2][0];
		outPosition.y = myElements[2][1];
	}

	//template<typename T>
	//inline void Matrix3x3<T>::Compose(const Quaternion<T>& aRotation, const Vector3<T>& aScale, const Vector3<T>& aTranslation)
	//{
	//	const T x = aRotation.x, y = aRotation.y, z = aRotation.z, w = aRotation.w;
	//	const T x2 = x + x, y2 = y + y, z2 = z + z;
	//	const T xx = x * x2, xy = x * y2, xz = x * z2;
	//	const T yy = y * y2, yz = y * z2, zz = z * z2;
	//	const T wx = w * x2, wy = w * y2, wz = w * z2;

	//	const T sx = aScale.x, sy = aScale.y, sz = aScale.z;

	//	myElements[0][0] = (1 - (yy + zz)) * sx;
	//	myElements[0][1] = (xy + wz) * sx;
	//	myElements[0][2] = (xz - wy) * sx;

	//	myElements[1][0] = (xy - wz) * sy;
	//	myElements[1][1] = (1 - (xx + zz)) * sy;
	//	myElements[1][2] = (yz + wx) * sy;

	//	myElements[2][0] = (xz + wy) * sz;
	//	myElements[2][1] = (yz - wx) * sz;
	//	myElements[2][2] = (1 - (xx + yy)) * sz;

	//	myElements[3][0] = aTranslation.x;
	//	myElements[3][0] = aTranslation.y;
	//	myElements[3][0] = aTranslation.z;
	//}

	//template<typename T>
	//inline void Matrix3x3<T>::Decompose(Quaternion<T>& outRotation, Vector3<T>& outScale, Vector3<T>& outTranslation)
	//{
	//	outTranslation.x = myElements[3][0];
	//	outTranslation.y = myElements[3][1];
	//	outTranslation.z = myElements[3][2];

	//	outScale.x = ::sqrt(myElements[0][0] * myElements[0][0] + myElements[0][1] * myElements[0][1] + myElements[0][2] * myElements[0][2]);
	//	outScale.y = ::sqrt(myElements[1][0] * myElements[1][0] + myElements[1][1] * myElements[1][1] + myElements[1][2] * myElements[1][2]);
	//	outScale.z = ::sqrt(myElements[2][0] * myElements[2][0] + myElements[2][1] * myElements[2][1] + myElements[2][2] * myElements[2][2]);


	//}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		Matrix3x3<T> matrix;
		matrix.myElements[1][1] = cos(aAngleInRadians);
		matrix.myElements[1][2] = sin(aAngleInRadians);
		matrix.myElements[2][1] = -sin(aAngleInRadians);
		matrix.myElements[2][2] = cos(aAngleInRadians);
		return matrix;
	}
	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		Matrix3x3<T> matrix;
		matrix.myElements[0][0] = cos(aAngleInRadians);
		matrix.myElements[0][2] = -sin(aAngleInRadians);
		matrix.myElements[2][0] = sin(aAngleInRadians);
		matrix.myElements[2][2] = cos(aAngleInRadians);
		return matrix;
	}
	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		Matrix3x3<T> matrix;
		matrix.myElements[0][0] = cos(aAngleInRadians);
		matrix.myElements[0][1] = sin(aAngleInRadians);
		matrix.myElements[1][0] = -sin(aAngleInRadians);
		matrix.myElements[1][1] = cos(aAngleInRadians);
		return matrix;
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::operator+(const Matrix3x3<T>& aRhs) const
	{
		return Matrix3x3<T>(*this) += aRhs;
	}
	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::operator-(const Matrix3x3<T>& aRhs) const
	{
		return Matrix3x3<T>(*this) -= aRhs;
	}
	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::operator*(const Matrix3x3<T>& aRhs) const
	{
		return Matrix3x3<T>(*this) *= aRhs;
	}

	template<typename T>
	inline bool Matrix3x3<T>::operator==(const Matrix3x3<T>& aRhs) const
	{
		return !memcmp(this->myElements, aRhs.myElements, sizeof(this->myElements));
	}
	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix3x3<T>& aRhs)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				this->myElements[i][j] += aRhs.myElements[i][j];
			}
		}
		return *this;
	}
	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator-=(const Matrix3x3<T>& aRhs)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				this->myElements[i][j] -= aRhs.myElements[i][j];
			}
		}
		return *this;
	}
	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator*=(const Matrix3x3<T>& aRhs)
	{
		Matrix3x3<T> matrix(*this);
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				this->myElements[i][j] = static_cast<T>(0);
				for (size_t k = 0; k < 3; k++)
				{
					this->myElements[i][j] += matrix.myElements[i][k] * aRhs.myElements[k][j];
				}
			}
		}
		return *this;
	}


	template<typename T>
	Vector3<T> operator*(const Vector3<T>& aLhs, const Matrix3x3<T>& aRhs)
	{
		Vector3<T> vec(0, 0, 0);
		vec.x += aLhs.x * aRhs(1, 1) + aLhs.y * aRhs(2, 1) + aLhs.z * aRhs(3, 1);
		vec.y += aLhs.x * aRhs(1, 2) + aLhs.y * aRhs(2, 2) + aLhs.z * aRhs(3, 2);
		vec.z += aLhs.x * aRhs(1, 3) + aLhs.y * aRhs(2, 3) + aLhs.z * aRhs(3, 3);
		return vec;
	}
}