#pragma once
#include "Line.hpp"
#include <vector>

namespace CommonUtilities
{
	template<typename _Ty>
	class LineVolume
	{
	public:
		// Default constructor: empty LineVolume.
		LineVolume() = default;

		// Constructor taking a list of Line that makes up the LineVolume.
		LineVolume(const std::vector<Line<_Ty>>& aLineList);

		// Add a Line to the LineVolume.
		void AddLine(const Line<_Ty>& aLine);

		// Returns whether a point is inside the LineVolume: it is inside when the point is
		// on the line or on the side the normal is pointing away from for all the lines in
		// the LineVolume.
		bool IsInside(const Vector2<_Ty>& aPosition) const;

	private:
		std::vector<Line<_Ty>> myLines;
	};
	template<typename _Ty>
	inline LineVolume<_Ty>::LineVolume(const std::vector<Line<_Ty>>& aLineList)
	{
		this->myLines = aLineList;
	}
	template<typename _Ty>
	inline void LineVolume<_Ty>::AddLine(const Line<_Ty>& aLine)
	{
		this->myLines.push_back(aLine);
	}
	template<typename _Ty>
	inline bool LineVolume<_Ty>::IsInside(const Vector2<_Ty>& aPosition) const
	{
		for (const Line<_Ty>& line : this->myLines)
		{
			if (!line.IsInside(aPosition))
			{
				return false;
			}
		}
		return true;
	}
}