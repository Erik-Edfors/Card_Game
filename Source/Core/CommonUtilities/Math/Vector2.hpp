#pragma once
#include <cmath>

namespace CommonUtilities
{
	template<class _Ty>
	class Vector3;
	template<class _Ty>
	class Vector4;

	template <class _Ty>
	class Vector2
	{
	public:
		union
		{
#pragma warning(push)
#pragma warning(disable : 4201)
			struct
			{
				_Ty x, y;
			};
#pragma warning(pop)
			_Ty Data[2];
		};
		//Creates a vector (aX, aY)
		Vector2<_Ty>(const _Ty& aX, const _Ty& aY);
		//Creates a vector (aValues[0], aValues[1])
		Vector2<_Ty>(const _Ty aValues[2]);
		//Creates a zero-vector
		Vector2<_Ty>();
		//Copy constructor (compiler generated)
		Vector2<_Ty>(const Vector2<_Ty>& aVector2) = default;
		//Explicit conversion constructor from other vector type.
		template<class _VecTy>
		explicit Vector2<_Ty>(const _VecTy& aVector);
		//Assignment operator (compiler generated)
		Vector2<_Ty>& operator=(const Vector2<_Ty>& aVector2) = default;
		//Destructor (compiler generated)
		~Vector2<_Ty>() = default;
		//Explicit Type operator, create a different vector with the same values.
		//Example creates a Tga::Vector2<_Ty> from this CommonUtillities::Vector2<_Ty>
		template <class OtherVectorClass>
		explicit operator OtherVectorClass() const;
		//Returns a negated copy of the vector
		Vector2<_Ty> operator-() const;
		//Returns the squared length of the vector
		_Ty LengthSqr() const;
		//Returns the length of the vector
		_Ty Length() const;
		//Returns a normalized copy of this vector
		Vector2<_Ty> GetNormalized() const;
		//Normalizes the vector
		void Normalize();
		//Returns the dot product of this and aVector
		_Ty Dot(const Vector2<_Ty>& aVector) const;

		Vector3<_Ty> Vec3(_Ty aZ) const;
		Vector4<_Ty> Vec4(_Ty aZ, _Ty aW) const;

		Vector2<_Ty> xy() const;
		Vector2<_Ty> yx() const;

		Vector3<_Ty> xy0() const;
		Vector3<_Ty> xy1() const;

		Vector4<_Ty> xy00() const;
		Vector4<_Ty> xy01() const;
	};

	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;
	using Vector2i = Vector2<int>;
	using Vector2ui = Vector2<unsigned int>;

	template<class _Ty>
	inline Vector2<_Ty>::Vector2(const _Ty& aX, const _Ty& aY)
	{
		this->x = aX;
		this->y = aY;
	}
	template<class _Ty>
	inline Vector2<_Ty>::Vector2(const _Ty aValues[2])
	{
		this->x = aValues[0];
		this->y = aValues[1];
	}
	template<class _Ty>
	inline Vector2<_Ty>::Vector2() : Vector2<_Ty>(0, 0)
	{
	}
	template<class _Ty>
	template<class _VecTy>
	inline Vector2<_Ty>::Vector2(const _VecTy& aVector) : 
		x(static_cast<_Ty>(aVector.x)),
		y(static_cast<_Ty>(aVector.y))
	{
	}
	template<class _Ty>
	template<class OtherVectorClass>
	inline Vector2<_Ty>::operator OtherVectorClass() const
	{
		typedef decltype(OtherVectorClass::x) OtherVectorElementTy;
		return OtherVectorClass(static_cast<OtherVectorElementTy>(this->x), static_cast<OtherVectorElementTy>(this->y));
	}
	template<class _Ty>
	inline Vector2<_Ty> Vector2<_Ty>::operator-() const
	{
		return Vector2<_Ty>(-this->x, -this->y);
	}
	template<class _Ty>
	inline _Ty Vector2<_Ty>::LengthSqr() const
	{
		return this->x * this->x + this->y * this->y;
	}
	template<class _Ty>
	inline _Ty Vector2<_Ty>::Length() const
	{
		return static_cast<_Ty>(std::sqrt(LengthSqr()));
	}
	template<class _Ty>
	inline Vector2<_Ty> Vector2<_Ty>::GetNormalized() const
	{
		Vector2<_Ty> res = *this;
		res.Normalize();
		return res;
	}
	template<class _Ty>
	inline void Vector2<_Ty>::Normalize()
	{
		_Ty len = Length();
		this->x /= len;
		this->y /= len;
	}
	template<class _Ty>
	inline _Ty Vector2<_Ty>::Dot(const Vector2<_Ty>& aVector) const
	{
		return this->x * aVector.x + this->y * aVector.y;
	}
	//Returns the vector sum of aVector0 and aVector1
	template <class _Ty>
	Vector2<_Ty> operator+(const Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		return Vector2<_Ty>(
			aVector0.x + aVector1.x,
			aVector0.y + aVector1.y
		);
	}
	//Returns the vector difference of aVector0 and aVector1
	template <class _Ty>
	Vector2<_Ty> operator-(const Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		return Vector2<_Ty>(
			aVector0.x - aVector1.x,
			aVector0.y - aVector1.y
		);
	}
	//Returns the vector aVector1 component-multiplied by aVector1
	template <class _Ty>
	Vector2<_Ty> operator*(const Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		return Vector2<_Ty>(
			aVector0.x * aVector1.x,
			aVector0.y * aVector1.y
		);
	}
	template <class _Ty>
	Vector2<_Ty> operator/(const Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		return Vector2<_Ty>(
			aVector0.x / aVector1.x,
			aVector0.y / aVector1.y
		);
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class _Ty>
	Vector2<_Ty> operator*(const Vector2<_Ty>& aVector, const _Ty& aScalar)
	{
		return Vector2<_Ty>(
			aScalar * aVector.x,
			aScalar * aVector.y
		);
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class _Ty>
	Vector2<_Ty> operator*(const _Ty& aScalar, const Vector2<_Ty>& aVector)
	{
		return aVector * aScalar;
	}
	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class _Ty>
	Vector2<_Ty> operator/(const Vector2<_Ty>& aVector, const _Ty& aScalar)
	{
		return Vector2<_Ty>(
			aVector.x / aScalar,
			aVector.y / aScalar
		);
	}
	template <class _Ty>
	Vector2<_Ty> operator/(const _Ty& aScalar, const Vector2<_Ty>& aVector)
	{
		return Vector2<_Ty>(
			aScalar / aVector.x,
			aScalar / aVector.y
		);
	}
	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class _Ty>
	Vector2<_Ty>& operator+=(Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		return aVector0;
	}
	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class _Ty>
	Vector2<_Ty>& operator-=(Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		return aVector0;
	}
	template <class _Ty>
	Vector2<_Ty>& operator*=(Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		aVector0.x *= aVector1.x;
		aVector0.y *= aVector1.y;
		return aVector0;
	}
	template <class _Ty>
	Vector2<_Ty>& operator/=(Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		aVector0.x /= aVector1.x;
		aVector0.y /= aVector1.y;
		return aVector0;
	}
	//Equivalent to setting aVector to (aVector * aScalar)
	template <class _Ty>
	Vector2<_Ty>& operator*=(Vector2<_Ty>& aVector, const _Ty& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		return aVector;
	}
	//Equivalent to setting aVector to (aVector / aScalar)
	template <class _Ty>
	Vector2<_Ty>& operator/=(Vector2<_Ty>& aVector, const _Ty& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		return aVector;
	}
	template <class _Ty>
	bool operator==(const Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		return aVector0.x == aVector1.x && aVector0.y == aVector1.y;
	}
	template <class _Ty>
	bool operator!=(const Vector2<_Ty>& aVector0, const Vector2<_Ty>& aVector1)
	{
		return aVector0.x != aVector1.x || aVector0.y != aVector1.y;
	}



	template<class _Ty>
	inline Vector3<_Ty> Vector2<_Ty>::Vec3(_Ty aZ) const
	{
		return Vector3<_Ty>(x, y, aZ);
	}
	template<class _Ty>
	inline Vector4<_Ty> Vector2<_Ty>::Vec4(_Ty aZ, _Ty aW) const
	{
		return Vector4<_Ty>(x, y, aZ, aW);
	}
	template<class _Ty>
	inline Vector2<_Ty> Vector2<_Ty>::xy() const
	{
		return Vector2<_Ty>(x, y);
	}
	template<class _Ty>
	inline Vector2<_Ty> Vector2<_Ty>::yx() const
	{
		return Vector2<_Ty>(y, x);
	}
	template<class _Ty>
	inline Vector3<_Ty> Vector2<_Ty>::xy0() const
	{
		return Vec3(static_cast<_Ty>(0));
	}
	template<class _Ty>
	inline Vector3<_Ty> Vector2<_Ty>::xy1() const
	{
		return Vec3(static_cast<_Ty>(1));
	}
	template<class _Ty>
	inline Vector4<_Ty> Vector2<_Ty>::xy00() const
	{
		return Vec4(static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector2<_Ty>::xy01() const
	{
		return Vec4(static_cast<_Ty>(0), static_cast<_Ty>(1));
	}
}