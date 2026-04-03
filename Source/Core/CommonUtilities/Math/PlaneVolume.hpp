#pragma once
#include "Vector3.hpp"
#include "Plane.hpp"
#include <vector>

namespace CommonUtilities
{
	template<typename _Ty>
	class PlaneVolume
	{
	public:
		// Default constructor: empty PlaneVolume.
		PlaneVolume() = default;

		// Constructor taking a list of Plane that makes up the PlaneVolume.
		PlaneVolume(const std::vector<Plane<_Ty>>& aPlaneList);

		// Add a Plane to the PlaneVolume.
		void AddPlane(const Plane<_Ty>& aPlane);

		// Returns whether a point is inside the PlaneVolume: it is inside when the point is on the
		// plane or on the side the normal is pointing away from for all the planes in the PlaneVolume.
		bool IsInside(const Vector3<_Ty>& aPosition) const;

	private:
		std::vector<Plane<_Ty>> myPlanes;
	};
	template<typename _Ty>
	inline PlaneVolume<_Ty>::PlaneVolume(const std::vector<Plane<_Ty>>& aPlaneList)
	{
		this->myPlanes = aPlaneList;
	}
	template<typename _Ty>
	inline void PlaneVolume<_Ty>::AddPlane(const Plane<_Ty>& aPlane)
	{
		this->myPlanes.push_back(aPlane);
	}
	template<typename _Ty>
	inline bool PlaneVolume<_Ty>::IsInside(const Vector3<_Ty>& aPosition) const
	{
		for (const Plane<_Ty>& Plane : this->myPlanes)
		{
			if (!Plane.IsInside(aPosition))
			{
				return false;
			}
		}
		return true;
	}
}