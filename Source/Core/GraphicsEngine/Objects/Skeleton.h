#pragma once
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>
#include <unordered_map>
#include <string>

struct Skeleton
{
	struct Bone
	{
		short Parent;
		std::string Name;
		CommonUtilities::Matrix4x4f BindPoseInverse;
	};

	std::unordered_map<std::string, short> BoneIndexMap;
	std::vector<Bone> Bones;
};