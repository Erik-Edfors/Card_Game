#pragma once
#include <cmath>

namespace CommonUtilities
{
	template<class _Ty>
	class Vector2;
	template<class _Ty>
	class Vector4;

	template <class _Ty>
	class Vector3
	{
	public:
		union
		{
#pragma warning(push)
#pragma warning(disable : 4201)
			struct
			{
				_Ty x, y, z;
			};
			_Ty Data[3];
#pragma warning(pop)
		};
		//Creates a vector (aX, aY, aZ)
		Vector3<_Ty>(const _Ty& aX, const _Ty& aY, const _Ty& aZ);
		//Creates a vector (aValues[0], aValues[1], aValues[2])
		Vector3<_Ty>(const _Ty aValues[3]);
		//Creates a zero-vector
		Vector3<_Ty>();
		//Copy constructor (compiler generated)
		Vector3<_Ty>(const Vector3<_Ty>& aVector3) = default;
		//
		Vector3<_Ty>(const Vector2<_Ty>& aVector2, const _Ty& aZ);
		//Explicit conversion constructor from other vector type.
		template<class _VecTy>
		explicit Vector3<_Ty>(const _VecTy& aVector);
		//Assignment operator (compiler generated)
		Vector3<_Ty>& operator=(const Vector3<_Ty>& aVector3) = default;
		//Destructor (compiler generated)
		~Vector3<_Ty>() = default;
		//Explicit Type operator, create a different vector with the same values.
		//Example creates a Tga::Vector3<_Ty> from this CommonUtillities::Vector3<_Ty>
		template <class OtherVectorClass>
		explicit operator OtherVectorClass() const;
		//Returns a negated copy of the vector
		Vector3<_Ty> operator-() const;
		//Returns the squared length of the vector
		_Ty LengthSqr() const;
		//Returns the length of the vector
		_Ty Length() const;
		//Returns a normalized copy of this vector
		Vector3<_Ty> GetNormalized() const;
		//Normalizes the vector
		void Normalize();
		//Returns the dot product of this and aVector
		_Ty Dot(const Vector3<_Ty>& aVector) const;
		//Returns the cross product of this and aVector
		Vector3<_Ty> Cross(const Vector3<_Ty>& aVector) const;

		Vector4<_Ty> Vec4(const _Ty& aW) const;

		Vector2<_Ty> xy() const;
		Vector2<_Ty> xz() const;

		Vector2<_Ty> yz() const;
		Vector2<_Ty> yx() const;

		Vector2<_Ty> zx() const;
		Vector2<_Ty> zy() const;

		Vector3<_Ty> xyz() const;
		Vector3<_Ty> xzy() const;

		Vector3<_Ty> yxz() const;
		Vector3<_Ty> yzx() const;

		Vector3<_Ty> zxy() const;
		Vector3<_Ty> zyx() const;

		Vector4<_Ty> xyz0() const;
		Vector4<_Ty> xyz1() const;
	};

	using Vector3f = Vector3<float>;
	using Vector3d = Vector3<double>;
	using Vector3i = Vector3<int>;
	using Vector3ui = Vector3<unsigned int>;

	template<class _Ty>
	inline Vector3<_Ty>::Vector3(const _Ty& aX, const _Ty& aY, const _Ty& aZ)
	{
		this->x = aX;
		this->y = aY;
		this->z = aZ;
	}
	template<class _Ty>
	inline Vector3<_Ty>::Vector3(const _Ty aValues[3])
	{
		this->x = aValues[0];
		this->y = aValues[1];
		this->z = aValues[2];
	}
	template<class _Ty>
	inline Vector3<_Ty>::Vector3() : Vector3<_Ty>(0, 0, 0)
	{
	}
	template<class _Ty>
	inline Vector3<_Ty>::Vector3(const Vector2<_Ty>& aVector2, const _Ty& aZ) : Vector3<_Ty>(aVector2.x, aVector2.y, aZ)
	{
	}
	template<class _Ty>
	template<class _VecTy>
	inline Vector3<_Ty>::Vector3(const _VecTy& aVector) : 
		x(static_cast<_Ty>(aVector.x)),
		y(static_cast<_Ty>(aVector.y)),
		z(static_cast<_Ty>(aVector.z))
	{
	}
	template<class _Ty>
	template<class OtherVectorClass>
	inline Vector3<_Ty>::operator OtherVectorClass() const
	{
		return OtherVectorClass(this->x, this->y, this->z);
	}
	template<class _Ty>
	inline Vector3<_Ty> Vector3<_Ty>::operator-() const
	{
		return Vector3<_Ty>(-this->x, -this->y, -this->z);
	}
	template<class _Ty>
	inline _Ty Vector3<_Ty>::LengthSqr() const
	{
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}
	template<class _Ty>
	inline _Ty Vector3<_Ty>::Length() const
	{
		return static_cast<_Ty>(std::sqrt(LengthSqr()));
	}
	template<class _Ty>
	inline Vector3<_Ty> Vector3<_Ty>::GetNormalized() const
	{
		Vector3<_Ty> res = *this;
		res.Normalize();
		return res;
	}
	template<class _Ty>
	inline void Vector3<_Ty>::Normalize()
	{
		_Ty len = Length();
		this->x /= len;
		this->y /= len;
		this->z /= len;
	}
	template<class _Ty>
	inline _Ty Vector3<_Ty>::Dot(const Vector3<_Ty>& aVector) const
	{
		return this->x * aVector.x + this->y * aVector.y + this->z * aVector.z;
	}
	template<class _Ty>
	inline Vector3<_Ty> Vector3<_Ty>::Cross(const Vector3<_Ty>& aVector) const
	{
		return Vector3<_Ty>(
			this->y * aVector.z - this->z * aVector.y,
			this->z * aVector.x - this->x * aVector.z,
			this->x * aVector.y - this->y * aVector.x
		);
	}
	//Returns the vector sum of aVector0 and aVector1
	template <class _Ty>
	Vector3<_Ty> operator+(const Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		return Vector3<_Ty>(
			aVector0.x + aVector1.x,
			aVector0.y + aVector1.y,
			aVector0.z + aVector1.z
		);
	}
	//Returns the vector difference of aVector0 and aVector1
	template <class _Ty>
	Vector3<_Ty> operator-(const Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		return Vector3<_Ty>(
			aVector0.x - aVector1.x,
			aVector0.y - aVector1.y,
			aVector0.z - aVector1.z
		);
	}
	//Returns the vector aVector1 component-multiplied by aVector1
	template <class _Ty>
	Vector3<_Ty> operator*(const Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		return Vector3<_Ty>(
			aVector0.x * aVector1.x,
			aVector0.y * aVector1.y,
			aVector0.z * aVector1.z
		);
	}
	template <class _Ty>
	Vector3<_Ty> operator/(const Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		return Vector3<_Ty>(
			aVector0.x / aVector1.x,
			aVector0.y / aVector1.y,
			aVector0.z / aVector1.z
		);
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class _Ty>
	Vector3<_Ty> operator*(const Vector3<_Ty>& aVector, const _Ty& aScalar)
	{
		return Vector3<_Ty>(
			aScalar * aVector.x,
			aScalar * aVector.y,
			aScalar * aVector.z
		);
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class _Ty>
	Vector3<_Ty> operator*(const _Ty& aScalar, const Vector3<_Ty>& aVector)
	{
		return aVector * aScalar;
	}
	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class _Ty>
	Vector3<_Ty> operator/(const Vector3<_Ty>& aVector, const _Ty& aScalar)
	{
		return Vector3<_Ty>(
			aVector.x / aScalar,
			aVector.y / aScalar,
			aVector.z / aScalar
		);
	}

	template <class _Ty>
	Vector3<_Ty> operator/(const _Ty& aScalar, const Vector3<_Ty>& aVector)
	{
		return Vector3<_Ty>(
			aScalar / aVector.x,
			aScalar / aVector.y,
			aScalar / aVector.z
		);
	}
	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class _Ty>
	Vector3<_Ty>& operator+=(Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		return aVector0;
	}
	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class _Ty>
	Vector3<_Ty>& operator-=(Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		return aVector0;
	}
	template <class _Ty>
	Vector3<_Ty>& operator*=(Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		aVector0.x *= aVector1.x;
		aVector0.y *= aVector1.y;
		aVector0.z *= aVector1.z;
		return aVector0;
	}
	template <class _Ty>
	Vector3<_Ty>& operator/=(Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		aVector0.x /= aVector1.x;
		aVector0.y /= aVector1.y;
		aVector0.z /= aVector1.z;
		return aVector0;
	}
	//Equivalent to setting aVector to (aVector * aScalar)
	template <class _Ty>
	Vector3<_Ty>& operator*=(Vector3<_Ty>& aVector, const _Ty& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
		return aVector;
	}
	//Equivalent to setting aVector to (aVector / aScalar)
	template <class _Ty>
	Vector3<_Ty>& operator/=(Vector3<_Ty>& aVector, const _Ty& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
		return aVector;
	}

	template <class _Ty>
	bool operator==(const Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		return aVector0.x == aVector1.x && aVector0.y == aVector1.y && aVector0.z == aVector1.z;
	}
	template <class _Ty>
	bool operator!=(const Vector3<_Ty>& aVector0, const Vector3<_Ty>& aVector1)
	{
		return aVector0.x != aVector1.x || aVector0.y != aVector1.y || aVector0.z != aVector1.z;
	}



	template<class _Ty>
	inline Vector4<_Ty> Vector3<_Ty>::Vec4(const _Ty& aW) const
	{
		return Vector4<_Ty>(x, y, z, aW);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector3<_Ty>::xy() const
	{
		return Vector2<_Ty>(x, y);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector3<_Ty>::xz() const
	{
		return Vector2<_Ty>(x, z);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector3<_Ty>::yz() const
	{
		return Vector2<_Ty>(y, z);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector3<_Ty>::yx() const
	{
		return Vector2<_Ty>(y, x);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector3<_Ty>::zx() const
	{
		return Vector2<_Ty>(z, x);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector3<_Ty>::zy() const
	{
		return Vector2<_Ty>(z, y);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector3<_Ty>::xyz() const
	{
		return Vector3<_Ty>(x, y, z);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector3<_Ty>::xzy() const
	{
		return Vector3<_Ty>(x, z, y);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector3<_Ty>::yxz() const
	{
		return Vector3<_Ty>(y, x, z);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector3<_Ty>::yzx() const
	{
		return Vector3<_Ty>(y, z, x);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector3<_Ty>::zxy() const
	{
		return Vector3<_Ty>(z, x, y);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector3<_Ty>::zyx() const
	{
		return Vector3<_Ty>(z, y, x);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector3<_Ty>::xyz0() const
	{
		return Vec4(static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector3<_Ty>::xyz1() const
	{
		return Vec4(static_cast<_Ty>(1));
	}
}