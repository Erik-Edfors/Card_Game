#pragma once
#include "Vector2.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Circle
	{
	public:
		Vector2<_Ty> Center;
		_Ty Radius;

		// Default constructor: there is no circle, the radius is zero and the position is
		// the zero vector.
		Circle();
		// Copy constructor.
		Circle(const Circle<_Ty>& aCircle) = default;
		// Constructor that takes the center position and radius of the circle.
		Circle(const Vector2<_Ty>& aCenter, _Ty aRadius);
		// Init the circle with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector2<_Ty>& aCenter, _Ty aRadius);
		// Returns whether a point is inside the circle: it is inside when the point is on the
		// circle surface or inside of the circle.
		bool IsInside(const Vector2<_Ty>& aPosition) const;
	};

	using Circlef = Circle<float>;
	using Circled = Circle<double>;

	template<typename _Ty>
	inline Circle<_Ty>::Circle()
	{
		this->Radius = static_cast<_Ty>(0);
	}
	template<typename _Ty>
	inline Circle<_Ty>::Circle(const Vector2<_Ty>& aCenter, _Ty aRadius)
	{
		InitWithCenterAndRadius(aCenter, aRadius);
	}
	template<typename _Ty>
	inline void Circle<_Ty>::InitWithCenterAndRadius(const Vector2<_Ty>& aCenter, _Ty aRadius)
	{
		this->Center = aCenter;
		this->Radius = aRadius;
	}
	template<typename _Ty>
	inline bool Circle<_Ty>::IsInside(const Vector2<_Ty>& aPosition) const
	{
		return (aPosition - this->Center).LengthSqr() <= this->Radius * this->Radius;
	}
}