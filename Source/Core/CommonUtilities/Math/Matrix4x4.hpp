#pragma once
#include <array>
#include "Vector4.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace CommonUtilities
{
	//template<typename T>
	//class Matrix3x3;

	template<typename _Ty>
	class Quaternion;

	template<typename T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		constexpr Matrix4x4();
		// Creates a matrix filled with the choosen value.
		constexpr Matrix4x4(T aValue);
		// Creates a matrix with the chosen values.
		constexpr Matrix4x4(const T(&aValues)[4][4]);
		constexpr Matrix4x4(const T(&aValues)[16]);
		constexpr Matrix4x4(const std::array<std::array<T, 4>, 4>& aValues);
		constexpr Matrix4x4(const std::array<T, 16>& aValues);
		// Copy Constructor.
		Matrix4x4(const Matrix4x4<T>& aMatrix) = default;
		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		const Vector4<T>& Row(const int aRow) const;
		Vector4<T>& Row(const int aRow);
		Vector4<T> Column(const int aColumn) const;
		T* Data();
		const T* Data() const;
		// Creates a transposed copy of the matrix.
		Matrix4x4<T> GetTranspose() const;
		// Gets the determinant of the matrix.
		T GetDeterminant() const;
		// Creates an Inverse copy of the translation matrix.
		Matrix4x4<T> GetFastInverse() const;
		Matrix4x4<T> GetInverse() const;

		void Compose(const Vector3<T>& aTranslation, const Quaternion<T>& aRotation, const Vector3<T>& aScale);
		void Decompose(Vector3<T>& outTranslation, Quaternion<T>& outRotation, Vector3<T>& outScale) const;

		//Vector3<T> GetEulerAnglesZYX() const;

		Matrix4x4<T> operator+(const Matrix4x4<T>& aRhs) const;
		Matrix4x4<T> operator-(const Matrix4x4<T>& aRhs) const;
		Matrix4x4<T> operator*(const Matrix4x4<T>& aRhs) const;
		bool operator==(const Matrix4x4<T>& aRhs) const;
		Matrix4x4<T> operator-() const;

		Matrix4x4<T>& operator+=(const Matrix4x4<T>& aRhs);
		Matrix4x4<T>& operator-=(const Matrix4x4<T>& aRhs);
		Matrix4x4<T>& operator*=(const Matrix4x4<T>& aRhs);

		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(const T aAngleInRadians);

		static Matrix4x4<T> CreateScale(const Vector3<T>& aScale);
		static Matrix4x4<T> CreateRotation(const Vector3<T>& aEulerAngles);
		static Matrix4x4<T> CreateRotation(const Quaternion<T>& aQuaternion);
		static Matrix4x4<T> CreateTranslation(const Vector3<T>& aTranslation);
		static Matrix4x4<T> CreateTransform(const Vector3<T>& aTranslation, const Vector3<T>& aEulerAngles);

	private:
		union
		{
			T myData[16];
			T myElements[4][4];
			Vector4<T> myRows[4];
		};
	};

	using Matrix4x4f = Matrix4x4<float>;
	using Matrix4x4d = Matrix4x4<double>;
	using Matrix4x4i = Matrix4x4<int>;
	using Matrix4x4ui = Matrix4x4<unsigned int>;

	template<typename T>
	constexpr inline Matrix4x4<T>::Matrix4x4()
	{
		memset(myElements, 0, sizeof(T) * 16);
		this->myElements[0][0] = 1;
		this->myElements[1][1] = 1;
		this->myElements[2][2] = 1;
		this->myElements[3][3] = 1;
	}
	template<typename T>
	constexpr inline Matrix4x4<T>::Matrix4x4(T aValue)
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
	constexpr inline Matrix4x4<T>::Matrix4x4(const T(&aValues)[4][4])
	{
		for (size_t i = 0; i < 4; i++)
		{
			std::copy(std::begin(aValues[i]), std::end(aValues[i]), myElements[i]);
		}
	}
	template<typename T>
	constexpr inline Matrix4x4<T>::Matrix4x4(const T(&aValues)[16])
	{
		std::copy(std::begin(aValues), std::end(aValues), myElements);
	}
	template<typename T>
	constexpr inline Matrix4x4<T>::Matrix4x4(const std::array<std::array<T, 4>, 4>& aValues)
	{
		for (size_t i = 0; i < 4; i++)
		{
			std::copy(std::begin(aValues[i]), std::end(aValues[i]), myElements[i]);
		}
	}
	template<typename T>
	constexpr inline Matrix4x4<T>::Matrix4x4(const std::array<T, 16>& aValues)
	{
		std::copy(std::begin(aValues), std::end(aValues), myData);
	}
	template<typename T>
	inline T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		return this->myElements[aRow - 1][aColumn - 1];
	}
	template<typename T>
	inline const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		return this->myElements[aRow - 1][aColumn - 1];
	}
	template<typename T>
	inline const Vector4<T>& Matrix4x4<T>::Row(const int aRow) const
	{
		return myRows[aRow - 1];
	}
	template<typename T>
	inline Vector4<T>& Matrix4x4<T>::Row(const int aRow)
	{
		return myRows[aRow - 1];
	}
	template<typename T>
	inline Vector4<T> Matrix4x4<T>::Column(const int aColumn) const
	{
		return Vector4<T>(myElements[0][aColumn - 1], myElements[1][aColumn - 1], myElements[2][aColumn - 1], myElements[3][aColumn - 1]);
	}
	template<typename T>
	inline T* Matrix4x4<T>::Data()
	{
		return myData;
	}
	template<typename T>
	inline const T* Matrix4x4<T>::Data() const
	{
		return myData;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetTranspose() const
	{
		Matrix4x4<T> matrix(static_cast<T>(0));
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				matrix.myElements[i][j] = this->myElements[j][i];
			}
		}
		return matrix;
	}
	template<typename T>
	inline T Matrix4x4<T>::GetDeterminant() const
	{
		return myElements[0][3] * myElements[1][2] * myElements[2][1] * myElements[3][0] - myElements[0][2] * myElements[1][3] * myElements[2][1] * myElements[3][0] -
			myElements[0][3] * myElements[1][1] * myElements[2][2] * myElements[3][0] + myElements[0][1] * myElements[1][3] * myElements[2][2] * myElements[3][0] +
			myElements[0][2] * myElements[1][1] * myElements[2][3] * myElements[3][0] - myElements[0][1] * myElements[1][2] * myElements[2][3] * myElements[3][0] -
			myElements[0][3] * myElements[1][2] * myElements[2][0] * myElements[3][1] + myElements[0][2] * myElements[1][3] * myElements[2][0] * myElements[3][1] +
			myElements[0][3] * myElements[1][0] * myElements[2][2] * myElements[3][1] - myElements[0][0] * myElements[1][3] * myElements[2][2] * myElements[3][1] -
			myElements[0][2] * myElements[1][0] * myElements[2][3] * myElements[3][1] + myElements[0][0] * myElements[1][2] * myElements[2][3] * myElements[3][1] +
			myElements[0][3] * myElements[1][1] * myElements[2][0] * myElements[3][2] - myElements[0][1] * myElements[1][3] * myElements[2][0] * myElements[3][2] -
			myElements[0][3] * myElements[1][0] * myElements[2][1] * myElements[3][2] + myElements[0][0] * myElements[1][3] * myElements[2][1] * myElements[3][2] +
			myElements[0][1] * myElements[1][0] * myElements[2][3] * myElements[3][2] - myElements[0][0] * myElements[1][1] * myElements[2][3] * myElements[3][2] -
			myElements[0][2] * myElements[1][1] * myElements[2][0] * myElements[3][3] + myElements[0][1] * myElements[1][2] * myElements[2][0] * myElements[3][3] +
			myElements[0][2] * myElements[1][0] * myElements[2][1] * myElements[3][3] - myElements[0][0] * myElements[1][2] * myElements[2][1] * myElements[3][3] -
			myElements[0][1] * myElements[1][0] * myElements[2][2] * myElements[3][3] + myElements[0][0] * myElements[1][1] * myElements[2][2] * myElements[3][3];
	}
	template <typename T>
	Matrix4x4<T> Matrix4x4<T>::GetFastInverse() const
	{
		Matrix4x4 rotationInversed = GetTranspose();
		Matrix4x4 translation;

		// Copy over an inverse of the translation from the rotation inverse
		translation.myElements[3][0] = -rotationInversed.myElements[0][3];
		//translation[12] = -rotationInversed[3];
		translation.myElements[3][1] = -rotationInversed.myElements[1][3];
		//translation[13] = -rotationInversed[7];
		translation.myElements[3][2] = -rotationInversed.myElements[2][3];
		//translation[14] = -rotationInversed[11];

		// Get rid of the translation in the rotation inverse.
		rotationInversed.myElements[0][3] = 0;
		//rotationInversed[3] = 0;
		rotationInversed.myElements[1][3] = 0;
		//rotationInversed[7] = 0;
		rotationInversed.myElements[2][3] = 0;
		//rotationInversed[11] = 0;

		return translation * rotationInversed;
	}
	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::GetInverse() const
	{
		T A2323 = myElements[2][2] * myElements[3][3] - myElements[2][3] * myElements[3][2];
		T A1323 = myElements[2][1] * myElements[3][3] - myElements[2][3] * myElements[3][1];
		T A1223 = myElements[2][1] * myElements[3][2] - myElements[2][2] * myElements[3][1];
		T A0323 = myElements[2][0] * myElements[3][3] - myElements[2][3] * myElements[3][0];
		T A0223 = myElements[2][0] * myElements[3][2] - myElements[2][2] * myElements[3][0];
		T A0123 = myElements[2][0] * myElements[3][1] - myElements[2][1] * myElements[3][0];
		T A2313 = myElements[1][2] * myElements[3][3] - myElements[1][3] * myElements[3][2];
		T A1313 = myElements[1][1] * myElements[3][3] - myElements[1][3] * myElements[3][1];
		T A1213 = myElements[1][1] * myElements[3][2] - myElements[1][2] * myElements[3][1];
		T A2312 = myElements[1][2] * myElements[2][3] - myElements[1][3] * myElements[2][2];
		T A1312 = myElements[1][1] * myElements[2][3] - myElements[1][3] * myElements[2][1];
		T A1212 = myElements[1][1] * myElements[2][2] - myElements[1][2] * myElements[2][1];
		T A0313 = myElements[1][0] * myElements[3][3] - myElements[1][3] * myElements[3][0];
		T A0213 = myElements[1][0] * myElements[3][2] - myElements[1][2] * myElements[3][0];
		T A0312 = myElements[1][0] * myElements[2][3] - myElements[1][3] * myElements[2][0];
		T A0212 = myElements[1][0] * myElements[2][2] - myElements[1][2] * myElements[2][0];
		T A0113 = myElements[1][0] * myElements[3][1] - myElements[1][1] * myElements[3][0];
		T A0112 = myElements[1][0] * myElements[2][1] - myElements[1][1] * myElements[2][0];

		T det = myElements[0][0] * (myElements[1][1] * A2323 - myElements[1][2] * A1323 + myElements[1][3] * A1223)
			- myElements[0][1] * (myElements[1][0] * A2323 - myElements[1][2] * A0323 + myElements[1][3] * A0223)
			+ myElements[0][2] * (myElements[1][0] * A1323 - myElements[1][1] * A0323 + myElements[1][3] * A0123)
			- myElements[0][3] * (myElements[1][0] * A1223 - myElements[1][1] * A0223 + myElements[1][2] * A0123);
		det = T(1) / det;

		Matrix4x4<T> im;

		im.myElements[0][0] = det * (myElements[1][1] * A2323 - myElements[1][2] * A1323 + myElements[1][3] * A1223);
		im.myElements[0][1] = det * -(myElements[0][1] * A2323 - myElements[0][2] * A1323 + myElements[0][3] * A1223);
		im.myElements[0][2] = det * (myElements[0][1] * A2313 - myElements[0][2] * A1313 + myElements[0][3] * A1213);
		im.myElements[0][3] = det * -(myElements[0][1] * A2312 - myElements[0][2] * A1312 + myElements[0][3] * A1212);
		im.myElements[1][0] = det * -(myElements[1][0] * A2323 - myElements[1][2] * A0323 + myElements[1][3] * A0223);
		im.myElements[1][1] = det * (myElements[0][0] * A2323 - myElements[0][2] * A0323 + myElements[0][3] * A0223);
		im.myElements[1][2] = det * -(myElements[0][0] * A2313 - myElements[0][2] * A0313 + myElements[0][3] * A0213);
		im.myElements[1][3] = det * (myElements[0][0] * A2312 - myElements[0][2] * A0312 + myElements[0][3] * A0212);
		im.myElements[2][0] = det * (myElements[1][0] * A1323 - myElements[1][1] * A0323 + myElements[1][3] * A0123);
		im.myElements[2][1] = det * -(myElements[0][0] * A1323 - myElements[0][1] * A0323 + myElements[0][3] * A0123);
		im.myElements[2][2] = det * (myElements[0][0] * A1313 - myElements[0][1] * A0313 + myElements[0][3] * A0113);
		im.myElements[2][3] = det * -(myElements[0][0] * A1312 - myElements[0][1] * A0312 + myElements[0][3] * A0112);
		im.myElements[3][0] = det * -(myElements[1][0] * A1223 - myElements[1][1] * A0223 + myElements[1][2] * A0123);
		im.myElements[3][1] = det * (myElements[0][0] * A1223 - myElements[0][1] * A0223 + myElements[0][2] * A0123);
		im.myElements[3][2] = det * -(myElements[0][0] * A1213 - myElements[0][1] * A0213 + myElements[0][2] * A0113);
		im.myElements[3][3] = det * (myElements[0][0] * A1212 - myElements[0][1] * A0212 + myElements[0][2] * A0112);

		return im;
	}
	template<typename T>
	inline void Matrix4x4<T>::Compose(const Vector3<T>& aTranslation, const Quaternion<T>& aRotation, const Vector3<T>& aScale)
	{
		const T x = aRotation.x, y = aRotation.y, z = aRotation.z, w = aRotation.w;
		const T x2 = x + x, y2 = y + y, z2 = z + z;
		const T xx = x * x2, xy = x * y2, xz = x * z2;
		const T yy = y * y2, yz = y * z2, zz = z * z2;
		const T wx = w * x2, wy = w * y2, wz = w * z2;

		const T sx = aScale.x, sy = aScale.y, sz = aScale.z;

		myData[0] = (static_cast<T>(1) - (yy + zz)) * sx;
		myData[1] = (xy + wz) * sx;
		myData[2] = (xz - wy) * sx;
		myData[3] = static_cast<T>(0);

		myData[4] = (xy - wz) * sy;
		myData[5] = (static_cast<T>(1) - (xx + zz)) * sy;
		myData[6] = (yz + wx) * sy;
		myData[7] = static_cast<T>(0);

		myData[8] = (xz + wy) * sz;
		myData[9] = (yz - wx) * sz;
		myData[10] = (static_cast<T>(1) - (xx + yy)) * sz;
		myData[11] = static_cast<T>(0);

		myData[12] = aTranslation.x;
		myData[13] = aTranslation.y;
		myData[14] = aTranslation.z;
		myData[15] = static_cast<T>(1);
	}

	template<typename T>
	inline void Matrix4x4<T>::Decompose(Vector3<T>& outTranslation, Quaternion<T>& outRotation, Vector3<T>& outScale) const
	{
		T sx = std::sqrt(myData[0] * myData[0] + myData[1] * myData[1] + myData[2] * myData[2]);
		const T sy = std::sqrt(myData[4] * myData[4] + myData[5] * myData[5] + myData[6] * myData[6]);
		const T sz = std::sqrt(myData[8] * myData[8] + myData[9] * myData[9] + myData[10] * myData[10]);

		// if determine is negative, we need to invert one scale
		const T det = GetDeterminant();
		if (det < static_cast<T>(0)) sx = -sx;

		outTranslation.x = myData[12];
		outTranslation.y = myData[13];
		outTranslation.z = myData[14];

		// scale the rotation part
		Matrix4x4<T> _m1 = *this;

		const T invSX = 1 / sx;
		const T invSY = 1 / sy;
		const T invSZ = 1 / sz;

		_m1.myData[0] *= invSX;
		_m1.myData[1] *= invSX;
		_m1.myData[2] *= invSX;

		_m1.myData[4] *= invSY;
		_m1.myData[5] *= invSY;
		_m1.myData[6] *= invSY;

		_m1.myData[8] *= invSZ;
		_m1.myData[9] *= invSZ;
		_m1.myData[10] *= invSZ;

		// Setting The Rotation

		const T m11 = _m1.myData[0], m12 = _m1.myData[4], m13 = _m1.myData[8];
		const T m21 = _m1.myData[1], m22 = _m1.myData[5], m23 = _m1.myData[9];
		const T m31 = _m1.myData[2], m32 = _m1.myData[6], m33 = _m1.myData[10];

		const T trace = m11 + m22 + m33;

		if (trace > 0)
		{
			const T s = static_cast<T>(1) / (static_cast<T>(2) * std::sqrt(trace + static_cast<T>(1)));

			outRotation.w = static_cast<T>(1) / (s * static_cast<T>(4));
			outRotation.x = (m32 - m23) * s;
			outRotation.y = (m13 - m31) * s;
			outRotation.z = (m21 - m12) * s;
		}
		else if (m11 > m22 && m11 > m33)
		{

			const T s = static_cast<T>(2) * std::sqrt(static_cast<T>(1) + m11 - m22 - m33);

			outRotation.w = (m32 - m23) / s;
			outRotation.x = s / static_cast<T>(4);
			outRotation.y = (m12 + m21) / s;
			outRotation.z = (m13 + m31) / s;
		}
		else if (m22 > m33)
		{

			const T s = static_cast<T>(2) * std::sqrt(static_cast<T>(1) + m22 - m11 - m33);

			outRotation.w = (m13 - m31) / s;
			outRotation.x = (m12 + m21) / s;
			outRotation.y = s / static_cast<T>(4);
			outRotation.z = (m23 + m32) / s;
		}
		else
		{

			const T s = static_cast<T>(2) * std::sqrt(static_cast<T>(1) + m33 - m11 - m22);

			outRotation.w = (m21 - m12) / s;
			outRotation.x = (m13 + m31) / s;
			outRotation.y = (m23 + m32) / s;
			outRotation.z = s / static_cast<T>(4);
		}

		outScale.x = sx;
		outScale.y = sy;
		outScale.z = sz;
	}

	//template<typename T>
	//Vector3<T> Matrix4x4<T>::GetEulerAnglesZYX() const
	//{
	//	const double EPS = 1.0e-6;
	//	Vector3<T> angles;

	//	angles.y = asin(myElements[0][2]);                      // Unique angle in [-pi/2,pi/2]

	//	if (abs(abs(myElements[0][2]) - 1.0) < EPS)				// Yuk! Gimbal lock. Infinite choice of X and Z
	//	{
	//		angles.x = atan2(myElements[2][1], myElements[1][1]);          // One choice amongst many
	//		angles.z = 0.0;
	//	}
	//	else                                       // Unique solutions in (-pi,pi]
	//	{
	//		angles.x = atan2(-myElements[1][2], myElements[2][2]);         // atan2 gives correct quadrant and unique solutions
	//		angles.z = atan2(-myElements[0][1], myElements[0][0]);
	//	}

	//	return angles;
	//}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		Matrix4x4<T> matrix;
		matrix.myElements[1][1] = cos(aAngleInRadians);
		matrix.myElements[1][2] = sin(aAngleInRadians);
		matrix.myElements[2][1] = -sin(aAngleInRadians);
		matrix.myElements[2][2] = cos(aAngleInRadians);
		return matrix;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		Matrix4x4<T> matrix;
		matrix.myElements[0][0] = cos(aAngleInRadians);
		matrix.myElements[0][2] = -sin(aAngleInRadians);
		matrix.myElements[2][0] = sin(aAngleInRadians);
		matrix.myElements[2][2] = cos(aAngleInRadians);
		return matrix;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		Matrix4x4<T> matrix;
		matrix.myElements[0][0] = cos(aAngleInRadians);
		matrix.myElements[0][1] = sin(aAngleInRadians);
		matrix.myElements[1][0] = -sin(aAngleInRadians);
		matrix.myElements[1][1] = cos(aAngleInRadians);
		return matrix;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateScale(const Vector3<T>& aScales)
	{
		Matrix4x4 matrix;
		matrix(1, 1) = aScales.x;
		matrix(2, 2) = aScales.y;
		matrix(3, 3) = aScales.z;
		return matrix;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotation(const Vector3<T>& aEulerAngles)
	{
		Matrix4x4<T> matrix = CreateRotationAroundX(aEulerAngles.z);

		matrix *= CreateRotationAroundY(aEulerAngles.y);
		matrix *= CreateRotationAroundZ(aEulerAngles.x);

		return matrix;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotation(const Quaternion<T>& q)
	{
		Matrix4x4<T> m(0);

		m(1, 1) = static_cast<T>(2) * (q.w * q.w + q.x * q.x) - static_cast<T>(1);
		m(1, 2) = static_cast<T>(2) * (q.x * q.y - q.w * q.z);
		m(1, 3) = static_cast<T>(2) * (q.x * q.z + q.w * q.y);

		m(2, 1) = static_cast<T>(2) * (q.x * q.y + q.w * q.z);
		m(2, 2) = static_cast<T>(2) * (q.w * q.w + q.y * q.y) - static_cast<T>(1);
		m(2, 3) = static_cast<T>(2) * (q.y * q.z - q.w * q.x);

		m(3, 1) = static_cast<T>(2) * (q.x * q.z - q.w * q.y);
		m(3, 2) = static_cast<T>(2) * (q.y * q.z + q.w * q.x);
		m(3, 3) = static_cast<T>(2) * (q.w * q.w + q.z * q.z) - static_cast<T>(1);

		m(4, 4) = 1.f;

		return m;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateTranslation(const Vector3<T>& aTranslation)
	{
		Matrix4x4<T> matrix;
		matrix(4, 1) = aTranslation.x;
		matrix(4, 2) = aTranslation.y;
		matrix(4, 3) = aTranslation.z;
		return matrix;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateTransform(const Vector3<T>& aTranslation, const Vector3<T>& aEulerAngles)
	{
		Matrix4x4<T> matrix = CreateRotation(aEulerAngles);
		matrix(4, 1) = aTranslation.x;
		matrix(4, 2) = aTranslation.y;
		matrix(4, 3) = aTranslation.z;
		return matrix;
	}

	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::operator+(const Matrix4x4<T>& aRhs) const
	{
		return Matrix4x4<T>(*this) += aRhs;
	}
	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::operator-(const Matrix4x4<T>& aRhs) const
	{
		return Matrix4x4<T>(*this) -= aRhs;
	}
	template<typename T>
	Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4<T>& aRhs) const
	{
		return Matrix4x4<T>(*this) *= aRhs;
	}

	template<typename T>
	inline bool Matrix4x4<T>::operator==(const Matrix4x4<T>& aRhs) const
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (myElements[i][j] != aRhs.myElements[i][j])
				{
					return false;
				}
			}
		}
		return true;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::operator-() const
	{
		Matrix4x4<T> result;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				result.myElements[i][j] = -myElements[i][j];
			}
		}
		return result;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4<T>& aRhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				this->myElements[i][j] += aRhs.myElements[i][j];
			}
		}
		return *this;
	}
	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator-=(const Matrix4x4<T>& aRhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				this->myElements[i][j] -= aRhs.myElements[i][j];
			}
		}
		return *this;
	}
	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4<T>& aRhs)
	{
		Matrix4x4<T> matrix(*this);
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				this->myElements[i][j] = static_cast<T>(0);
				for (size_t k = 0; k < 4; k++)
				{
					this->myElements[i][j] += matrix.myElements[i][k] * aRhs.myElements[k][j];
				}
			}
		}
		return *this;
	}


	template<typename T>
	Vector4<T> operator*(const Vector4<T>& aLhs, const Matrix4x4<T>& aRhs)
	{
		Vector4<T> vec;
		vec.x = aLhs.x * aRhs(1, 1) + aLhs.y * aRhs(2, 1) + aLhs.z * aRhs(3, 1) + aLhs.w * aRhs(4, 1);
		vec.y = aLhs.x * aRhs(1, 2) + aLhs.y * aRhs(2, 2) + aLhs.z * aRhs(3, 2) + aLhs.w * aRhs(4, 2);
		vec.z = aLhs.x * aRhs(1, 3) + aLhs.y * aRhs(2, 3) + aLhs.z * aRhs(3, 3) + aLhs.w * aRhs(4, 3);
		vec.w = aLhs.x * aRhs(1, 4) + aLhs.y * aRhs(2, 4) + aLhs.z * aRhs(3, 4) + aLhs.w * aRhs(4, 4);
		return vec;
	}
}