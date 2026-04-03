#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Sphere
	{
	public:
		Vector3<_Ty> Center;
		_Ty Radius;

		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		Sphere();
		// Copy constructor.
		Sphere(const Sphere<_Ty>& aSphere) = default;
		// Constructor that takes the center position and radius of the sphere.
		Sphere(const Vector3<_Ty>& aCenter, _Ty aRadius);
		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<_Ty>& aCenter, _Ty aRadius);
		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const Vector3<_Ty>& aPosition) const;
	};
	template<typename _Ty>
	inline Sphere<_Ty>::Sphere()
	{
		this->Radius = static_cast<_Ty>(0);
	}
	template<typename _Ty>
	inline Sphere<_Ty>::Sphere(const Vector3<_Ty>& aCenter, _Ty aRadius)
	{
		InitWithCenterAndRadius(aCenter, aRadius);
	}
	template<typename _Ty>
	inline void Sphere<_Ty>::InitWithCenterAndRadius(const Vector3<_Ty>& aCenter, _Ty aRadius)
	{
		this->Center = aCenter;
		this->Radius = aRadius;
	}
	template<typename _Ty>
	inline bool Sphere<_Ty>::IsInside(const Vector3<_Ty>& aPosition) const
	{
		return (aPosition - this->Center).LengthSqr() <= this->Radius * this->Radius;
	}
}