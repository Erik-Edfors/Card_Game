#pragma once
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>

CommonUtilities::Matrix4x4<float> CreateOrthographicProjection(float aLeft, float aRight, float aBottom, float aTop, float aNear, float aFar);

CommonUtilities::Matrix4x4<float> CreatePerspectiveProjection(float aFOVRad, float aAspectRatio, float aNear, float aFar);