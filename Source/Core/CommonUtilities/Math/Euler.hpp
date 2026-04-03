#pragma once
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix3x3.hpp"
#include "Matrix4x4.hpp"
#include "Constants.h"

#ifndef DEFAULT_ROTATION_ORDER
#define DEFAULT_ROTATION_ORDER RotationOrder::xyz
#endif // !DEFAULT_ROTATION_ORDER

namespace CommonUtilities
{
	enum class RotationOrder : unsigned char
	{
		xyz = 0,
		xzy = 1,

		yxz = 2,
		yzx = 3,

		zxy = 4,
		zyx = 5
	};

	template<typename _Ty>
	struct Euler
	{
		Vector3<_Ty> Angles;
		RotationOrder Order;
	};

	template<typename _Ty>
	Euler<_Ty> ToEuler(const Quaternion<_Ty>& aQuaternion, RotationOrder aRotationOrder = DEFAULT_ROTATION_ORDER);
	template<typename _Ty>
	Euler<_Ty> ToEulerXYZ(const Quaternion<_Ty>& aQuaternion);
	template<typename _Ty>
	Euler<_Ty> ToEulerXZY(const Quaternion<_Ty>& aQuaternion);
	template<typename _Ty>
	Euler<_Ty> ToEulerYXZ(const Quaternion<_Ty>& aQuaternion);
	template<typename _Ty>
	Euler<_Ty> ToEulerYZX(const Quaternion<_Ty>& aQuaternion);
	template<typename _Ty>
	Euler<_Ty> ToEulerZXY(const Quaternion<_Ty>& aQuaternion);
	template<typename _Ty>
	Euler<_Ty> ToEulerZYX(const Quaternion<_Ty>& aQuaternion);

	template<typename _Ty>
	Euler<_Ty> ToEuler(const Matrix3x3<_Ty>& aRotationMatrix, RotationOrder aRotationOrder = DEFAULT_ROTATION_ORDER);
	template<typename _Ty>
	Euler<_Ty> ToEulerXYZ(const Matrix3x3<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerXZY(const Matrix3x3<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerYXZ(const Matrix3x3<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerYZX(const Matrix3x3<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerZXY(const Matrix3x3<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerZYX(const Matrix3x3<_Ty>& aRotationMatrix);

	template<typename _Ty>
	Euler<_Ty> ToEuler(const Matrix4x4<_Ty>& aRotationMatrix, RotationOrder aRotationOrder = DEFAULT_ROTATION_ORDER);
	template<typename _Ty>
	Euler<_Ty> ToEulerXYZ(const Matrix4x4<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerXZY(const Matrix4x4<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerYXZ(const Matrix4x4<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerYZX(const Matrix4x4<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerZXY(const Matrix4x4<_Ty>& aRotationMatrix);
	template<typename _Ty>
	Euler<_Ty> ToEulerZYX(const Matrix4x4<_Ty>& aRotationMatrix);

	template<typename _Ty>
	void SetRotation(Quaternion<_Ty>& aQuaternion, const Euler<_Ty>& aEuler);
	template<typename _Ty>
	void SetRotation(Quaternion<_Ty>& aQuaternion, const Vector3<_Ty>& aEulerAngles, RotationOrder aRotationOrder = DEFAULT_ROTATION_ORDER);
	template<typename _Ty>
	void SetRotationXYZ(Quaternion<_Ty>& aQuaternion, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationXZY(Quaternion<_Ty>& aQuaternion, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationYXZ(Quaternion<_Ty>& aQuaternion, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationYZX(Quaternion<_Ty>& aQuaternion, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationZXY(Quaternion<_Ty>& aQuaternion, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationZYX(Quaternion<_Ty>& aQuaternion, const Vector3<_Ty>& aEulerAngles);

	template<typename _Ty>
	void SetRotation(Matrix3x3<_Ty>& aMatrix3x3, const Euler<_Ty>& aEuler);
	template<typename _Ty>
	void SetRotation(Matrix3x3<_Ty>& aMatrix3x3, const Vector3<_Ty>& aEulerAngles, RotationOrder aRotationOrder = DEFAULT_ROTATION_ORDER);
	template<typename _Ty>
	void SetRotationXYZ(Matrix3x3<_Ty>& aMatrix3x3, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationXZY(Matrix3x3<_Ty>& aMatrix3x3, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationYXZ(Matrix3x3<_Ty>& aMatrix3x3, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationYZX(Matrix3x3<_Ty>& aMatrix3x3, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationZXY(Matrix3x3<_Ty>& aMatrix3x3, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationZYX(Matrix3x3<_Ty>& aMatrix3x3, const Vector3<_Ty>& aEulerAngles);

	template<typename _Ty>
	void SetRotation(Matrix4x4<_Ty>& aMatrix4x4, const Euler<_Ty>& aEuler);
	template<typename _Ty>
	void SetRotation(Matrix4x4<_Ty>& aMatrix4x4, const Vector3<_Ty>& aEulerAngles, RotationOrder aRotationOrder = DEFAULT_ROTATION_ORDER);
	template<typename _Ty>
	void SetRotationXYZ(Matrix4x4<_Ty>& aMatrix4x4, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationXZY(Matrix4x4<_Ty>& aMatrix4x4, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationYXZ(Matrix4x4<_Ty>& aMatrix4x4, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationYZX(Matrix4x4<_Ty>& aMatrix4x4, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationZXY(Matrix4x4<_Ty>& aMatrix4x4, const Vector3<_Ty>& aEulerAngles);
	template<typename _Ty>
	void SetRotationZYX(Matrix4x4<_Ty>& aMatrix4x4, const Vector3<_Ty>& aEulerAngles);

#pragma region QuaternionToEuler
	template<typename _Ty>
	inline Euler<_Ty> ToEuler(const Quaternion<_Ty>& aQuaternion, RotationOrder aRotationOrder)
	{
		Matrix3x3<_Ty> m;
		m.Compose(aQuaternion, Vector3<_Ty>(static_cast<_Ty>(1), static_cast<_Ty>(1), static_cast<_Ty>(1)));
		return ToEuler(m, aRotationOrder);
	}
	template<RotationOrder _RotationOrder, typename _Ty>
	inline Euler<_Ty> ToEulerXYZ(const Quaternion<_Ty>& aQuaternion)
	{
		Matrix3x3<_Ty> m;
		m.Compose(aQuaternion, Vector3<_Ty>(static_cast<_Ty>(1), static_cast<_Ty>(1), static_cast<_Ty>(1)));
		return ToEulerXYZ(m);
	}
	template<RotationOrder _RotationOrder, typename _Ty>
	inline Euler<_Ty> ToEulerXZY(const Quaternion<_Ty>& aQuaternion)
	{
		Matrix3x3<_Ty> m;
		m.Compose(aQuaternion, Vector3<_Ty>(static_cast<_Ty>(1), static_cast<_Ty>(1), static_cast<_Ty>(1)));
		return ToEulerXZY(m);
	}
	template<RotationOrder _RotationOrder, typename _Ty>
	inline Euler<_Ty> ToEulerYXZ(const Quaternion<_Ty>& aQuaternion)
	{
		Matrix3x3<_Ty> m;
		m.Compose(aQuaternion, Vector3<_Ty>(static_cast<_Ty>(1), static_cast<_Ty>(1), static_cast<_Ty>(1)));
		return ToEulerYXZ(m);
	}
	template<RotationOrder _RotationOrder, typename _Ty>
	inline Euler<_Ty> ToEulerYZX(const Quaternion<_Ty>& aQuaternion)
	{
		Matrix3x3<_Ty> m;
		m.Compose(aQuaternion, Vector3<_Ty>(static_cast<_Ty>(1), static_cast<_Ty>(1), static_cast<_Ty>(1)));
		return ToEulerYZX(m);
	}
	template<RotationOrder _RotationOrder, typename _Ty>
	inline Euler<_Ty> ToEulerZXY(const Quaternion<_Ty>& aQuaternion)
	{
		Matrix3x3<_Ty> m;
		m.Compose(aQuaternion, Vector3<_Ty>(static_cast<_Ty>(1), static_cast<_Ty>(1), static_cast<_Ty>(1)));
		return ToEulerZXY(m);
	}
	template<RotationOrder _RotationOrder, typename _Ty>
	inline Euler<_Ty> ToEulerZYX(const Quaternion<_Ty>& aQuaternion)
	{
		Matrix3x3<_Ty> m;
		m.Compose(aQuaternion, Vector3<_Ty>(static_cast<_Ty>(1), static_cast<_Ty>(1), static_cast<_Ty>(1)));
		return ToEulerZYX(m);
	}
#pragma endregion

#pragma region Matrix3x3ToEuler
	template<typename _Ty>
	inline Euler<_Ty> ToEuler(const Matrix3x3<_Ty>& aRotationMatrix3x3, RotationOrder aRotationOrder)
	{
		typedef Euler<_Ty>(*toEuler)(const Matrix3x3<_Ty>&);

		static const toEuler constexpr index[6] = {
			&ToEulerXYZ,
			&ToEulerXZY,
			&ToEulerYXZ,
			&ToEulerYZX,
			&ToEulerZXY,
			&ToEulerZYX,
		};

		return (index[static_cast<size_t>(aRotationOrder)])(aRotationMatrix3x3);
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerXYZ(const Matrix3x3<_Ty>& aRotationMatrix3x3)
	{
		Vector3<_Ty> angles;

		angles.y = static_cast<_Ty>(asin(aRotationMatrix3x3(3, 1)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix3x3(3, 1)) < threshold) [[likely]]
		{
			angles.x = atan2(-aRotationMatrix3x3(3, 2), aRotationMatrix3x3(3, 3));
			angles.z = atan2(-aRotationMatrix3x3(2, 1), aRotationMatrix3x3(1, 1));
		}
		else
		{
			angles.x = atan2(aRotationMatrix3x3(2, 3), aRotationMatrix3x3(2, 2));
			angles.z = static_cast<_Ty>(0);
		}
		return Euler<_Ty>{ angles, RotationOrder::xyz };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerXZY(const Matrix3x3<_Ty>& aRotationMatrix3x3)
	{
		Vector3<_Ty> angles;

		angles.z = static_cast<_Ty>(-asin(aRotationMatrix3x3(2, 1)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix3x3(2, 1)) < threshold) [[likely]]
		{
			angles.x = atan2(aRotationMatrix3x3(2, 3), aRotationMatrix3x3(2, 2));
			angles.y = atan2(aRotationMatrix3x3(3, 1), aRotationMatrix3x3(1, 1));
		}
		else
		{
			angles.x = atan2(-aRotationMatrix3x3(3, 2), aRotationMatrix3x3(3, 3));
			angles.y = static_cast<_Ty>(0);
		}
		return Euler<_Ty>{ angles, RotationOrder::xzy };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerYXZ(const Matrix3x3<_Ty>& aRotationMatrix3x3)
	{
		Vector3<_Ty> angles;

		angles.x = static_cast<_Ty>(-asin(aRotationMatrix3x3(3, 2)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix3x3(3, 2)) < threshold) [[likely]]
		{
			angles.y = atan2(aRotationMatrix3x3(3, 1), aRotationMatrix3x3(3, 3));
			angles.z = atan2(aRotationMatrix3x3(1, 2), aRotationMatrix3x3(2, 2));
		}
		else
		{
			angles.y = atan2(-aRotationMatrix3x3(1, 3), aRotationMatrix3x3(1, 1));
		}
		return Euler<_Ty>{ angles, RotationOrder::yxz };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerYZX(const Matrix3x3<_Ty>& aRotationMatrix3x3)
	{
		Vector3<_Ty> angles;

		angles.z = static_cast<_Ty>(asin(aRotationMatrix3x3(1, 2)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix3x3(1, 2)) < threshold) [[likely]]
		{
			angles.x = atan2(-aRotationMatrix3x3(3, 2), aRotationMatrix3x3(2, 2));
			angles.y = atan2(-aRotationMatrix3x3(1, 3), aRotationMatrix3x3(1, 1));
		}
		else
		{
			angles.x = static_cast<_Ty>(0);
			angles.y = atan2(aRotationMatrix3x3(3, 1), aRotationMatrix3x3(3, 3));
		}
		return Euler<_Ty>{ angles, RotationOrder::yzx };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerZXY(const Matrix3x3<_Ty>& aRotationMatrix3x3)
	{
		Vector3<_Ty> angles;

		angles.x = static_cast<_Ty>(asin(aRotationMatrix3x3(2, 3)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix3x3(2, 3)) < threshold) [[likely]]
		{
			angles.y = atan2(-aRotationMatrix3x3(1, 3), aRotationMatrix3x3(3, 3));
			angles.z = atan2(-aRotationMatrix3x3(2, 1), aRotationMatrix3x3(2, 2));
		}
		else
		{
			angles.y = static_cast<_Ty>(0);
			angles.z = atan2(aRotationMatrix3x3(1, 2), aRotationMatrix3x3(1, 1));
		}
		return Euler<_Ty>{ angles, RotationOrder::zxy };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerZYX(const Matrix3x3<_Ty>& aRotationMatrix3x3)
	{
		Vector3<_Ty> angles;

		angles.y = static_cast<_Ty>(-asin(aRotationMatrix3x3(1, 3)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix3x3(1, 3)) < threshold) [[likely]]
		{
			angles.x = atan2(aRotationMatrix3x3(2, 3), aRotationMatrix3x3(3, 3));
			angles.z = atan2(aRotationMatrix3x3(1, 2), aRotationMatrix3x3(1, 1));
		}
		else
		{
			angles.x = static_cast<_Ty>(0);
			angles.z = atan2(-aRotationMatrix3x3(2, 1), aRotationMatrix3x3(2, 2));
		}
		return Euler<_Ty>{ angles, RotationOrder::zyx };
	}
#pragma endregion

#pragma region Matrix4x4ToEuler
	template<typename _Ty>
	inline Euler<_Ty> ToEuler(const Matrix4x4<_Ty>& aRotationMatrix4x4, RotationOrder aRotationOrder)
	{
		typedef Euler<_Ty>(*toEuler)(const Matrix4x4<_Ty>&);

		static const toEuler constexpr index[6] = {
			&ToEulerXYZ,
			&ToEulerXZY,
			&ToEulerYXZ,
			&ToEulerYZX,
			&ToEulerZXY,
			&ToEulerZYX,
		};

		return (index[static_cast<size_t>(aRotationOrder)])(aRotationMatrix4x4);
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerXYZ(const Matrix4x4<_Ty>& aRotationMatrix4x4)
	{
		Vector3<_Ty> angles;

		angles.y = static_cast<_Ty>(asin(aRotationMatrix4x4(3, 1)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix4x4(3, 1)) < threshold) [[likely]]
		{
			angles.x = atan2(-aRotationMatrix4x4(3, 2), aRotationMatrix4x4(3, 3));
			angles.z = atan2(-aRotationMatrix4x4(2, 1), aRotationMatrix4x4(1, 1));
		}
		else
		{
			angles.x = atan2(aRotationMatrix4x4(2, 3), aRotationMatrix4x4(2, 2));
			angles.z = static_cast<_Ty>(0);
		}
		return Euler<_Ty> { angles, RotationOrder::xyz };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerXZY(const Matrix4x4<_Ty>& aRotationMatrix4x4)
	{
		Vector3<_Ty> angles;

		angles.z = static_cast<_Ty>(-asin(aRotationMatrix4x4(2, 1)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix4x4(2, 1)) < threshold) [[likely]]
		{
			angles.x = atan2(aRotationMatrix4x4(2, 3), aRotationMatrix4x4(2, 2));
			angles.y = atan2(aRotationMatrix4x4(3, 1), aRotationMatrix4x4(1, 1));
		}
		else
		{
			angles.x = atan2(-aRotationMatrix4x4(3, 2), aRotationMatrix4x4(3, 3));
			angles.y = static_cast<_Ty>(0);
		}
		return Euler<_Ty> { angles, RotationOrder::xzy };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerYXZ(const Matrix4x4<_Ty>& aRotationMatrix4x4)
	{
		Vector3<_Ty> angles;

		angles.x = static_cast<_Ty>(-asin(aRotationMatrix4x4(3, 2)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix4x4(3, 2)) < threshold) [[likely]]
		{
			angles.y = atan2(aRotationMatrix4x4(3, 1), aRotationMatrix4x4(3, 3));
			angles.z = atan2(aRotationMatrix4x4(1, 2), aRotationMatrix4x4(2, 2));
		}
		else
		{
			angles.y = atan2(-aRotationMatrix4x4(1, 3), aRotationMatrix4x4(1, 1));
			angles.z = static_cast<_Ty>(0);
		}
		return Euler<_Ty> { angles, RotationOrder::yxz };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerYZX(const Matrix4x4<_Ty>& aRotationMatrix4x4)
	{
		Vector3<_Ty> angles;

		angles.z = static_cast<_Ty>(asin(aRotationMatrix4x4(1, 2)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix4x4(1, 2)) < threshold) [[likely]]
		{
			angles.x = atan2(-aRotationMatrix4x4(3, 2), aRotationMatrix4x4(2, 2));
			angles.y = atan2(-aRotationMatrix4x4(1, 3), aRotationMatrix4x4(1, 1));
		}
		else
		{
			angles.x = static_cast<_Ty>(0);
			angles.y = atan2(aRotationMatrix4x4(3, 1), aRotationMatrix4x4(3, 3));
		}
		return Euler<_Ty> { angles, RotationOrder::yzx };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerZXY(const Matrix4x4<_Ty>& aRotationMatrix4x4)
	{
		Vector3<_Ty> angles;

		angles.x = static_cast<_Ty>(asin(aRotationMatrix4x4(2, 3)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix4x4(2, 3)) < threshold) [[likely]]
		{
			angles.y = atan2(-aRotationMatrix4x4(1, 3), aRotationMatrix4x4(3, 3));
			angles.z = atan2(-aRotationMatrix4x4(2, 1), aRotationMatrix4x4(2, 2));
		}
		else
		{
			angles.y = static_cast<_Ty>(0);
			angles.z = atan2(aRotationMatrix4x4(1, 2), aRotationMatrix4x4(1, 1));
		}
		return Euler<_Ty> { angles, RotationOrder::zxy };
	}
	template<typename _Ty>
	inline Euler<_Ty> ToEulerZYX(const Matrix4x4<_Ty>& aRotationMatrix4x4)
	{
		Vector3<_Ty> angles;

		angles.y = static_cast<_Ty>(-asin(aRotationMatrix4x4(1, 3)));

		constexpr _Ty threshold = static_cast<_Ty>(0.9999999);
		if (abs(aRotationMatrix4x4(1, 3)) < threshold) [[likely]]
		{
			angles.x = atan2(aRotationMatrix4x4(2, 3), aRotationMatrix4x4(3, 3));
			angles.z = atan2(aRotationMatrix4x4(1, 2), aRotationMatrix4x4(1, 1));
		}
		else
		{
			angles.x = static_cast<_Ty>(0);
			angles.z = atan2(-aRotationMatrix4x4(2, 1), aRotationMatrix4x4(2, 2));
		}
		return Euler<_Ty> { angles, RotationOrder::zyx };
	}
#pragma endregion

#pragma region QuaternionRotationSet
	template<typename _Ty>
	void SetRotation(Quaternion<_Ty>& aQuaternion, const Euler<_Ty>& aEuler)
	{
		SetRotation(aQuaternion, aEuler.Angles, aEuler.Order);
	}
	template<typename _Ty>
	inline void SetRotation(Quaternion<_Ty>& aQuaternion, const Vector3<_Ty>& aEulerAngles, RotationOrder aRotationOrder)
	{
		typedef void(*setRotation)(Quaternion<_Ty>&, const Vector3<_Ty>&);

		static const setRotation constexpr index[6] = {
			&SetRotationXYZ<_Ty>,
			&SetRotationXZY<_Ty>,
			&SetRotationYXZ<_Ty>,
			&SetRotationYZX<_Ty>,
			&SetRotationZXY<_Ty>,
			&SetRotationZYX<_Ty>,
		};

		(index[static_cast<size_t>(aRotationOrder)])(aQuaternion, aEulerAngles);
	}
	template<typename _Ty>
	inline void SetRotationXYZ(Quaternion<_Ty>& quat, const Vector3<_Ty>& euler)
	{
		const _Ty x = euler.x, y = euler.y, z = euler.z;

		const _Ty c1 = cos(x / static_cast<_Ty>(2));
		const _Ty c2 = cos(y / static_cast<_Ty>(2));
		const _Ty c3 = cos(z / static_cast<_Ty>(2));

		const _Ty s1 = sin(x / static_cast<_Ty>(2));
		const _Ty s2 = sin(y / static_cast<_Ty>(2));
		const _Ty s3 = sin(z / static_cast<_Ty>(2));


		quat.x = s1 * c2 * c3 + c1 * s2 * s3;
		quat.y = c1 * s2 * c3 - s1 * c2 * s3;
		quat.z = c1 * c2 * s3 + s1 * s2 * c3;
		quat.w = c1 * c2 * c3 - s1 * s2 * s3;
	}
	template<typename _Ty>
	inline void SetRotationXZY(Quaternion<_Ty>& quat, const Vector3<_Ty>& euler)
	{
		const _Ty x = euler.x, y = euler.y, z = euler.z;

		const _Ty c1 = cos(x / static_cast<_Ty>(2));
		const _Ty c2 = cos(y / static_cast<_Ty>(2));
		const _Ty c3 = cos(z / static_cast<_Ty>(2));

		const _Ty s1 = sin(x / static_cast<_Ty>(2));
		const _Ty s2 = sin(y / static_cast<_Ty>(2));
		const _Ty s3 = sin(z / static_cast<_Ty>(2));

		quat.x = s1 * c2 * c3 - c1 * s2 * s3;
		quat.y = c1 * s2 * c3 - s1 * c2 * s3;
		quat.z = c1 * c2 * s3 + s1 * s2 * c3;
		quat.w = c1 * c2 * c3 + s1 * s2 * s3;
	}
	template<typename _Ty>
	inline void SetRotationYXZ(Quaternion<_Ty>& quat, const Vector3<_Ty>& euler)
	{
		const _Ty x = euler.x, y = euler.y, z = euler.z;

		const _Ty c1 = cos(x / static_cast<_Ty>(2));
		const _Ty c2 = cos(y / static_cast<_Ty>(2));
		const _Ty c3 = cos(z / static_cast<_Ty>(2));

		const _Ty s1 = sin(x / static_cast<_Ty>(2));
		const _Ty s2 = sin(y / static_cast<_Ty>(2));
		const _Ty s3 = sin(z / static_cast<_Ty>(2));

		quat.x = s1 * c2 * c3 + c1 * s2 * s3;
		quat.y = c1 * s2 * c3 - s1 * c2 * s3;
		quat.z = c1 * c2 * s3 - s1 * s2 * c3;
		quat.w = c1 * c2 * c3 + s1 * s2 * s3;
	}
	template<typename _Ty>
	inline void SetRotationYZX(Quaternion<_Ty>& quat, const Vector3<_Ty>& euler)
	{
		const _Ty x = euler.x, y = euler.y, z = euler.z;

		const _Ty c1 = cos(x / static_cast<_Ty>(2));
		const _Ty c2 = cos(y / static_cast<_Ty>(2));
		const _Ty c3 = cos(z / static_cast<_Ty>(2));

		const _Ty s1 = sin(x / static_cast<_Ty>(2));
		const _Ty s2 = sin(y / static_cast<_Ty>(2));
		const _Ty s3 = sin(z / static_cast<_Ty>(2));

		quat.x = s1 * c2 * c3 + c1 * s2 * s3;
		quat.y = c1 * s2 * c3 + s1 * c2 * s3;
		quat.z = c1 * c2 * s3 - s1 * s2 * c3;
		quat.w = c1 * c2 * c3 - s1 * s2 * s3;
	}
	template<typename _Ty>
	inline void SetRotationZXY(Quaternion<_Ty>& quat, const Vector3<_Ty>& euler)
	{
		const _Ty x = euler.x, y = euler.y, z = euler.z;

		const _Ty c1 = cos(x / static_cast<_Ty>(2));
		const _Ty c2 = cos(y / static_cast<_Ty>(2));
		const _Ty c3 = cos(z / static_cast<_Ty>(2));

		const _Ty s1 = sin(x / static_cast<_Ty>(2));
		const _Ty s2 = sin(y / static_cast<_Ty>(2));
		const _Ty s3 = sin(z / static_cast<_Ty>(2));

		quat.x = s1 * c2 * c3 - c1 * s2 * s3;
		quat.y = c1 * s2 * c3 + s1 * c2 * s3;
		quat.z = c1 * c2 * s3 + s1 * s2 * c3;
		quat.w = c1 * c2 * c3 - s1 * s2 * s3;
	}
	template<typename _Ty>
	inline void SetRotationZYX(Quaternion<_Ty>& quat, const Vector3<_Ty>& euler)
	{
		const _Ty x = euler.x, y = euler.y, z = euler.z;

		const _Ty c1 = cos(x / static_cast<_Ty>(2));
		const _Ty c2 = cos(y / static_cast<_Ty>(2));
		const _Ty c3 = cos(z / static_cast<_Ty>(2));

		const _Ty s1 = sin(x / static_cast<_Ty>(2));
		const _Ty s2 = sin(y / static_cast<_Ty>(2));
		const _Ty s3 = sin(z / static_cast<_Ty>(2));

		quat.x = s1 * c2 * c3 - c1 * s2 * s3;
		quat.y = c1 * s2 * c3 + s1 * c2 * s3;
		quat.z = c1 * c2 * s3 - s1 * s2 * c3;
		quat.w = c1 * c2 * c3 + s1 * s2 * s3;
	}
#pragma endregion

#pragma region Matrix3x3RotationSet
	template<typename _Ty>
	void SetRotation(Matrix3x3<_Ty>& aMatrix3x3, const Euler<_Ty>& aEuler)
	{
		SetRotation(aMatrix3x3, aEuler.Angles, aEuler.Order);
	}
	template<typename _Ty>
	inline void SetRotation(Matrix3x3<_Ty>& aMatrix3x3, const Vector3<_Ty>& aEulerAngles, RotationOrder aRotationOrder)
	{
		typedef void(*setRotation)(Matrix3x3<_Ty>&, const Vector3<_Ty>&);

		static const setRotation constexpr index[6] = {
			&SetRotationXYZ,
			&SetRotationXZY,
			&SetRotationYXZ,
			&SetRotationYZX,
			&SetRotationZXY,
			&SetRotationZYX,
		};

		(index[static_cast<size_t>(aRotationOrder)])(aMatrix3x3, aEulerAngles);
	}
	template<typename _Ty>
	inline void SetRotationXYZ(Matrix3x3<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ae = a * e, af = a * f, be = b * e, bf = b * f;

		matrix(1, 1) = c * e;
		matrix(2, 1) = -c * f;
		matrix(3, 1) = d;

		matrix(1, 2) = af + be * d;
		matrix(2, 2) = ae - bf * d;
		matrix(3, 2) = -b * c;

		matrix(1, 3) = bf - ae * d;
		matrix(2, 3) = be + af * d;
		matrix(3, 3) = a * c;
	}
	template<typename _Ty>
	inline void SetRotationXZY(Matrix3x3<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ac = a * c, ad = a * d, bc = b * c, bd = b * d;

		matrix(1, 1) = c * e;
		matrix(2, 1) = -f;
		matrix(3, 1) = d * e;

		matrix(1, 2) = ac * f + bd;
		matrix(2, 2) = a * e;
		matrix(3, 2) = ad * f - bc;

		matrix(1, 3) = bc * f - ad;
		matrix(2, 3) = b * e;
		matrix(3, 3) = bd * f + ac;
	}
	template<typename _Ty>
	inline void SetRotationYXZ(Matrix3x3<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ce = c * e, cf = c * f, de = d * e, df = d * f;

		matrix(1, 1) = ce + df * b;
		matrix(2, 1) = de * b - cf;
		matrix(3, 1) = a * d;

		matrix(1, 2) = a * f;
		matrix(2, 2) = a * e;
		matrix(3, 2) = -b;

		matrix(1, 3) = cf * b - de;
		matrix(2, 3) = df + ce * b;
		matrix(3, 3) = a * c;
	}
	template<typename _Ty>
	inline void SetRotationYZX(Matrix3x3<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ac = a * c, ad = a * d, bc = b * c, bd = b * d;

		matrix(1, 1) = c * e;
		matrix(2, 1) = bd - ac * f;
		matrix(3, 1) = bc * f + ad;

		matrix(1, 2) = f;
		matrix(2, 2) = a * e;
		matrix(3, 2) = -b * e;

		matrix(1, 3) = -d * e;
		matrix(2, 3) = ad * f + bc;
		matrix(3, 3) = ac - bd * f;
	}
	template<typename _Ty>
	inline void SetRotationZXY(Matrix3x3<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ce = c * e, cf = c * f, de = d * e, df = d * f;

		matrix(1, 1) = ce - df * b;
		matrix(2, 1) = -a * f;
		matrix(3, 1) = de + cf * b;

		matrix(1, 2) = cf + de * b;
		matrix(2, 2) = a * e;
		matrix(3, 2) = df - ce * b;

		matrix(1, 3) = -a * d;
		matrix(2, 3) = b;
		matrix(3, 3) = a * c;
	}
	template<typename _Ty>
	inline void SetRotationZYX(Matrix3x3<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ae = a * e, af = a * f, be = b * e, bf = b * f;

		matrix(1, 1) = c * e;
		matrix(2, 1) = be * d - af;
		matrix(3, 1) = ae * d + bf;

		matrix(1, 2) = c * f;
		matrix(2, 2) = bf * d + ae;
		matrix(3, 2) = af * d - be;

		matrix(1, 3) = -d;
		matrix(2, 3) = b * c;
		matrix(3, 3) = a * c;
	}
#pragma endregion

#pragma region Matrix4x4RotationSet
	template<typename _Ty>
	void SetRotation(Matrix4x4<_Ty>& aMatrix4x4, const Euler<_Ty>& aEuler)
	{
		SetRotation(aMatrix4x4, aEuler.Angles, aEuler.Order);
	}
	template<typename _Ty>
	inline void SetRotation(Matrix4x4<_Ty>& aMatrix4x4, const Vector3<_Ty>& aEulerAngles, RotationOrder aRotationOrder)
	{
		typedef void(*setRotation)(Matrix4x4<_Ty>&, const Vector3<_Ty>&);

		static const setRotation constexpr index[6] = {
			&SetRotationXYZ,
			&SetRotationXZY,
			&SetRotationYXZ,
			&SetRotationYZX,
			&SetRotationZXY,
			&SetRotationZYX,
		};

		(index[static_cast<size_t>(aRotationOrder)])(aMatrix4x4, aEulerAngles);
	}
	template<typename _Ty>
	inline void SetRotationXYZ(Matrix4x4<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ae = a * e, af = a * f, be = b * e, bf = b * f;

		matrix(1, 1) = c * e;
		matrix(2, 1) = -c * f;
		matrix(3, 1) = d;

		matrix(1, 2) = af + be * d;
		matrix(2, 2) = ae - bf * d;
		matrix(3, 2) = -b * c;

		matrix(1, 3) = bf - ae * d;
		matrix(2, 3) = be + af * d;
		matrix(3, 3) = a * c;
	}
	template<typename _Ty>
	inline void SetRotationXZY(Matrix4x4<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ac = a * c, ad = a * d, bc = b * c, bd = b * d;

		matrix(1, 1) = c * e;
		matrix(2, 1) = -f;
		matrix(3, 1) = d * e;

		matrix(1, 2) = ac * f + bd;
		matrix(2, 2) = a * e;
		matrix(3, 2) = ad * f - bc;

		matrix(1, 3) = bc * f - ad;
		matrix(2, 3) = b * e;
		matrix(3, 3) = bd * f + ac;
	}
	template<typename _Ty>
	inline void SetRotationYXZ(Matrix4x4<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ce = c * e, cf = c * f, de = d * e, df = d * f;

		matrix(1, 1) = ce + df * b;
		matrix(2, 1) = de * b - cf;
		matrix(3, 1) = a * d;

		matrix(1, 2) = a * f;
		matrix(2, 2) = a * e;
		matrix(3, 2) = -b;

		matrix(1, 3) = cf * b - de;
		matrix(2, 3) = df + ce * b;
		matrix(3, 3) = a * c;
	}
	template<typename _Ty>
	inline void SetRotationYZX(Matrix4x4<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ac = a * c, ad = a * d, bc = b * c, bd = b * d;

		matrix(1, 1) = c * e;
		matrix(2, 1) = bd - ac * f;
		matrix(3, 1) = bc * f + ad;

		matrix(1, 2) = f;
		matrix(2, 2) = a * e;
		matrix(3, 2) = -b * e;

		matrix(1, 3) = -d * e;
		matrix(2, 3) = ad * f + bc;
		matrix(3, 3) = ac - bd * f;
	}
	template<typename _Ty>
	inline void SetRotationZXY(Matrix4x4<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ce = c * e, cf = c * f, de = d * e, df = d * f;

		matrix(1, 1) = ce - df * b;
		matrix(2, 1) = -a * f;
		matrix(3, 1) = de + cf * b;

		matrix(1, 2) = cf + de * b;
		matrix(2, 2) = a * e;
		matrix(3, 2) = df - ce * b;

		matrix(1, 3) = -a * d;
		matrix(2, 3) = b;
		matrix(3, 3) = a * c;
	}
	template<typename _Ty>
	inline void SetRotationZYX(Matrix4x4<_Ty>& matrix, const Vector3<_Ty>& euler)
	{
		const _Ty
			x = euler.x,
			y = euler.y,
			z = euler.z;

		const _Ty a = cos(x), b = sin(x);
		const _Ty c = cos(y), d = sin(y);
		const _Ty e = cos(z), f = sin(z);

		const _Ty ae = a * e, af = a * f, be = b * e, bf = b * f;

		matrix(1, 1) = c * e;
		matrix(2, 1) = be * d - af;
		matrix(3, 1) = ae * d + bf;

		matrix(1, 2) = c * f;
		matrix(2, 2) = bf * d + ae;
		matrix(3, 2) = af * d - be;

		matrix(1, 3) = -d;
		matrix(2, 3) = b * c;
		matrix(3, 3) = a * c;
	}
#pragma endregion
}