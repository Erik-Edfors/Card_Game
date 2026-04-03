#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class AABB3D
	{
	public:
		Vector3<_Ty> Min, Max;
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D() = default;
		// Copy constructor.
		AABB3D(const AABB3D<_Ty>& aAABB3D) = default;
		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<_Ty>& aMin, const Vector3<_Ty>& aMax);
		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<_Ty>& aMin, const Vector3<_Ty>& aMax);
		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector3<_Ty>& aPosition) const;
	};
	template<typename _Ty>
	inline AABB3D<_Ty>::AABB3D(const Vector3<_Ty>& aMin, const Vector3<_Ty>& aMax)
	{
		this->InitWithMinAndMax(aMin, aMax);
	}
	template<typename _Ty>
	inline void AABB3D<_Ty>::InitWithMinAndMax(const Vector3<_Ty>& aMin, const Vector3<_Ty>& aMax)
	{
		this->Min = aMin;
		this->Max = aMax;
	}
	template<typename _Ty>
	inline bool AABB3D<_Ty>::IsInside(const Vector3<_Ty>& aPosition) const
	{
		if (aPosition.x < this->Min.x || aPosition.y < this->Min.y || aPosition.z < this->Min.z)
		{
			return false;
		}
		if (aPosition.x > this->Max.x || aPosition.y > this->Max.y || aPosition.z > this->Max.z)
		{
			return false;
		}

		return true;
	}
}