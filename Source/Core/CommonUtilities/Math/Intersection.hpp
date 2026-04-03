#pragma once
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Circle.hpp"
#include "AABB3D.hpp"
#include "AABB2D.hpp"
#include "Ray.hpp"
#include "Line.hpp"

namespace CommonUtilities
{
	// If the ray is parallel to the plane, aOutIntersectionPoint remains unchanged. If
	// the ray is in the plane, true is returned, if not, false is returned. If the ray
	// isn't parallel to the plane and hits, the intersection point is stored in
	// aOutIntersectionPoint and true returned otherwise false is returned and
	// outIntersectionPoint is unchanged.
	template<typename _Ty>
	bool IntersectionPlaneRay(const Plane<_Ty>& aPlane, const Ray<_Ty>& aRay, Vector3<_Ty>&
		outIntersectionPoint);
	// If no collision, aOutIntersectionPoint remains unchanged.
	// If The sphere overlaps the AABB true is returned, if not, false is returned.
	// The point on the AABB closest to the sphere centre is saved in
	// aOutIntersectionPoint.
	// A sphere touching the aabb is considered overlapping.
	template<typename _Ty>
	bool IntersectionSphereAABB(const Sphere<_Ty>& aSphere, const AABB3D<_Ty>& aAABB3D,
		Vector3<_Ty>& outIntersectionPoint);
	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// A ray in one of the AABB's sides is counted as intersecting it.
	template<typename _Ty>
	bool IntersectionAABBRay(const AABB3D<_Ty>& aAABB, const Ray<_Ty>& aRay);
	// If the ray intersects the sphere, true is returned, if not, false is returned.
	// A ray intersecting the surface of the sphere is considered as intersecting it.
	template<typename _Ty>
	bool IntersectionSphereRay(const Sphere<_Ty>& aSphere, const Ray<_Ty>& aRay);

	template<typename _Ty>
	bool IntersectionCircleAABB(const Circle<_Ty>& aCircle, const AABB2D<_Ty>& aAABB);

	template<typename _Ty>
	bool IntersectionPlaneRay(const Plane<_Ty>& aPlane, const Ray<_Ty>& aRay, Vector3<_Ty>& outIntersectionPoint)
	{
		bool parallel = aPlane.Normal.Dot(aRay.Direction) == 0;

		Vector3<_Ty> Origin_To_Plane = aPlane.Point - aRay.Origin;
		if (aPlane.Normal.Dot((Origin_To_Plane)) == 0)
		{
			if (!parallel)
			{
				outIntersectionPoint = aRay.Origin;
			}
			return true;
		}
		else if (!parallel)
		{
			_Ty t = (Origin_To_Plane.Dot(aPlane.Normal) / aRay.Direction.Dot(aPlane.Normal));
			if (t >= 0)
			{
				outIntersectionPoint = aRay.Origin + aRay.Direction * t;
				return true;
			}
			else return false;
		}
		else return false;
	}
	template<typename _Ty>
	bool IntersectionSphereAABB(const Sphere<_Ty>& aSphere, const AABB3D<_Ty>& aAABB3D, Vector3<_Ty>& outIntersectionPoint)
	{
		Vector3<_Ty> closestPoint;
		closestPoint.x = std::fmax(aSphere.Center.x, aAABB3D.Min.x);
		closestPoint.y = std::fmax(aSphere.Center.y, aAABB3D.Min.y);
		closestPoint.z = std::fmax(aSphere.Center.z, aAABB3D.Min.z);

		closestPoint.x = std::fmin(closestPoint.x, aAABB3D.Max.x);
		closestPoint.y = std::fmin(closestPoint.y, aAABB3D.Max.y);
		closestPoint.z = std::fmin(closestPoint.z, aAABB3D.Max.z);

		if ((closestPoint - aSphere.Center).LengthSqr() <= aSphere.Radius * aSphere.Radius)
		{
			outIntersectionPoint = closestPoint;
			return true;
		}
		return false;
	}
	template<typename _Ty>
	bool IntersectionAABBRay(const AABB3D<_Ty>& aAABB, const Ray<_Ty>& aRay)
	{
		enum quadrant
		{
			eLeft = 0,
			eRight = 1,
			eMiddle = 2
		};

		if (aRay.Direction.x == static_cast<_Ty>(0) && aRay.Direction.y == static_cast<_Ty>(0) && aRay.Direction.z == static_cast<_Ty>(0))
		{
			return false;
		}

		bool inside = true;
		quadrant x, y, z;
		_Ty planeX = aAABB.Min.x, planeY = aAABB.Min.y, planeZ = aAABB.Min.z;
		if (aRay.Origin.x < aAABB.Min.x)
		{
			x = quadrant::eLeft;
			inside = false;
		}
		else if (aRay.Origin.x > aAABB.Max.x)
		{
			x = quadrant::eRight;
			inside = false;
			planeX = aAABB.Max.x;
		}
		else x = quadrant::eMiddle;

		if (aRay.Origin.y < aAABB.Min.y)
		{
			y = quadrant::eLeft;
			inside = false;
		}
		else if (aRay.Origin.y > aAABB.Max.y)
		{
			y = quadrant::eRight;
			inside = false;
			planeY = aAABB.Max.y;
		}
		else y = quadrant::eMiddle;

		if (aRay.Origin.z < aAABB.Min.z)
		{
			z = quadrant::eLeft;
			inside = false;
		}
		else if (aRay.Origin.z > aAABB.Max.z)
		{
			z = quadrant::eRight;
			inside = false;
			planeZ = aAABB.Max.z;
		}
		else z = quadrant::eMiddle;

		if (inside)
		{
			return true;
		}

		_Ty maxX = static_cast<_Ty>(-1);
		_Ty maxY = static_cast<_Ty>(-1);
		_Ty maxZ = static_cast<_Ty>(-1);
		if (x != quadrant::eMiddle)
		{
			maxX = (planeX - aRay.Origin.x) / aRay.Direction.x;
		}
		if (y != quadrant::eMiddle)
		{
			maxY = (planeY - aRay.Origin.y) / aRay.Direction.y;
		}
		if (z != quadrant::eMiddle)
		{
			maxZ = (planeZ - aRay.Origin.z) / aRay.Direction.z;
		}

		if (maxX > 0 && maxX > maxY && maxX > maxZ)
		{
			_Ty coordY = aRay.Origin.y + maxY * aRay.Direction.x;
			if (coordY > aAABB.Min.y || coordY < aAABB.Max.y) return false;
			_Ty coordZ = aRay.Origin.z + maxZ * aRay.Direction.x;
			if (coordZ > aAABB.Min.z || coordZ < aAABB.Max.z) return false;
		}
		else if (maxY > 0 && maxY > maxZ)
		{
			_Ty coordX = aRay.Origin.x + maxX * aRay.Direction.x;
			if (coordX > aAABB.Min.x || coordX < aAABB.Max.x) return false;
			_Ty coordZ = aRay.Origin.z + maxZ * aRay.Direction.x;
			if (coordZ > aAABB.Min.z || coordZ < aAABB.Max.z) return false;
		}
		else if (maxZ > 0)
		{
			_Ty coordX = aRay.Origin.x + maxX * aRay.Direction.x;
			if (coordX > aAABB.Min.x || coordX < aAABB.Max.x) return false;
			_Ty coordY = aRay.Origin.y + maxY * aRay.Direction.x;
			if (coordY > aAABB.Min.y || coordY < aAABB.Max.y) return false;
		}
		else return false;

		return true;
	}
	template<typename _Ty>
	bool IntersectionAABB2DRay(const AABB2D<_Ty> aAABB, const Vector2<_Ty>& aOrigin, const Vector2<_Ty>& aDirection)
	{
		Vector2<_Ty> sizeY(static_cast<_Ty>(0), aAABB.Max.y - aAABB.Min.y);

		Line<_Ty> lines[4];

		lines[0] = Line<_Ty>(aAABB.Min + sizeY, aAABB.Max);
		lines[1] = Line<_Ty>(aAABB.Max, aAABB.Max - sizeY);
		lines[2] = Line<_Ty>(aAABB.Max - sizeY, aAABB.Min);
		lines[3] = Line<_Ty>(aAABB.Min, aAABB.Min + sizeY);

		for (Line<_Ty>& line : lines)
		{
			_Ty bDotDPerp = aDirection.x * line.Direction.y - aDirection.y * line.Direction.x;

			if (bDotDPerp == static_cast<_Ty>(0))
			{
				continue;
			}

			Vector2<_Ty> c = line.Point - aOrigin;
			_Ty t = (c.x * line.Direction.y - c.y * line.Direction.x) / bDotDPerp;
			if (t < static_cast<_Ty>(0))
			{
				continue;
			}

			_Ty u = (c.x * aDirection.y - c.y * aDirection.x) / bDotDPerp;
			if (u < static_cast<_Ty>(0) || u > static_cast<_Ty>(1))
			{
				continue;
			}

			return true;
		}

		return false;
	}
	template<typename _Ty>
	bool IntersectionSphereRay(const Sphere<_Ty>& aSphere, const Ray<_Ty>& aRay)
	{
		Vector3<_Ty> diff = aSphere.Center - aRay.Origin;
		_Ty t = aRay.Direction.Dot(diff) / (aRay.Direction.LengthSqr());
		if (t < static_cast<_Ty>(0))
		{
			return false;
		}
		Vector3<_Ty> point = aRay.Origin + aRay.Direction * t;
		return (point - aSphere.Center).LengthSqr() <= aSphere.Radius * aSphere.Radius;
	}
	template<typename _Ty>
	bool IntersectionCircleRay(const Circle<_Ty>& aCircle, const Vector2<_Ty>& aOrigin, const Vector2<_Ty>& aDirection)
	{
		Vector2<_Ty> oc = aCircle.Center - aOrigin;
		_Ty t = aDirection.Dot(oc) / aDirection.LengthSqr();

		if (t < 0)
		{
			return false;
		}

		Vector2<_Ty> closest = aOrigin + t * aDirection;

		return (aCircle.Center - closest).LengthSqr() <= aCircle.Radius * aCircle.Radius;
	}
	template<typename _Ty>
	bool IntersectionCircleAABB(const Circle<_Ty>& aCircle, const AABB2D<_Ty>& aAABB)
	{
		if (aAABB.IsInside(aCircle.Center))
		{
			return true;
		}

		const Vector2<_Ty> clampedCenter = {
			std::max(std::min(aCircle.Center.x, aAABB.Max.x), aAABB.Min.x),
			std::max(std::min(aCircle.Center.y, aAABB.Max.y), aAABB.Min.y)
		};

		return (clampedCenter - aCircle.Center).LengthSqr() <= aCircle.Radius * aCircle.Radius;
	}
}