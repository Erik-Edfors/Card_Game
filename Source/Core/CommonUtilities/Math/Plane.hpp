#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Plane
	{
	public:
		Vector3<_Ty> Point;
		Vector3<_Ty> Normal;

		// Default constructor.
		Plane() = default;

		// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2 - aPoint0).
		Plane(const Vector3<_Ty>& aPoint0, const Vector3<_Ty>& aPoint1, const Vector3<_Ty>& aPoint2);

		// Constructor taking a point and a normal.
		Plane(const Vector3<_Ty>& aPoint0, const Vector3<_Ty>& aNormal);

		// Init the plane with three points, the same as the constructor above.
		void InitWith3Points(const Vector3<_Ty>& aPoint0, const Vector3<_Ty>& aPoint1, const Vector3<_Ty>& aPoint2);

		// Init the plane with a point and a normal, the same as the constructor above.
		void InitWithPointAndNormal(const Vector3<_Ty>& aPoint, const Vector3<_Ty>& aNormal);

		// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
		bool IsInside(const Vector3<_Ty>& aPosition) const;

		// Returns the normal of the plane.
		const Vector3<_Ty>& GetNormal() const;

	};
	template<typename _Ty>
	inline Plane<_Ty>::Plane(const Vector3<_Ty>& aPoint0, const Vector3<_Ty>& aPoint1, const Vector3<_Ty>& aPoint2)
	{
		InitWith3Points(aPoint0, aPoint1, aPoint2);
	}
	template<typename _Ty>
	inline Plane<_Ty>::Plane(const Vector3<_Ty>& aPoint0, const Vector3<_Ty>& aNormal)
	{
		InitWithPointAndNormal(aPoint0, aNormal);
	}
	template<typename _Ty>
	inline void Plane<_Ty>::InitWith3Points(const Vector3<_Ty>& aPoint0, const Vector3<_Ty>& aPoint1, const Vector3<_Ty>& aPoint2)
	{
		Vector3<_Ty> l1 = aPoint1 - aPoint0;
		Vector3<_Ty> l2 = aPoint2 - aPoint0;

		this->Point = aPoint0;
		this->Normal = l1.Cross(l2);
	}
	template<typename _Ty>
	inline void Plane<_Ty>::InitWithPointAndNormal(const Vector3<_Ty>& aPoint, const Vector3<_Ty>& aNormal)
	{
		this->Point = aPoint;
		this->Normal = aNormal;
	}
	template<typename _Ty>
	inline bool Plane<_Ty>::IsInside(const Vector3<_Ty>& aPosition) const
	{
		return this->Normal.Dot(aPosition - Point) <= static_cast<_Ty>(0);
	}
	template<typename _Ty>
	inline const Vector3<_Ty>& Plane<_Ty>::GetNormal() const
	{
		return this->Normal;
	}
}