#pragma once

#include "Vector.h"
#include "Quaternion.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Vector2;
	template<typename _Ty>
	class Vector3;
	template<typename _Ty>
	class Vector4;

	template<typename _Ty>
	class Quaternion;

	template<typename _Ty>
	Vector2<_Ty> Lerp(const Vector2<_Ty> aV1, const Vector2<_Ty> aV2, const _Ty& aCoeffiscient);
	template<typename _Ty>
	Vector3<_Ty> Lerp(const Vector3<_Ty> aV1, const Vector3<_Ty> aV2, const _Ty& aCoeffiscient);
	template<typename _Ty>
	Vector4<_Ty> Lerp(const Vector4<_Ty> aV1, const Vector4<_Ty> aV2, const _Ty& aCoeffiscient);

	template<typename _Ty>
	Quaternion<_Ty> SLerp(const Quaternion<_Ty> aQ1, const Quaternion<_Ty> aQ2, const _Ty& aCoeffiscient);


	template<typename _Ty>
	Vector2<_Ty> Lerp(const Vector2<_Ty> aV1, const Vector2<_Ty> aV2, const _Ty& aCoeffiscient)
	{
		return Vector2<_Ty>(
			aV1.x + aCoeffiscient * (aV2.x - aV1.x),
			aV1.y + aCoeffiscient * (aV2.y - aV1.y)
		);
	}
	template<typename _Ty>
	Vector3<_Ty> Lerp(const Vector3<_Ty> aV1, const Vector3<_Ty> aV2, const _Ty& aCoeffiscient)
	{
		return Vector3<_Ty>(
			aV1.x + aCoeffiscient * (aV2.x - aV1.x),
			aV1.y + aCoeffiscient * (aV2.y - aV1.y),
			aV1.z + aCoeffiscient * (aV2.z - aV1.z)
		);
	}
	template<typename _Ty>
	Vector4<_Ty> Lerp(const Vector4<_Ty> aV1, const Vector4<_Ty> aV2, const _Ty& aCoeffiscient)
	{
		return Vector4<_Ty>(
			aV1.x + aCoeffiscient * (aV2.x - aV1.x),
			aV1.y + aCoeffiscient * (aV2.y - aV1.y),
			aV1.z + aCoeffiscient * (aV2.z - aV1.z),
			aV1.w + aCoeffiscient * (aV2.w - aV1.w)
		);
	}
	template<typename _Ty>
	Quaternion<_Ty> SLerp(const Quaternion<_Ty> aQ1, const Quaternion<_Ty> aQ2, const _Ty& aCoeffiscient)
	{
		_Ty dot = aQ1.w * aQ2.w
			+ aQ1.x * aQ2.x
			+ aQ1.y * aQ2.y
			+ aQ1.z * aQ2.z;

		Quaternion<_Ty> second = aQ2;
		if (dot < static_cast<_Ty>(0))
		{
			dot = -dot;
			second *= static_cast<_Ty>(-1);
		}

		const _Ty theta = acos(dot);	//Half angle between 3D vectors of same rotation
		const _Ty sTheta = sin(theta);

		if (isnan(theta) || abs(theta) < 0.001f)
		{
			return aQ1;
		}

		const _Ty w1 = sin((static_cast<_Ty>(1) - aCoeffiscient) * theta) / sTheta;
		const _Ty w2 = sin((aCoeffiscient * theta)) / sTheta;

		return w1 * aQ1 + w2 * second;
	}
}
