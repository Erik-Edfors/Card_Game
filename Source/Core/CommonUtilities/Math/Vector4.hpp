#pragma once
#include <cmath>
#include "Vector3.hpp"
#include "Vector2.hpp"

namespace CommonUtilities
{
	template <class _Ty>
	class Vector4
	{
	public:
		union
		{
#pragma warning(push)
#pragma warning(disable : 4201)
			struct
			{
				_Ty x, y, z, w;
			};
#pragma warning(pop)
			_Ty Data[4];
		};
		//Creates a vector (aX, aY, aZ)
		constexpr Vector4<_Ty>(const _Ty& aX, const _Ty& aY, const _Ty& aZ, const _Ty& aW);
		//Creates a vector (aValues[0], aValues[1], aValues[2], aValues[3])
		constexpr Vector4<_Ty>(const _Ty aValues[4]);
		//Creates a vector from a Vector3 and a W scalar.
		constexpr Vector4<_Ty>(const Vector3<_Ty>& aVector3, const _Ty& aW);
		//Creates a zero-vector
		constexpr Vector4<_Ty>();
		//Copy constructor (compiler generated)
		constexpr Vector4<_Ty>(const Vector4<_Ty>& aVector4) = default;
		//Explicit conversion constructor from other vector type.
		template<class _VecTy>
		constexpr explicit Vector4<_Ty>(const _VecTy& aVector);
		//Assignment operator (compiler generated)
		constexpr Vector4<_Ty>& operator=(const Vector4<_Ty>& aVector4) = default;
		//Destructor (compiler generated)
		constexpr ~Vector4<_Ty>() = default;
		//Explicit Type operator, create a different vector with the same values.
		//Example creates a Tga::Vector4<_Ty> from this CommonUtillities::Vector4<_Ty>
		template <class OtherVectorClass>
		constexpr explicit operator OtherVectorClass() const;
		//Returns a negated copy of the vector
		constexpr Vector4<_Ty> operator-() const;
		//Returns the squared length of the vector
		constexpr _Ty LengthSqr() const;
		//Returns the length of the vector
		constexpr _Ty Length() const;
		//Returns a normalized copy of this vector
		constexpr Vector4<_Ty> GetNormalized() const;
		//Normalizes the vector
		constexpr void Normalize();
		//Returns the dot product of this and aVector
		constexpr _Ty Dot(const Vector4<_Ty>& aVector) const;


		constexpr Vector2<_Ty> xy() const;
		constexpr Vector2<_Ty> xz() const;
		constexpr Vector2<_Ty> xw() const;

		constexpr Vector2<_Ty> yz() const;
		constexpr Vector2<_Ty> yw() const;
		constexpr Vector2<_Ty> yx() const;

		constexpr Vector2<_Ty> zw() const;
		constexpr Vector2<_Ty> zx() const;
		constexpr Vector2<_Ty> zy() const;

		constexpr Vector2<_Ty> wx() const;
		constexpr Vector2<_Ty> wy() const;
		constexpr Vector2<_Ty> wz() const;

		constexpr Vector3<_Ty> xyz() const;
		constexpr Vector3<_Ty> xyw() const;
		constexpr Vector3<_Ty> xzy() const;
		constexpr Vector3<_Ty> xzw() const;
		constexpr Vector3<_Ty> xwy() const;
		constexpr Vector3<_Ty> xwz() const;

		constexpr Vector3<_Ty> yxz() const;
		constexpr Vector3<_Ty> yxw() const;
		constexpr Vector3<_Ty> yzx() const;
		constexpr Vector3<_Ty> yzw() const;
		constexpr Vector3<_Ty> ywx() const;
		constexpr Vector3<_Ty> ywz() const;

		constexpr Vector3<_Ty> zxy() const;
		constexpr Vector3<_Ty> zxw() const;
		constexpr Vector3<_Ty> zyx() const;
		constexpr Vector3<_Ty> zyw() const;
		constexpr Vector3<_Ty> zwx() const;
		constexpr Vector3<_Ty> zwy() const;

		constexpr Vector3<_Ty> wxy() const;
		constexpr Vector3<_Ty> wxz() const;
		constexpr Vector3<_Ty> wyx() const;
		constexpr Vector3<_Ty> wyz() const;
		constexpr Vector3<_Ty> wzx() const;
		constexpr Vector3<_Ty> wzy() const;

		constexpr Vector3<_Ty> xy0() const;
		constexpr Vector3<_Ty> xz0() const;
		constexpr Vector3<_Ty> xw0() const;

		constexpr Vector3<_Ty> yz0() const;
		constexpr Vector3<_Ty> yw0() const;
		constexpr Vector3<_Ty> yx0() const;

		constexpr Vector3<_Ty> zw0() const;
		constexpr Vector3<_Ty> zx0() const;
		constexpr Vector3<_Ty> zy0() const;

		constexpr Vector3<_Ty> wx0() const;
		constexpr Vector3<_Ty> wy0() const;
		constexpr Vector3<_Ty> wz0() const;

		constexpr Vector3<_Ty> xy1() const;
		constexpr Vector3<_Ty> xz1() const;
		constexpr Vector3<_Ty> xw1() const;

		constexpr Vector3<_Ty> yz1() const;
		constexpr Vector3<_Ty> yw1() const;
		constexpr Vector3<_Ty> yx1() const;

		constexpr Vector3<_Ty> zw1() const;
		constexpr Vector3<_Ty> zx1() const;
		constexpr Vector3<_Ty> zy1() const;

		constexpr Vector3<_Ty> wx1() const;
		constexpr Vector3<_Ty> wy1() const;
		constexpr Vector3<_Ty> wz1() const;

		constexpr Vector4<_Ty> xy00() const;
		constexpr Vector4<_Ty> xz00() const;
		constexpr Vector4<_Ty> xw00() const;

		constexpr Vector4<_Ty> yz00() const;
		constexpr Vector4<_Ty> yw00() const;
		constexpr Vector4<_Ty> yx00() const;

		constexpr Vector4<_Ty> zw00() const;
		constexpr Vector4<_Ty> zx00() const;
		constexpr Vector4<_Ty> zy00() const;

		constexpr Vector4<_Ty> wx00() const;
		constexpr Vector4<_Ty> wy00() const;
		constexpr Vector4<_Ty> wz00() const;

		constexpr Vector4<_Ty> xyz0() const;
		constexpr Vector4<_Ty> xyw0() const;
		constexpr Vector4<_Ty> xzy0() const;
		constexpr Vector4<_Ty> xzw0() const;
		constexpr Vector4<_Ty> xwy0() const;
		constexpr Vector4<_Ty> xwz0() const;

		constexpr Vector4<_Ty> yxz0() const;
		constexpr Vector4<_Ty> yxw0() const;
		constexpr Vector4<_Ty> yzx0() const;
		constexpr Vector4<_Ty> yzw0() const;
		constexpr Vector4<_Ty> ywx0() const;
		constexpr Vector4<_Ty> ywz0() const;

		constexpr Vector4<_Ty> zxy0() const;
		constexpr Vector4<_Ty> zxw0() const;
		constexpr Vector4<_Ty> zyx0() const;
		constexpr Vector4<_Ty> zyw0() const;
		constexpr Vector4<_Ty> zwx0() const;
		constexpr Vector4<_Ty> zwy0() const;

		constexpr Vector4<_Ty> wxy0() const;
		constexpr Vector4<_Ty> wxz0() const;
		constexpr Vector4<_Ty> wyx0() const;
		constexpr Vector4<_Ty> wyz0() const;
		constexpr Vector4<_Ty> wzx0() const;
		constexpr Vector4<_Ty> wzy0() const;

		constexpr Vector4<_Ty> xy01() const;
		constexpr Vector4<_Ty> xz01() const;
		constexpr Vector4<_Ty> xw01() const;

		constexpr Vector4<_Ty> yz01() const;
		constexpr Vector4<_Ty> yw01() const;
		constexpr Vector4<_Ty> yx01() const;

		constexpr Vector4<_Ty> zw01() const;
		constexpr Vector4<_Ty> zx01() const;
		constexpr Vector4<_Ty> zy01() const;

		constexpr Vector4<_Ty> wx01() const;
		constexpr Vector4<_Ty> wy01() const;
		constexpr Vector4<_Ty> wz01() const;

		constexpr Vector4<_Ty> xyz1() const;
		constexpr Vector4<_Ty> xyw1() const;
		constexpr Vector4<_Ty> xzy1() const;
		constexpr Vector4<_Ty> xzw1() const;
		constexpr Vector4<_Ty> xwy1() const;
		constexpr Vector4<_Ty> xwz1() const;

		constexpr Vector4<_Ty> yxz1() const;
		constexpr Vector4<_Ty> yxw1() const;
		constexpr Vector4<_Ty> yzx1() const;
		constexpr Vector4<_Ty> yzw1() const;
		constexpr Vector4<_Ty> ywx1() const;
		constexpr Vector4<_Ty> ywz1() const;

		constexpr Vector4<_Ty> zxy1() const;
		constexpr Vector4<_Ty> zxw1() const;
		constexpr Vector4<_Ty> zyx1() const;
		constexpr Vector4<_Ty> zyw1() const;
		constexpr Vector4<_Ty> zwx1() const;
		constexpr Vector4<_Ty> zwy1() const;

		constexpr Vector4<_Ty> wxy1() const;
		constexpr Vector4<_Ty> wxz1() const;
		constexpr Vector4<_Ty> wyx1() const;
		constexpr Vector4<_Ty> wyz1() const;
		constexpr Vector4<_Ty> wzx1() const;
		constexpr Vector4<_Ty> wzy1() const;

		constexpr Vector4<_Ty> xyzw() const;
		constexpr Vector4<_Ty> xywz() const;
		constexpr Vector4<_Ty> xzyw() const;
		constexpr Vector4<_Ty> xzwy() const;
		constexpr Vector4<_Ty> xwyz() const;
		constexpr Vector4<_Ty> xwzy() const;

		constexpr Vector4<_Ty> yxzw() const;
		constexpr Vector4<_Ty> yxwz() const;
		constexpr Vector4<_Ty> yzxw() const;
		constexpr Vector4<_Ty> yzwx() const;
		constexpr Vector4<_Ty> ywxz() const;
		constexpr Vector4<_Ty> ywzx() const;

		constexpr Vector4<_Ty> zxyw() const;
		constexpr Vector4<_Ty> zxwy() const;
		constexpr Vector4<_Ty> zyxw() const;
		constexpr Vector4<_Ty> zywx() const;
		constexpr Vector4<_Ty> zwxy() const;
		constexpr Vector4<_Ty> zwyx() const;

		constexpr Vector4<_Ty> wxyz() const;
		constexpr Vector4<_Ty> wxzy() const;
		constexpr Vector4<_Ty> wyxz() const;
		constexpr Vector4<_Ty> wyzx() const;
		constexpr Vector4<_Ty> wzxy() const;
		constexpr Vector4<_Ty> wzyx() const;
	};

	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4i = Vector4<int>;
	using Vector4ui = Vector4<unsigned int>;

	template<class _Ty>
	inline constexpr Vector4<_Ty>::Vector4(const _Ty& aX, const _Ty& aY, const _Ty& aZ, const _Ty& aW) : 
		x(aX), y(aY), z(aZ), w(aW)
	{
	}
	template<class _Ty>
	inline constexpr Vector4<_Ty>::Vector4(const _Ty aValues[4]) :
		x(aValues[0]), y(aValues[1]), z(aValues[2]), w(aValues[3])
	{
	}
	template<class _Ty>
	inline constexpr Vector4<_Ty>::Vector4(const Vector3<_Ty>& aVector3, const _Ty& aW) :
		x(aVector3.x), y(aVector3.y), z(aVector3.z), w(aW)
	{
	}
	template<class _Ty>
	inline constexpr Vector4<_Ty>::Vector4() : Vector4<_Ty>(0, 0, 0, 0)
	{
	}
	template<class _Ty>
	template<class _VecTy>
	inline constexpr Vector4<_Ty>::Vector4(const _VecTy& aVector) : 
		x(static_cast<_Ty>(aVector.x)),
		y(static_cast<_Ty>(aVector.y)),
		z(static_cast<_Ty>(aVector.z)),
		w(static_cast<_Ty>(aVector.w))
	{
	}
	template<class _Ty>
	template<class OtherVectorClass>
	inline constexpr Vector4<_Ty>::operator OtherVectorClass() const
	{
		return OtherVectorClass(x, y, z, w);
	}
	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::operator-() const
	{
		return Vector4<_Ty>(-x, -y, -z, -w);
	}
	template<class _Ty>
	inline constexpr _Ty Vector4<_Ty>::LengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}
	template<class _Ty>
	inline constexpr _Ty Vector4<_Ty>::Length() const
	{
		return static_cast<_Ty>(std::sqrt(LengthSqr()));
	}
	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::GetNormalized() const
	{
		Vector4<_Ty> res = *this;
		res.Normalize();
		return res;
	}
	template<class _Ty>
	inline constexpr void Vector4<_Ty>::Normalize()
	{
		_Ty len = Length();
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}
	template<class _Ty>
	inline constexpr _Ty Vector4<_Ty>::Dot(const Vector4<_Ty>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}
	//Returns the vector sum of aVector0 and aVector1
	template <class _Ty>
	inline constexpr Vector4<_Ty> operator+(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		return Vector4<_Ty>(
			aVector0.x + aVector1.x,
			aVector0.y + aVector1.y,
			aVector0.z + aVector1.z,
			aVector0.w + aVector1.w
		);
	}
	//Returns the vector difference of aVector0 and aVector1
	template <class _Ty>
	inline constexpr Vector4<_Ty> operator-(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		return Vector4<_Ty>(
			aVector0.x - aVector1.x,
			aVector0.y - aVector1.y,
			aVector0.z - aVector1.z,
			aVector0.w - aVector1.w
		);
	}
	//Returns the vector aVector1 component-multiplied by aVector1
	template <class _Ty>
	inline constexpr Vector4<_Ty> operator*(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		return Vector4<_Ty>(
			aVector0.x * aVector1.x,
			aVector0.y * aVector1.y,
			aVector0.z * aVector1.z,
			aVector0.w * aVector1.w
		);
	}
	template <class _Ty>
	inline constexpr Vector4<_Ty> operator/(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		return Vector4<_Ty>(
			aVector0.x / aVector1.x,
			aVector0.y / aVector1.y,
			aVector0.z / aVector1.z,
			aVector0.w / aVector1.w
		);
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class _Ty>
	inline constexpr Vector4<_Ty> operator*(const Vector4<_Ty>& aVector, const _Ty& aScalar)
	{
		return Vector4<_Ty>(
			aScalar * aVector.x,
			aScalar * aVector.y,
			aScalar * aVector.z,
			aScalar * aVector.w
		);
	}
	//Returns the vector aVector multiplied by the scalar aScalar
	template <class _Ty>
	inline constexpr Vector4<_Ty> operator*(const _Ty& aScalar, const Vector4<_Ty>& aVector)
	{
		return aVector * aScalar;
	}
	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class _Ty>
	inline constexpr Vector4<_Ty> operator/(const Vector4<_Ty>& aVector, const _Ty& aScalar)
	{
		return Vector4<_Ty>(
			aVector.x / aScalar,
			aVector.y / aScalar,
			aVector.z / aScalar,
			aVector.w / aScalar
		);
	}

	template <class _Ty>
	inline constexpr Vector4<_Ty> operator/(const _Ty& aScalar, const Vector4<_Ty>& aVector)
	{
		return Vector4<_Ty>(
			aScalar / aVector.x,
			aScalar / aVector.y,
			aScalar / aVector.z,
			aScalar / aVector.w
		);
	}
	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class _Ty>
	inline constexpr Vector4<_Ty>& operator+=(Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		aVector0.w += aVector1.w;
		return aVector0;
	}
	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class _Ty>
	inline constexpr Vector4<_Ty>& operator-=(Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		aVector0.w -= aVector1.w;
		return aVector0;
	}
	template <class _Ty>
	inline constexpr Vector4<_Ty>& operator*=(Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		aVector0.x *= aVector1.x;
		aVector0.y *= aVector1.y;
		aVector0.z *= aVector1.z;
		aVector0.w *= aVector1.w;
		return aVector0;
	}
	template <class _Ty>
	inline constexpr Vector4<_Ty>& operator/=(Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		aVector0.x /= aVector1.x;
		aVector0.y /= aVector1.y;
		aVector0.z /= aVector1.z;
		aVector0.w /= aVector1.w;
		return aVector0;
	}
	//Equivalent to setting aVector to (aVector * aScalar)
	template <class _Ty>
	inline constexpr Vector4<_Ty>& operator*=(Vector4<_Ty>& aVector, const _Ty& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
		aVector.w *= aScalar;
		return aVector;
	}
	//Equivalent to setting aVector to (aVector / aScalar)
	template <class _Ty>
	inline constexpr Vector4<_Ty>& operator/=(Vector4<_Ty>& aVector, const _Ty& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
		aVector.w /= aScalar;
		return aVector;
	}

	template <class _Ty>
	inline constexpr bool operator==(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		return aVector0.x == aVector1.x && aVector0.y == aVector1.y && aVector0.z == aVector1.z && aVector0.w == aVector1.w;
	}
	template <class _Ty>
	inline constexpr bool operator!=(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		return aVector0.x != aVector1.x || aVector0.y != aVector1.y || aVector0.z != aVector1.z || aVector0.w != aVector1.w;
	}



	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::xy() const
	{
		return Vector2<_Ty>(x, y);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::xz() const
	{
		return Vector2<_Ty>(x, z);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::xw() const
	{
		return Vector2<_Ty>(x, w);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::yz() const
	{
		return Vector2<_Ty>(y, z);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::yw() const
	{
		return Vector2<_Ty>(y, w);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::yx() const
	{
		return Vector2<_Ty>(y, x);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::zw() const
	{
		return Vector2<_Ty>(z, w);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::zx() const
	{
		return Vector2<_Ty>(z, x);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::zy() const
	{
		return Vector2<_Ty>(z, y);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::wx() const
	{
		return Vector2<_Ty>(w, x);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::wy() const
	{
		return Vector2<_Ty>(w, y);
	}

	template<class _Ty>
	inline constexpr Vector2<_Ty> Vector4<_Ty>::wz() const
	{
		return Vector2<_Ty>(w, z);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xyz() const
	{
		return Vector3<_Ty>(x, y, z);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xyw() const
	{
		return Vector3<_Ty>(x, y, w);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xzy() const
	{
		return Vector3<_Ty>(x, z, y);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xzw() const
	{
		return Vector3<_Ty>(x, z, w);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xwy() const
	{
		return Vector3<_Ty>(x, w, y);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xwz() const
	{
		return Vector3<_Ty>(x, w, z);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yxz() const
	{
		return Vector3<_Ty>(y, x, z);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yxw() const
	{
		return Vector3<_Ty>(y, x, w);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yzx() const
	{
		return Vector3<_Ty>(y, z, x);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yzw() const
	{
		return Vector3<_Ty>(y, z, w);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::ywx() const
	{
		return Vector3<_Ty>(y, w, x);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::ywz() const
	{
		return Vector3<_Ty>(y, w, z);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zxy() const
	{
		return Vector3<_Ty>(z, x, y);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zxw() const
	{
		return Vector3<_Ty>(z, x, w);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zyx() const
	{
		return Vector3<_Ty>(z, y, x);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zyw() const
	{
		return Vector3<_Ty>(z, y, w);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zwx() const
	{
		return Vector3<_Ty>(z, w, x);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zwy() const
	{
		return Vector3<_Ty>(z, w, y);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wxy() const
	{
		return Vector3<_Ty>(w, x, y);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wxz() const
	{
		return Vector3<_Ty>(w, x, z);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wyx() const
	{
		return Vector3<_Ty>(w, y, x);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wyz() const
	{
		return Vector3<_Ty>(w, y, z);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wzx() const
	{
		return Vector3<_Ty>(w, z, x);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wzy() const
	{
		return Vector3<_Ty>(w, z, y);
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xy0() const
	{
		return Vector3<_Ty>(x, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xz0() const
	{
		return Vector3<_Ty>(x, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xw0() const
	{
		return Vector3<_Ty>(x, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yz0() const
	{
		return Vector3<_Ty>(y, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yw0() const
	{
		return Vector3<_Ty>(y, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yx0() const
	{
		return Vector3<_Ty>(y, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zw0() const
	{
		return Vector3<_Ty>(z, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zx0() const
	{
		return Vector3<_Ty>(z, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zy0() const
	{
		return Vector3<_Ty>(z, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wx0() const
	{
		return Vector3<_Ty>(w, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wy0() const
	{
		return Vector3<_Ty>(w, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wz0() const
	{
		return Vector3<_Ty>(w, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xy1() const
	{
		return Vector3<_Ty>(x, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xz1() const
	{
		return Vector3<_Ty>(x, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::xw1() const
	{
		return Vector3<_Ty>(x, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yz1() const
	{
		return Vector3<_Ty>(y, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yw1() const
	{
		return Vector3<_Ty>(y, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::yx1() const
	{
		return Vector3<_Ty>(y, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zw1() const
	{
		return Vector3<_Ty>(z, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zx1() const
	{
		return Vector3<_Ty>(z, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::zy1() const
	{
		return Vector3<_Ty>(z, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wx1() const
	{
		return Vector3<_Ty>(w, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wy1() const
	{
		return Vector3<_Ty>(w, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector3<_Ty> Vector4<_Ty>::wz1() const
	{
		return Vector3<_Ty>(w, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xy00() const
	{
		return Vector4<_Ty>(x, y, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xz00() const
	{
		return Vector4<_Ty>(x, z, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xw00() const
	{
		return Vector4<_Ty>(x, w, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yz00() const
	{
		return Vector4<_Ty>(y, z, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yw00() const
	{
		return Vector4<_Ty>(y, w, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yx00() const
	{
		return Vector4<_Ty>(y, x, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zw00() const
	{
		return Vector4<_Ty>(z, w, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zx00() const
	{
		return Vector4<_Ty>(z, x, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zy00() const
	{
		return Vector4<_Ty>(z, y, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wx00() const
	{
		return Vector4<_Ty>(w, x, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wy00() const
	{
		return Vector4<_Ty>(w, y, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wz00() const
	{
		return Vector4<_Ty>(w, z, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xyz0() const
	{
		return Vector4<_Ty>(x, y, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xyw0() const
	{
		return Vector4<_Ty>(x, y, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xzy0() const
	{
		return Vector4<_Ty>(x, z, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xzw0() const
	{
		return Vector4<_Ty>(x, z, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xwy0() const
	{
		return Vector4<_Ty>(x, w, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xwz0() const
	{
		return Vector4<_Ty>(x, w, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yxz0() const
	{
		return Vector4<_Ty>(y, x, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yxw0() const
	{
		return Vector4<_Ty>(y, x, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yzx0() const
	{
		return Vector4<_Ty>(y, z, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yzw0() const
	{
		return Vector4<_Ty>(y, z, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::ywx0() const
	{
		return Vector4<_Ty>(y, w, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::ywz0() const
	{
		return Vector4<_Ty>(y, w, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zxy0() const
	{
		return Vector4<_Ty>(z, x, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zxw0() const
	{
		return Vector4<_Ty>(z, x, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zyx0() const
	{
		return Vector4<_Ty>(z, y, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zyw0() const
	{
		return Vector4<_Ty>(z, y, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zwx0() const
	{
		return Vector4<_Ty>(z, w, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zwy0() const
	{
		return Vector4<_Ty>(z, w, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wxy0() const
	{
		return Vector4<_Ty>(w, x, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wxz0() const
	{
		return Vector4<_Ty>(w, x, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wyx0() const
	{
		return Vector4<_Ty>(w, y, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wyz0() const
	{
		return Vector4<_Ty>(w, y, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wzx0() const
	{
		return Vector4<_Ty>(w, z, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wzy0() const
	{
		return Vector4<_Ty>(w, z, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xy01() const
	{
		return Vector4<_Ty>(x, y, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xz01() const
	{
		return Vector4<_Ty>(x, z, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xw01() const
	{
		return Vector4<_Ty>(x, w, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yz01() const
	{
		return Vector4<_Ty>(y, z, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yw01() const
	{
		return Vector4<_Ty>(y, w, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yx01() const
	{
		return Vector4<_Ty>(y, x, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zw01() const
	{
		return Vector4<_Ty>(z, w, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zx01() const
	{
		return Vector4<_Ty>(z, x, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zy01() const
	{
		return Vector4<_Ty>(z, y, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wx01() const
	{
		return Vector4<_Ty>(w, x, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wy01() const
	{
		return Vector4<_Ty>(w, y, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wz01() const
	{
		return Vector4<_Ty>(w, z, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xyz1() const
	{
		return Vector4<_Ty>(x, y, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xyw1() const
	{
		return Vector4<_Ty>(x, y, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xzy1() const
	{
		return Vector4<_Ty>(x, z, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xzw1() const
	{
		return Vector4<_Ty>(x, z, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xwy1() const
	{
		return Vector4<_Ty>(x, w, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xwz1() const
	{
		return Vector4<_Ty>(x, w, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yxz1() const
	{
		return Vector4<_Ty>(y, x, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yxw1() const
	{
		return Vector4<_Ty>(y, x, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yzx1() const
	{
		return Vector4<_Ty>(y, z, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yzw1() const
	{
		return Vector4<_Ty>(y, z, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::ywx1() const
	{
		return Vector4<_Ty>(y, w, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::ywz1() const
	{
		return Vector4<_Ty>(y, w, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zxy1() const
	{
		return Vector4<_Ty>(z, x, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zxw1() const
	{
		return Vector4<_Ty>(z, x, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zyx1() const
	{
		return Vector4<_Ty>(z, y, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zyw1() const
	{
		return Vector4<_Ty>(z, y, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zwx1() const
	{
		return Vector4<_Ty>(z, w, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zwy1() const
	{
		return Vector4<_Ty>(z, w, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wxy1() const
	{
		return Vector4<_Ty>(w, x, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wxz1() const
	{
		return Vector4<_Ty>(w, x, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wyx1() const
	{
		return Vector4<_Ty>(w, y, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wyz1() const
	{
		return Vector4<_Ty>(w, y, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wzx1() const
	{
		return Vector4<_Ty>(w, z, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wzy1() const
	{
		return Vector4<_Ty>(w, z, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xyzw() const
	{
		return Vector4<_Ty>(x, y, z, w);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xywz() const
	{
		return Vector4<_Ty>(x, y, w, z);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xzyw() const
	{
		return Vector4<_Ty>(x, z, y, w);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xzwy() const
	{
		return Vector4<_Ty>(x, z, w, y);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xwyz() const
	{
		return Vector4<_Ty>(x, w, y, z);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::xwzy() const
	{
		return Vector4<_Ty>(x, w, z, y);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yxzw() const
	{
		return Vector4<_Ty>(y, x, z, w);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yxwz() const
	{
		return Vector4<_Ty>(y, x, w, z);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yzxw() const
	{
		return Vector4<_Ty>(y, z, x, w);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::yzwx() const
	{
		return Vector4<_Ty>(y, z, w, x);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::ywxz() const
	{
		return Vector4<_Ty>(y, w, x, z);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::ywzx() const
	{
		return Vector4<_Ty>(y, w, z, x);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zxyw() const
	{
		return Vector4<_Ty>(z, x, y, w);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zxwy() const
	{
		return Vector4<_Ty>(z, x, w, y);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zyxw() const
	{
		return Vector4<_Ty>(z, y, x, w);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zywx() const
	{
		return Vector4<_Ty>(z, y, w, x);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zwxy() const
	{
		return Vector4<_Ty>(z, w, x, y);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::zwyx() const
	{
		return Vector4<_Ty>(z, w, y, x);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wxyz() const
	{
		return Vector4<_Ty>(w, x, y, z);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wxzy() const
	{
		return Vector4<_Ty>(w, x, z, y);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wyxz() const
	{
		return Vector4<_Ty>(w, y, x, z);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wyzx() const
	{
		return Vector4<_Ty>(w, y, z, x);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wzxy() const
	{
		return Vector4<_Ty>(w, z, x, y);
	}

	template<class _Ty>
	inline constexpr Vector4<_Ty> Vector4<_Ty>::wzyx() const
	{
		return Vector4<_Ty>(w, z, y, x);
	}
}
