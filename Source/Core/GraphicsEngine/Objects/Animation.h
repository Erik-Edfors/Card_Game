#pragma once
#include "../GraphicsEngineDefs.h"
#include <vector>
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>
#include <Core/CommonUtilities/Math/Quaternion.hpp>

struct Joint
{
	CommonUtilities::Vector3<float> Translation;
	CommonUtilities::Vector3<float> Scale;
	CommonUtilities::Quaternion<float> Rotation;
};

struct Animation
{
	struct Frame
	{
		float Time;
		std::vector<Joint> Joints;
	};

	std::vector<Frame> Frames;
	std::vector<std::string> BoneNames;
	float Duration;
};