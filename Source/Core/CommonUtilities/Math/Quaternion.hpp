#pragma once
#include "Vector4.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Quaternion
	{
	public:
		union
		{
#pragma warning(push)
#pragma warning(disable : 4201)
			struct
			{
				_Ty w, x, y, z;
			};
			_Ty Data[4];
#pragma warning(pop)
		};

		// Construct an identity quaternion (1, 0, 0, 0).
		constexpr Quaternion();
		// Constructs a quaternion from the given values.
		constexpr Quaternion(_Ty aW, _Ty aX, _Ty aY, _Ty aZ);
		// Constructs a quaternion from the vector representation.
		constexpr Quaternion(const Vector4<_Ty>& aVector);
		// Construct a quaternion representing rotation around an axis.
		constexpr Quaternion(const Vector3<_Ty>& aAxis, _Ty aAngle);
		// Quaternion Destructor.
		constexpr ~Quaternion() = default;

		// Rotates a vector.
		constexpr Vector3<_Ty> Rotate(const Vector3<_Ty> aVector) const;
		// Gets a vector pointing in the forwards direction of the quaternion. Will be normalized if quaternion is normalized.
		constexpr Vector3<_Ty> Forward() const;
		// Gets a vector pointing in the upwards direction of the quaternion. Will be normalized if quaternion is normalized.
		constexpr Vector3<_Ty> Up() const;
		// Gets a vector pointing in the right direction of the quaternion. Will be normalized if quaternion is normalized.
		constexpr Vector3<_Ty> Right() const;

		// Gets the dot product of this quaternion and another.
		constexpr _Ty Dot(const Quaternion<_Ty>& aRHS) const;

		// Normalizes this quaternion.
		constexpr void Normalize();
		// Gets a normalized quaternion with same rotation.
		constexpr Quaternion<_Ty> GetNormalized() const;
		// Gets the conjugate of the quaternion.
		constexpr Quaternion<_Ty> Conjugate() const;
		// Gets the inverse of the quaternion.
		constexpr Quaternion<_Ty> Inverse() const;

		// Gets the magnitude of the quaternion
		constexpr _Ty Length() const;
		// Gets the magnitude squared of the quaternion. Is faster than Length().
		constexpr _Ty LengthSqr() const;

		constexpr bool operator==(const Quaternion<_Ty>& aRhs) const;
		constexpr bool operator!=(const Quaternion<_Ty>& aRhs) const;

		constexpr Quaternion<_Ty>& operator+=(const Quaternion<_Ty>& aRhs);
		constexpr Quaternion<_Ty> operator+(const Quaternion<_Ty>& aRhs) const;
		constexpr Quaternion<_Ty>& operator-=(const Quaternion<_Ty>& aRhs);
		constexpr Quaternion<_Ty> operator-(const Quaternion<_Ty>& aRhs) const;

		constexpr Quaternion<_Ty> operator-() const;

		constexpr Quaternion<_Ty>& operator*=(const Quaternion<_Ty>& aRhs);
		constexpr Quaternion<_Ty> operator*(const Quaternion<_Ty>& aRhs) const;
		constexpr Quaternion<_Ty>& operator*=(const _Ty& aScalar);
		constexpr Quaternion<_Ty> operator*(const _Ty& aScalar) const;
		constexpr Quaternion<_Ty>& operator/=(const _Ty& aScalar);
		constexpr Quaternion<_Ty> operator/(const _Ty& aScalar) const;
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;

	template<typename _Ty>
	constexpr static Quaternion<_Ty> operator*(_Ty aScalar, Quaternion<_Ty> aQuaternion);
	template<typename _Ty>
	constexpr static Quaternion<_Ty> operator/(_Ty aScalar, Quaternion<_Ty> aQuaternion);

	template<typename _Ty>
	inline constexpr Quaternion<_Ty>::Quaternion() :
		w(static_cast<_Ty>(1)),
		x(static_cast<_Ty>(0)),
		y(static_cast<_Ty>(0)),
		z(static_cast<_Ty>(0))
	{
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty>::Quaternion(_Ty aW, _Ty aX, _Ty aY, _Ty aZ) :
		w(aW),
		x(aX),
		y(aY),
		z(aZ)
	{
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty>::Quaternion(const Vector4<_Ty>& aVector) :
		x(aVector.x),
		y(aVector.y),
		z(aVector.z),
		w(aVector.w)
	{
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty>::Quaternion(const Vector3<_Ty>& aAxis, _Ty aAngle)
	{
		const _Ty a = aAngle / static_cast<_Ty>(2);
		const _Ty sin_a = std::sin(a);
		w = std::cos(a);
		x = aAxis.x * sin_a;
		y = aAxis.y * sin_a;
		z = aAxis.z * sin_a;
		Normalize();
	}
	template<typename _Ty>
	inline constexpr Vector3<_Ty> Quaternion<_Ty>::Rotate(const Vector3<_Ty> v) const
	{
		Vector3<float> u(x, y, z);

		// Extract the scalar part of the quaternion
		_Ty s = w;

		// Do the math
		return 2.0f * u.Dot(v) * u
			+ (s * s - u.Dot(u)) * v
			+ 2.0f * s * u.Cross(v);
	}
	template<typename _Ty>
	inline constexpr Vector3<_Ty> Quaternion<_Ty>::Forward() const
	{
		return Rotate(Vector3<_Ty>(static_cast<_Ty>(0), static_cast<_Ty>(0), static_cast<_Ty>(1)));
	}
	template<typename _Ty>
	inline constexpr Vector3<_Ty> Quaternion<_Ty>::Up() const
	{
		return Rotate(Vector3<_Ty>(static_cast<_Ty>(0), static_cast<_Ty>(1), static_cast<_Ty>(0)));
	}
	template<typename _Ty>
	inline constexpr Vector3<_Ty> Quaternion<_Ty>::Right() const
	{
		return Rotate(Vector3<_Ty>(static_cast<_Ty>(1), static_cast<_Ty>(0), static_cast<_Ty>(0)));
	}
	template<typename _Ty>
	inline constexpr _Ty Quaternion<_Ty>::Dot(const Quaternion<_Ty>& aRHS) const
	{
		return w * aRHS.w
			+ x * aRHS.x
			+ y * aRHS.y
			+ z * aRHS.z;
	}
	template<typename _Ty>
	inline constexpr void Quaternion<_Ty>::Normalize()
	{
		_Ty length = Length();
		w /= length;
		x /= length;
		y /= length;
		z /= length;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> Quaternion<_Ty>::GetNormalized() const
	{
		Quaternion<_Ty> q(*this);
		q.Normalize();
		return q;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> Quaternion<_Ty>::Conjugate() const
	{
		return Quaternion<_Ty>(w, -x, -y, -z);
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> Quaternion<_Ty>::Inverse() const
	{
		return Conjugate() / Length();
	}
	template<typename _Ty>
	inline constexpr _Ty Quaternion<_Ty>::Length() const
	{
		return std::sqrt(LengthSqr());
	}
	template<typename _Ty>
	inline constexpr _Ty Quaternion<_Ty>::LengthSqr() const
	{
		return w * w +
			x * x +
			y * y +
			z * z;
	}

#pragma region Operators
	template<typename _Ty>
	inline constexpr bool Quaternion<_Ty>::operator==(const Quaternion<_Ty>& aRhs) const
	{
		return w == aRhs.w &&
			x == aRhs.x &&
			y == aRhs.y &&
			z == aRhs.z;
	}
	template<typename _Ty>
	inline constexpr bool Quaternion<_Ty>::operator!=(const Quaternion<_Ty>& aRhs) const
	{
		return w != aRhs.w ||
			x != aRhs.x ||
			y != aRhs.y ||
			z != aRhs.z;
	}


	template<typename _Ty>
	inline constexpr Quaternion<_Ty>& Quaternion<_Ty>::operator+=(const Quaternion<_Ty>& aRhs)
	{
		w += aRhs.w;
		x += aRhs.x;
		y += aRhs.y;
		z += aRhs.z;
		return *this;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> Quaternion<_Ty>::operator+(const Quaternion<_Ty>& aRhs) const
	{
		return Quaternion<_Ty>(*this) += aRhs;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty>& Quaternion<_Ty>::operator-=(const Quaternion<_Ty>& aRhs)
	{
		w -= aRhs.w;
		x -= aRhs.x;
		y -= aRhs.y;
		z -= aRhs.z;
		return *this;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> Quaternion<_Ty>::operator-(const Quaternion<_Ty>& aRhs) const
	{
		return Quaternion<_Ty>(*this) -= aRhs;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> Quaternion<_Ty>::operator-() const
	{
		return Quaternion<_Ty>(-w, -x, -y, -z);
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty>& Quaternion<_Ty>::operator*=(const Quaternion<_Ty>& aRhs)
	{
		*this = (*this) * aRhs;
		return *this;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> Quaternion<_Ty>::operator*(const Quaternion<_Ty>& aRhs) const
	{
		Quaternion<_Ty> q;
		q.w = w * aRhs.w - x * aRhs.x - y * aRhs.y - z * aRhs.z;
		q.x = w * aRhs.x + x * aRhs.w + y * aRhs.z - z * aRhs.y;
		q.y = w * aRhs.y - x * aRhs.z + y * aRhs.w + z * aRhs.x;
		q.z = w * aRhs.z + x * aRhs.y - y * aRhs.x + z * aRhs.w;
		return q;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty>& Quaternion<_Ty>::operator*=(const _Ty& aScalar)
	{
		w *= aScalar;
		x *= aScalar;
		y *= aScalar;
		z *= aScalar;
		return *this;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> Quaternion<_Ty>::operator*(const _Ty& aScalar) const
	{
		return Quaternion<_Ty>(
			this->w * aScalar,
			this->x * aScalar,
			this->y * aScalar,
			this->z * aScalar
		);
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty>& Quaternion<_Ty>::operator/=(const _Ty& aScalar)
	{
		w /= aScalar;
		x /= aScalar;
		y /= aScalar;
		z /= aScalar;
		return *this;
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> Quaternion<_Ty>::operator/(const _Ty& aScalar) const
	{
		return Quaternion<_Ty>(
			this->w / aScalar,
			this->x / aScalar,
			this->y / aScalar,
			this->z / aScalar
		);
	}

	template<typename _Ty>
	inline constexpr Quaternion<_Ty> operator*(_Ty aScalar, Quaternion<_Ty> aQuaternion)
	{
		return Quaternion<_Ty>(
			aQuaternion.w * aScalar,
			aQuaternion.x * aScalar,
			aQuaternion.y * aScalar,
			aQuaternion.z * aScalar
		);
	}
	template<typename _Ty>
	inline constexpr Quaternion<_Ty> operator/(_Ty aScalar, Quaternion<_Ty> aQuaternion)
	{
		return Quaternion<_Ty>(
			aQuaternion.w / aScalar,
			aQuaternion.x / aScalar,
			aQuaternion.y / aScalar,
			aQuaternion.z / aScalar
		);
	}
#pragma endregion
}