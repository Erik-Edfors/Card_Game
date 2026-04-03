#pragma once
#include <cmath>

namespace CommonUtilities
{
	template <class _Ty>
	class Vector2;
	template <class _Ty>
	class Vector3;

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
		Vector4<_Ty>(const _Ty& aX, const _Ty& aY, const _Ty& aZ, const _Ty& aW);
		//Creates a vector (aValues[0], aValues[1], aValues[2], aValues[3])
		Vector4<_Ty>(const _Ty aValues[4]);
		//Creates a zero-vector
		Vector4<_Ty>();
		//Copy constructor (compiler generated)
		Vector4<_Ty>(const Vector4<_Ty>& aVector4) = default;
		//Explicit conversion constructor from other vector type.
		template<class _VecTy>
		explicit Vector4<_Ty>(const _VecTy& aVector);
		//Assignment operator (compiler generated)
		Vector4<_Ty>& operator=(const Vector4<_Ty>& aVector4) = default;
		//Destructor (compiler generated)
		~Vector4<_Ty>() = default;
		//Explicit Type operator, create a different vector with the same values.
		//Example creates a Tga::Vector4<_Ty> from this CommonUtillities::Vector4<_Ty>
		template <class OtherVectorClass>
		explicit operator OtherVectorClass() const;
		//Returns a negated copy of the vector
		Vector4<_Ty> operator-() const;
		//Returns the squared length of the vector
		_Ty LengthSqr() const;
		//Returns the length of the vector
		_Ty Length() const;
		//Returns a normalized copy of this vector
		Vector4<_Ty> GetNormalized() const;
		//Normalizes the vector
		void Normalize();
		//Returns the dot product of this and aVector
		_Ty Dot(const Vector4<_Ty>& aVector) const;


		Vector2<_Ty> xy() const;
		Vector2<_Ty> xz() const;
		Vector2<_Ty> xw() const;

		Vector2<_Ty> yz() const;
		Vector2<_Ty> yw() const;
		Vector2<_Ty> yx() const;

		Vector2<_Ty> zw() const;
		Vector2<_Ty> zx() const;
		Vector2<_Ty> zy() const;

		Vector2<_Ty> wx() const;
		Vector2<_Ty> wy() const;
		Vector2<_Ty> wz() const;

		Vector3<_Ty> xyz() const;
		Vector3<_Ty> xyw() const;
		Vector3<_Ty> xzy() const;
		Vector3<_Ty> xzw() const;
		Vector3<_Ty> xwy() const;
		Vector3<_Ty> xwz() const;

		Vector3<_Ty> yxz() const;
		Vector3<_Ty> yxw() const;
		Vector3<_Ty> yzx() const;
		Vector3<_Ty> yzw() const;
		Vector3<_Ty> ywx() const;
		Vector3<_Ty> ywz() const;

		Vector3<_Ty> zxy() const;
		Vector3<_Ty> zxw() const;
		Vector3<_Ty> zyx() const;
		Vector3<_Ty> zyw() const;
		Vector3<_Ty> zwx() const;
		Vector3<_Ty> zwy() const;

		Vector3<_Ty> wxy() const;
		Vector3<_Ty> wxz() const;
		Vector3<_Ty> wyx() const;
		Vector3<_Ty> wyz() const;
		Vector3<_Ty> wzx() const;
		Vector3<_Ty> wzy() const;

		Vector3<_Ty> xy0() const;
		Vector3<_Ty> xz0() const;
		Vector3<_Ty> xw0() const;

		Vector3<_Ty> yz0() const;
		Vector3<_Ty> yw0() const;
		Vector3<_Ty> yx0() const;

		Vector3<_Ty> zw0() const;
		Vector3<_Ty> zx0() const;
		Vector3<_Ty> zy0() const;

		Vector3<_Ty> wx0() const;
		Vector3<_Ty> wy0() const;
		Vector3<_Ty> wz0() const;

		Vector3<_Ty> xy1() const;
		Vector3<_Ty> xz1() const;
		Vector3<_Ty> xw1() const;

		Vector3<_Ty> yz1() const;
		Vector3<_Ty> yw1() const;
		Vector3<_Ty> yx1() const;

		Vector3<_Ty> zw1() const;
		Vector3<_Ty> zx1() const;
		Vector3<_Ty> zy1() const;

		Vector3<_Ty> wx1() const;
		Vector3<_Ty> wy1() const;
		Vector3<_Ty> wz1() const;

		Vector4<_Ty> xy00() const;
		Vector4<_Ty> xz00() const;
		Vector4<_Ty> xw00() const;

		Vector4<_Ty> yz00() const;
		Vector4<_Ty> yw00() const;
		Vector4<_Ty> yx00() const;

		Vector4<_Ty> zw00() const;
		Vector4<_Ty> zx00() const;
		Vector4<_Ty> zy00() const;

		Vector4<_Ty> wx00() const;
		Vector4<_Ty> wy00() const;
		Vector4<_Ty> wz00() const;

		Vector4<_Ty> xyz0() const;
		Vector4<_Ty> xyw0() const;
		Vector4<_Ty> xzy0() const;
		Vector4<_Ty> xzw0() const;
		Vector4<_Ty> xwy0() const;
		Vector4<_Ty> xwz0() const;

		Vector4<_Ty> yxz0() const;
		Vector4<_Ty> yxw0() const;
		Vector4<_Ty> yzx0() const;
		Vector4<_Ty> yzw0() const;
		Vector4<_Ty> ywx0() const;
		Vector4<_Ty> ywz0() const;

		Vector4<_Ty> zxy0() const;
		Vector4<_Ty> zxw0() const;
		Vector4<_Ty> zyx0() const;
		Vector4<_Ty> zyw0() const;
		Vector4<_Ty> zwx0() const;
		Vector4<_Ty> zwy0() const;

		Vector4<_Ty> wxy0() const;
		Vector4<_Ty> wxz0() const;
		Vector4<_Ty> wyx0() const;
		Vector4<_Ty> wyz0() const;
		Vector4<_Ty> wzx0() const;
		Vector4<_Ty> wzy0() const;

		Vector4<_Ty> xy01() const;
		Vector4<_Ty> xz01() const;
		Vector4<_Ty> xw01() const;

		Vector4<_Ty> yz01() const;
		Vector4<_Ty> yw01() const;
		Vector4<_Ty> yx01() const;

		Vector4<_Ty> zw01() const;
		Vector4<_Ty> zx01() const;
		Vector4<_Ty> zy01() const;

		Vector4<_Ty> wx01() const;
		Vector4<_Ty> wy01() const;
		Vector4<_Ty> wz01() const;

		Vector4<_Ty> xyz1() const;
		Vector4<_Ty> xyw1() const;
		Vector4<_Ty> xzy1() const;
		Vector4<_Ty> xzw1() const;
		Vector4<_Ty> xwy1() const;
		Vector4<_Ty> xwz1() const;

		Vector4<_Ty> yxz1() const;
		Vector4<_Ty> yxw1() const;
		Vector4<_Ty> yzx1() const;
		Vector4<_Ty> yzw1() const;
		Vector4<_Ty> ywx1() const;
		Vector4<_Ty> ywz1() const;

		Vector4<_Ty> zxy1() const;
		Vector4<_Ty> zxw1() const;
		Vector4<_Ty> zyx1() const;
		Vector4<_Ty> zyw1() const;
		Vector4<_Ty> zwx1() const;
		Vector4<_Ty> zwy1() const;

		Vector4<_Ty> wxy1() const;
		Vector4<_Ty> wxz1() const;
		Vector4<_Ty> wyx1() const;
		Vector4<_Ty> wyz1() const;
		Vector4<_Ty> wzx1() const;
		Vector4<_Ty> wzy1() const;

		Vector4<_Ty> xyzw() const;
		Vector4<_Ty> xywz() const;
		Vector4<_Ty> xzyw() const;
		Vector4<_Ty> xzwy() const;
		Vector4<_Ty> xwyz() const;
		Vector4<_Ty> xwzy() const;

		Vector4<_Ty> yxzw() const;
		Vector4<_Ty> yxwz() const;
		Vector4<_Ty> yzxw() const;
		Vector4<_Ty> yzwx() const;
		Vector4<_Ty> ywxz() const;
		Vector4<_Ty> ywzx() const;

		Vector4<_Ty> zxyw() const;
		Vector4<_Ty> zxwy() const;
		Vector4<_Ty> zyxw() const;
		Vector4<_Ty> zywx() const;
		Vector4<_Ty> zwxy() const;
		Vector4<_Ty> zwyx() const;

		Vector4<_Ty> wxyz() const;
		Vector4<_Ty> wxzy() const;
		Vector4<_Ty> wyxz() const;
		Vector4<_Ty> wyzx() const;
		Vector4<_Ty> wzxy() const;
		Vector4<_Ty> wzyx() const;
	};

	using Vector4f = Vector4<float>;
	using Vector4d = Vector4<double>;
	using Vector4i = Vector4<int>;
	using Vector4ui = Vector4<unsigned int>;

	template<class _Ty>
	inline Vector4<_Ty>::Vector4(const _Ty& aX, const _Ty& aY, const _Ty& aZ, const _Ty& aW)
	{
		this->x = aX;
		this->y = aY;
		this->z = aZ;
		this->w = aW;
	}
	template<class _Ty>
	inline Vector4<_Ty>::Vector4(const _Ty aValues[4])
	{
		this->x = aValues[0];
		this->y = aValues[1];
		this->z = aValues[2];
		this->w = aValues[3];
	}
	template<class _Ty>
	inline Vector4<_Ty>::Vector4() : Vector4<_Ty>(0, 0, 0, 0)
	{
	}
	template<class _Ty>
	template<class _VecTy>
	inline Vector4<_Ty>::Vector4(const _VecTy& aVector) : 
		x(static_cast<_Ty>(aVector.x)),
		y(static_cast<_Ty>(aVector.y)),
		z(static_cast<_Ty>(aVector.z)),
		w(static_cast<_Ty>(aVector.w))
	{
	}
	template<class _Ty>
	template<class OtherVectorClass>
	inline Vector4<_Ty>::operator OtherVectorClass() const
	{
		return OtherVectorClass(this->x, this->y, this->z, this->w);
	}
	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::operator-() const
	{
		return Vector4<_Ty>(-this->x, -this->y, -this->z, -this->w);
	}
	template<class _Ty>
	inline _Ty Vector4<_Ty>::LengthSqr() const
	{
		return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
	}
	template<class _Ty>
	inline _Ty Vector4<_Ty>::Length() const
	{
		return static_cast<_Ty>(std::sqrt(LengthSqr()));
	}
	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::GetNormalized() const
	{
		Vector4<_Ty> res = *this;
		res.Normalize();
		return res;
	}
	template<class _Ty>
	inline void Vector4<_Ty>::Normalize()
	{
		_Ty len = Length();
		this->x /= len;
		this->y /= len;
		this->z /= len;
		this->w /= len;
	}
	template<class _Ty>
	inline _Ty Vector4<_Ty>::Dot(const Vector4<_Ty>& aVector) const
	{
		return this->x * aVector.x + this->y * aVector.y + this->z * aVector.z + this->w * aVector.w;
	}
	//Returns the vector sum of aVector0 and aVector1
	template <class _Ty>
	Vector4<_Ty> operator+(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
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
	Vector4<_Ty> operator-(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
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
	Vector4<_Ty> operator*(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		return Vector4<_Ty>(
			aVector0.x * aVector1.x,
			aVector0.y * aVector1.y,
			aVector0.z * aVector1.z,
			aVector0.w * aVector1.w
		);
	}
	template <class _Ty>
	Vector4<_Ty> operator/(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
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
	Vector4<_Ty> operator*(const Vector4<_Ty>& aVector, const _Ty& aScalar)
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
	Vector4<_Ty> operator*(const _Ty& aScalar, const Vector4<_Ty>& aVector)
	{
		return aVector * aScalar;
	}
	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class _Ty>
	Vector4<_Ty> operator/(const Vector4<_Ty>& aVector, const _Ty& aScalar)
	{
		return Vector4<_Ty>(
			aVector.x / aScalar,
			aVector.y / aScalar,
			aVector.z / aScalar,
			aVector.w / aScalar
		);
	}

	template <class _Ty>
	Vector4<_Ty> operator/(const _Ty& aScalar, const Vector4<_Ty>& aVector)
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
	Vector4<_Ty>& operator+=(Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		aVector0.w += aVector1.w;
		return aVector0;
	}
	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class _Ty>
	Vector4<_Ty>& operator-=(Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		aVector0.w -= aVector1.w;
		return aVector0;
	}
	template <class _Ty>
	Vector4<_Ty>& operator*=(Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		aVector0.x *= aVector1.x;
		aVector0.y *= aVector1.y;
		aVector0.z *= aVector1.z;
		aVector0.w *= aVector1.w;
		return aVector0;
	}
	template <class _Ty>
	Vector4<_Ty>& operator/=(Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		aVector0.x /= aVector1.x;
		aVector0.y /= aVector1.y;
		aVector0.z /= aVector1.z;
		aVector0.w /= aVector1.w;
		return aVector0;
	}
	//Equivalent to setting aVector to (aVector * aScalar)
	template <class _Ty>
	Vector4<_Ty>& operator*=(Vector4<_Ty>& aVector, const _Ty& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
		aVector.w *= aScalar;
		return aVector;
	}
	//Equivalent to setting aVector to (aVector / aScalar)
	template <class _Ty>
	Vector4<_Ty>& operator/=(Vector4<_Ty>& aVector, const _Ty& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
		aVector.w /= aScalar;
		return aVector;
	}

	template <class _Ty>
	bool operator==(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		return aVector0.x == aVector1.x && aVector0.y == aVector1.y && aVector0.z == aVector1.z && aVector0.w == aVector1.w;
	}
	template <class _Ty>
	bool operator!=(const Vector4<_Ty>& aVector0, const Vector4<_Ty>& aVector1)
	{
		return aVector0.x != aVector1.x || aVector0.y != aVector1.y || aVector0.z != aVector1.z || aVector0.w != aVector1.w;
	}



	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::xy() const
	{
		return Vector2<_Ty>(x, y);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::xz() const
	{
		return Vector2<_Ty>(x, z);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::xw() const
	{
		return Vector2<_Ty>(x, w);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::yz() const
	{
		return Vector2<_Ty>(y, z);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::yw() const
	{
		return Vector2<_Ty>(y, w);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::yx() const
	{
		return Vector2<_Ty>(y, x);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::zw() const
	{
		return Vector2<_Ty>(z, w);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::zx() const
	{
		return Vector2<_Ty>(z, x);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::zy() const
	{
		return Vector2<_Ty>(z, y);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::wx() const
	{
		return Vector2<_Ty>(w, x);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::wy() const
	{
		return Vector2<_Ty>(w, y);
	}

	template<class _Ty>
	inline Vector2<_Ty> Vector4<_Ty>::wz() const
	{
		return Vector2<_Ty>(w, z);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xyz() const
	{
		return Vector3<_Ty>(x, y, z);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xyw() const
	{
		return Vector3<_Ty>(x, y, w);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xzy() const
	{
		return Vector3<_Ty>(x, z, y);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xzw() const
	{
		return Vector3<_Ty>(x, z, w);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xwy() const
	{
		return Vector3<_Ty>(x, w, y);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xwz() const
	{
		return Vector3<_Ty>(x, w, z);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yxz() const
	{
		return Vector3<_Ty>(y, x, z);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yxw() const
	{
		return Vector3<_Ty>(y, x, w);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yzx() const
	{
		return Vector3<_Ty>(y, z, x);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yzw() const
	{
		return Vector3<_Ty>(y, z, w);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::ywx() const
	{
		return Vector3<_Ty>(y, w, x);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::ywz() const
	{
		return Vector3<_Ty>(y, w, z);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zxy() const
	{
		return Vector3<_Ty>(z, x, y);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zxw() const
	{
		return Vector3<_Ty>(z, x, w);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zyx() const
	{
		return Vector3<_Ty>(z, y, x);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zyw() const
	{
		return Vector3<_Ty>(z, y, w);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zwx() const
	{
		return Vector3<_Ty>(z, w, x);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zwy() const
	{
		return Vector3<_Ty>(z, w, y);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wxy() const
	{
		return Vector3<_Ty>(w, x, y);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wxz() const
	{
		return Vector3<_Ty>(w, x, z);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wyx() const
	{
		return Vector3<_Ty>(w, y, x);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wyz() const
	{
		return Vector3<_Ty>(w, y, z);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wzx() const
	{
		return Vector3<_Ty>(w, z, x);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wzy() const
	{
		return Vector3<_Ty>(w, z, y);
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xy0() const
	{
		return Vector3<_Ty>(x, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xz0() const
	{
		return Vector3<_Ty>(x, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xw0() const
	{
		return Vector3<_Ty>(x, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yz0() const
	{
		return Vector3<_Ty>(y, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yw0() const
	{
		return Vector3<_Ty>(y, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yx0() const
	{
		return Vector3<_Ty>(y, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zw0() const
	{
		return Vector3<_Ty>(z, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zx0() const
	{
		return Vector3<_Ty>(z, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zy0() const
	{
		return Vector3<_Ty>(z, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wx0() const
	{
		return Vector3<_Ty>(w, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wy0() const
	{
		return Vector3<_Ty>(w, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wz0() const
	{
		return Vector3<_Ty>(w, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xy1() const
	{
		return Vector3<_Ty>(x, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xz1() const
	{
		return Vector3<_Ty>(x, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::xw1() const
	{
		return Vector3<_Ty>(x, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yz1() const
	{
		return Vector3<_Ty>(y, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yw1() const
	{
		return Vector3<_Ty>(y, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::yx1() const
	{
		return Vector3<_Ty>(y, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zw1() const
	{
		return Vector3<_Ty>(z, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zx1() const
	{
		return Vector3<_Ty>(z, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::zy1() const
	{
		return Vector3<_Ty>(z, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wx1() const
	{
		return Vector3<_Ty>(w, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wy1() const
	{
		return Vector3<_Ty>(w, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector3<_Ty> Vector4<_Ty>::wz1() const
	{
		return Vector3<_Ty>(w, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xy00() const
	{
		return Vector4<_Ty>(x, y, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xz00() const
	{
		return Vector4<_Ty>(x, z, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xw00() const
	{
		return Vector4<_Ty>(x, w, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yz00() const
	{
		return Vector4<_Ty>(y, z, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yw00() const
	{
		return Vector4<_Ty>(y, w, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yx00() const
	{
		return Vector4<_Ty>(y, x, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zw00() const
	{
		return Vector4<_Ty>(z, w, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zx00() const
	{
		return Vector4<_Ty>(z, x, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zy00() const
	{
		return Vector4<_Ty>(z, y, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wx00() const
	{
		return Vector4<_Ty>(w, x, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wy00() const
	{
		return Vector4<_Ty>(w, y, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wz00() const
	{
		return Vector4<_Ty>(w, z, static_cast<_Ty>(0), static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xyz0() const
	{
		return Vector4<_Ty>(x, y, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xyw0() const
	{
		return Vector4<_Ty>(x, y, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xzy0() const
	{
		return Vector4<_Ty>(x, z, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xzw0() const
	{
		return Vector4<_Ty>(x, z, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xwy0() const
	{
		return Vector4<_Ty>(x, w, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xwz0() const
	{
		return Vector4<_Ty>(x, w, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yxz0() const
	{
		return Vector4<_Ty>(y, x, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yxw0() const
	{
		return Vector4<_Ty>(y, x, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yzx0() const
	{
		return Vector4<_Ty>(y, z, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yzw0() const
	{
		return Vector4<_Ty>(y, z, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::ywx0() const
	{
		return Vector4<_Ty>(y, w, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::ywz0() const
	{
		return Vector4<_Ty>(y, w, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zxy0() const
	{
		return Vector4<_Ty>(z, x, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zxw0() const
	{
		return Vector4<_Ty>(z, x, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zyx0() const
	{
		return Vector4<_Ty>(z, y, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zyw0() const
	{
		return Vector4<_Ty>(z, y, w, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zwx0() const
	{
		return Vector4<_Ty>(z, w, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zwy0() const
	{
		return Vector4<_Ty>(z, w, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wxy0() const
	{
		return Vector4<_Ty>(w, x, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wxz0() const
	{
		return Vector4<_Ty>(w, x, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wyx0() const
	{
		return Vector4<_Ty>(w, y, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wyz0() const
	{
		return Vector4<_Ty>(w, y, z, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wzx0() const
	{
		return Vector4<_Ty>(w, z, x, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wzy0() const
	{
		return Vector4<_Ty>(w, z, y, static_cast<_Ty>(0));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xy01() const
	{
		return Vector4<_Ty>(x, y, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xz01() const
	{
		return Vector4<_Ty>(x, z, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xw01() const
	{
		return Vector4<_Ty>(x, w, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yz01() const
	{
		return Vector4<_Ty>(y, z, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yw01() const
	{
		return Vector4<_Ty>(y, w, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yx01() const
	{
		return Vector4<_Ty>(y, x, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zw01() const
	{
		return Vector4<_Ty>(z, w, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zx01() const
	{
		return Vector4<_Ty>(z, x, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zy01() const
	{
		return Vector4<_Ty>(z, y, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wx01() const
	{
		return Vector4<_Ty>(w, x, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wy01() const
	{
		return Vector4<_Ty>(w, y, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wz01() const
	{
		return Vector4<_Ty>(w, z, static_cast<_Ty>(0), static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xyz1() const
	{
		return Vector4<_Ty>(x, y, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xyw1() const
	{
		return Vector4<_Ty>(x, y, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xzy1() const
	{
		return Vector4<_Ty>(x, z, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xzw1() const
	{
		return Vector4<_Ty>(x, z, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xwy1() const
	{
		return Vector4<_Ty>(x, w, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xwz1() const
	{
		return Vector4<_Ty>(x, w, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yxz1() const
	{
		return Vector4<_Ty>(y, x, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yxw1() const
	{
		return Vector4<_Ty>(y, x, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yzx1() const
	{
		return Vector4<_Ty>(y, z, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yzw1() const
	{
		return Vector4<_Ty>(y, z, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::ywx1() const
	{
		return Vector4<_Ty>(y, w, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::ywz1() const
	{
		return Vector4<_Ty>(y, w, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zxy1() const
	{
		return Vector4<_Ty>(z, x, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zxw1() const
	{
		return Vector4<_Ty>(z, x, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zyx1() const
	{
		return Vector4<_Ty>(z, y, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zyw1() const
	{
		return Vector4<_Ty>(z, y, w, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zwx1() const
	{
		return Vector4<_Ty>(z, w, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zwy1() const
	{
		return Vector4<_Ty>(z, w, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wxy1() const
	{
		return Vector4<_Ty>(w, x, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wxz1() const
	{
		return Vector4<_Ty>(w, x, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wyx1() const
	{
		return Vector4<_Ty>(w, y, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wyz1() const
	{
		return Vector4<_Ty>(w, y, z, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wzx1() const
	{
		return Vector4<_Ty>(w, z, x, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wzy1() const
	{
		return Vector4<_Ty>(w, z, y, static_cast<_Ty>(1));
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xyzw() const
	{
		return Vector4<_Ty>(x, y, z, w);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xywz() const
	{
		return Vector4<_Ty>(x, y, w, z);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xzyw() const
	{
		return Vector4<_Ty>(x, z, y, w);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xzwy() const
	{
		return Vector4<_Ty>(x, z, w, y);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xwyz() const
	{
		return Vector4<_Ty>(x, w, y, z);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::xwzy() const
	{
		return Vector4<_Ty>(x, w, z, y);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yxzw() const
	{
		return Vector4<_Ty>(y, x, z, w);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yxwz() const
	{
		return Vector4<_Ty>(y, x, w, z);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yzxw() const
	{
		return Vector4<_Ty>(y, z, x, w);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::yzwx() const
	{
		return Vector4<_Ty>(y, z, w, x);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::ywxz() const
	{
		return Vector4<_Ty>(y, w, x, z);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::ywzx() const
	{
		return Vector4<_Ty>(y, w, z, x);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zxyw() const
	{
		return Vector4<_Ty>(z, x, y, w);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zxwy() const
	{
		return Vector4<_Ty>(z, x, w, y);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zyxw() const
	{
		return Vector4<_Ty>(z, y, x, w);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zywx() const
	{
		return Vector4<_Ty>(z, y, w, x);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zwxy() const
	{
		return Vector4<_Ty>(z, w, x, y);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::zwyx() const
	{
		return Vector4<_Ty>(z, w, y, x);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wxyz() const
	{
		return Vector4<_Ty>(w, x, y, z);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wxzy() const
	{
		return Vector4<_Ty>(w, x, z, y);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wyxz() const
	{
		return Vector4<_Ty>(w, y, x, z);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wyzx() const
	{
		return Vector4<_Ty>(w, y, z, x);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wzxy() const
	{
		return Vector4<_Ty>(w, z, x, y);
	}

	template<class _Ty>
	inline Vector4<_Ty> Vector4<_Ty>::wzyx() const
	{
		return Vector4<_Ty>(w, z, y, x);
	}
}
