#pragma once
#include "Vector2.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Line
	{
	public:
		Vector2<_Ty> Point;
		Vector2<_Ty> Direction;

		// Default constructor: there is no line, the normal is the zero vector.
		Line() = default;

		// Copy constructor.
		Line(const Line<_Ty>& aLine) = default;

		// Constructor that takes two points that define the line, the direction is aPoint1 - aPoint0.
		Line(const Vector2<_Ty>& aPoint0, const Vector2<_Ty>& aPoint1);

		// Init the line with two points, the same as the constructor above.
		void InitWith2Points(const Vector2<_Ty>& aPoint0, const Vector2<_Ty>& aPoint1);

		// Init the line with a point and a direction.
		void InitWithPointAndDirection(const Vector2<_Ty>& aPoint, const Vector2<_Ty>& aDirection);

		// Returns whether a point is inside the line: it is inside when the point is on the line or on the side the normal is pointing away from.
		bool IsInside(const Vector2<_Ty>& aPosition) const;

		// Returns the direction of the line.
		const Vector2<_Ty>& GetDirection() const;

		// Returns the normal of the line, which is (-direction.y, direction.x).
		const Vector2<_Ty> GetNormal() const;


	};

	template<typename _Ty>
	inline Line<_Ty>::Line(const Vector2<_Ty>& aPoint0, const Vector2<_Ty>& aPoint1)
	{
		InitWith2Points(aPoint0, aPoint1);
	}
	template<typename _Ty>
	inline void Line<_Ty>::InitWith2Points(const Vector2<_Ty>& aPoint0, const Vector2<_Ty>& aPoint1)
	{
		this->Point = aPoint0;
		this->Direction = aPoint1 - aPoint0;
	}
	template<typename _Ty>
	inline void Line<_Ty>::InitWithPointAndDirection(const Vector2<_Ty>& aPoint, const Vector2<_Ty>& aDirection)
	{
		this->Point = aPoint;
		this->Direction = aDirection;
	}
	template<typename _Ty>
	inline bool Line<_Ty>::IsInside(const Vector2<_Ty>& aPosition) const
	{
		return GetNormal().Dot(aPosition - Point) <= static_cast<_Ty>(0);
	}
	template<typename _Ty>
	inline const Vector2<_Ty>& Line<_Ty>::GetDirection() const
	{
		return this->Direction;
	}
	template<typename _Ty>
	inline const Vector2<_Ty> Line<_Ty>::GetNormal() const
	{
		return Vector2<_Ty>(-this->Direction.y, this->Direction.x);
	}
}

