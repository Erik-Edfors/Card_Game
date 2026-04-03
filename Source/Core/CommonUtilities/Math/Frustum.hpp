#pragma once
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4x4.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Frustum
	{
	public:
		Frustum();
		~Frustum() = default;

		// Produces a frustum that only includes positions that will be inside -1 < x < +1, -1 < y < +1 and 0 < z < +1 after transformation;
		Frustum(const Matrix4x4<_Ty>& aMatrix);

		bool IsInside(const Vector3<_Ty>& aPoint) const;
		bool IsInside(const Vector3<_Ty>& aPoint, float aRadius) const;

		union
		{
			_Ty Data[24];
			_Ty PlaneData[6][4];
			Vector4<_Ty> Planes[6];
#pragma warning(push)
#pragma warning(disable : 4201)	// Disable unnamned struct warning.
			struct
			{
				Vector4<_Ty> Left;
				Vector4<_Ty> Right;
				Vector4<_Ty> Bottom;
				Vector4<_Ty> Top;
				Vector4<_Ty> Front;
				Vector4<_Ty> Back;
			};
#pragma warning(pop)
		};
	};

	template<typename _Ty>
	inline CommonUtilities::Frustum<_Ty>::Frustum()
	{
		constexpr _Ty zero = static_cast<_Ty>(0);
		constexpr _Ty one = static_cast<_Ty>(1);

		Left = Vector4<_Ty>(one, zero, zero, -one);
		Right = Vector4<_Ty>(-one, zero, zero, -one);
		Bottom = Vector4<_Ty>(zero, one, zero, -one);
		Top = Vector4<_Ty>(zero, -one, zero, -one);
		Front = Vector4<_Ty>(zero, zero, one, zero);
		Back = Vector4<_Ty>(zero, zero, -one, -one);
	}

	template<typename _Ty>
	inline CommonUtilities::Frustum<_Ty>::Frustum(const Matrix4x4<_Ty>& aMatrix)
	{
		// Left clipping plane
		Planes[0].x = aMatrix(1, 4) + aMatrix(1, 1);
		Planes[0].y = aMatrix(2, 4) + aMatrix(2, 1);
		Planes[0].z = aMatrix(3, 4) + aMatrix(3, 1);
		Planes[0].w = -(aMatrix(4, 4) + aMatrix(4, 1));
		// Right clipping plane
		Planes[1].x = aMatrix(1, 4) - aMatrix(1, 1);
		Planes[1].y = aMatrix(2, 4) - aMatrix(2, 1);
		Planes[1].z = aMatrix(3, 4) - aMatrix(3, 1);
		Planes[1].w = -(aMatrix(4, 4) - aMatrix(4, 1));
		// Bottom clipping plane
		Planes[2].x = aMatrix(1, 4) + aMatrix(1, 2);
		Planes[2].y = aMatrix(2, 4) + aMatrix(2, 2);
		Planes[2].z = aMatrix(3, 4) + aMatrix(3, 2);
		Planes[2].w = -(aMatrix(4, 4) + aMatrix(4, 2));
		// Top clipping plane
		Planes[3].x = aMatrix(1, 4) - aMatrix(1, 2);
		Planes[3].y = aMatrix(2, 4) - aMatrix(2, 2);
		Planes[3].z = aMatrix(3, 4) - aMatrix(3, 2);
		Planes[3].w = -(aMatrix(4, 4) - aMatrix(4, 2));

		// Near clipping plane
		Planes[4].x = aMatrix(1, 3);
		Planes[4].y = aMatrix(2, 3);
		Planes[4].z = aMatrix(3, 3);
		Planes[4].w = -aMatrix(4, 3);
		// Far clipping plane
		Planes[5].x = aMatrix(1, 4) - aMatrix(1, 3);
		Planes[5].y = aMatrix(2, 4) - aMatrix(2, 3);
		Planes[5].z = aMatrix(3, 4) - aMatrix(3, 3);
		Planes[5].w = -(aMatrix(4, 4) - aMatrix(4, 3));

		// Normalizing plane normals to make Sphere culling faster.
		for (size_t i = 0; i < 6; i++)
		{
			const _Ty factor = 1.f / Planes[i].GetXYZ().Length();
			Planes[i].x *= factor;
			Planes[i].y *= factor;
			Planes[i].z *= factor;
			Planes[i].w *= factor;
		}
	}

	template<typename _Ty>
	inline bool Frustum<_Ty>::IsInside(const Vector3<_Ty>& aPoint) const
	{
		for (size_t i = 0; i < 6; i++)
		{
			const _Ty dot = Planes[i].GetXYZ().Dot(aPoint);
			if (static_cast<_Ty>(0) > dot - Planes[i].w)
			{
				return false;
			}
		}
		return true;
	}

	template<typename _Ty>
	inline bool Frustum<_Ty>::IsInside(const Vector3<_Ty>& aPoint, float aRadius) const
	{
		for (size_t i = 0; i < 6; i++)
		{
			const _Ty dot = Planes[i].GetXYZ().Dot(aPoint);
			if (static_cast<_Ty>(0) > dot + aRadius - Planes[i].w) return false;
		}
		return true;
	}
}