#pragma once
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>
#include <Core/CommonUtilities/Math/Vector4.hpp>

struct ShadowMapBuffer
{
    CommonUtilities::Matrix4x4<float> View;
    CommonUtilities::Matrix4x4<float> Projection;
    CommonUtilities::Vector4<float> Position;
};