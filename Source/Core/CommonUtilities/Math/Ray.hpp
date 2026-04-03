#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Ray
	{
	public:
		Vector3<_Ty> Origin;
		Vector3<_Ty> Direction;
		// Default constructor: there is no ray, the origin and direction are the
		// zero vector.
		Ray() = default;
		// Copy constructor.
		Ray(const Ray<_Ty>& aRay) = default;
		// Constructor that takes origin and direction to define the ray
		Ray(const Vector3<_Ty>& aOrigin, const Vector3<_Ty>& aDirection);
		// Init the ray with two points.
		void InitWith2Points(const Vector3<_Ty>& aOrigin, const Vector3<_Ty>& aPoint);
		// Init the ray with an origin and a direction.
		void InitWithOriginAndDirection(const Vector3<_Ty>& aOrigin, const Vector3<_Ty>&
			aDirection);

	};
	template<typename _Ty>
	inline Ray<_Ty>::Ray(const Vector3<_Ty>& aOrigin, const Vector3<_Ty>& aDirection)
	{
		InitWithOriginAndDirection(aOrigin, aDirection);
	}
	template<typename _Ty>
	inline void Ray<_Ty>::InitWith2Points(const Vector3<_Ty>& aOrigin, const Vector3<_Ty>& aPoint)
	{
		this->Origin = aOrigin;
		this->Direction = aPoint - aOrigin;
	}
	template<typename _Ty>
	inline void Ray<_Ty>::InitWithOriginAndDirection(const Vector3<_Ty>& aOrigin, const Vector3<_Ty>& aDirection)
	{
		this->Origin = aOrigin;
		this->Direction = aDirection;
	}
}