#pragma once
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>

namespace gengine
{
	struct Camera
	{
		CommonUtilities::Matrix4x4<float> Transform;
		CommonUtilities::Matrix4x4<float> Projection;
	};
}
