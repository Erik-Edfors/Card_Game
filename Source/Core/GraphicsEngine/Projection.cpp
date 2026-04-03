#include "GraphicsEngine.pch.h"
#include "Projection.h"

CommonUtilities::Matrix4x4<float> CreateOrthographicProjection(float aLeft, float aRight, float aBottom, float aTop, float aNear, float aFar)
{
	CommonUtilities::Matrix4x4<float> projection;

	projection(1, 1) = 2.f / (aRight - aLeft);
	projection(2, 2) = 2.f / (aTop - aBottom);
	projection(3, 3) = 2.f / (aFar - aNear);

	projection(4, 1) = -(aRight + aLeft) / (aRight - aLeft);
	projection(4, 2) = -(aTop + aBottom) / (aTop - aBottom);
	projection(4, 3) = -(aFar + aNear) / (aFar - aNear);
	projection(4, 4) = 1.f;

	//projection(4, 1) = -2.f * aLeft / (aRight - aLeft) - 1.f;
	//projection(4, 2) = -2.f * aBottom / (aTop - aBottom) - 1.f;
	//projection(4, 3) = -aNear / (aFar - aNear);

	return projection;
}

CommonUtilities::Matrix4x4<float> CreatePerspectiveProjection(float aFOVRad, float aAspectRatio, float aNear, float aFar)
{
	CommonUtilities::Matrix4x4<float> projection;

	//const float zoomX = 1.f / std::tanf(aFOVRad * 0.5f);

	//projection(1, 1) = zoomX;
	//projection(2, 2) = aAspectRatio * zoomX;
	//projection(3, 3) = (aFar + aNear) / (aFar - aNear);
	//projection(4, 4) = 0.f;

	//projection(4, 3) = -2.f * aFar * aNear / (aFar - aNear);
	//projection(3, 4) = 1.f;


	//projection(1, 1) = zoomX;
	//projection(2, 2) = aAspectRatio * zoomX;
	//projection(3, 3) = aFar / (aFar - aNear);
	//projection(3, 4) = (aFar - aNear) / aFar;
	//projection(4, 3) = -aFar * aNear / (aFar - aNear);
	//projection(4, 4) = 0.f;



	//const float vFoVRad = 2.0f * std::atan(std::tan(0.5f * aFOVRad) / aAspectRatio);

	//const float yScale = 1.0f / std::tan(0.5f * vFoVRad);
	//const float xScale = yScale / aAspectRatio;
	//const float zRange = aFar / (aFar - aNear);

	const float tangent = std::tan(0.5f * aFOVRad);

	const float yScale = aAspectRatio / tangent;
	const float xScale = 1.f / tangent;
	const float zRange = aFar / (aFar - aNear);

	projection(1, 1) = xScale;
	projection(2, 2) = yScale;
	projection(3, 3) = zRange;
	projection(3, 4) = 1.0f;
	projection(4, 3) = -zRange * aNear;
	projection(4, 4) = 0.0f;

	return projection;
}
