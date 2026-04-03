#pragma once
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>
#include <Core/CommonUtilities/Math/Vector3.hpp>

//#define LB_MAX_DIRECTIONAL_LIGHTS 1
//#define LB_MAX_POINT_LIGHTS 4
//#define LB_MAX_SPOT_LIGHTS 4

struct LightBuffer
{
    CommonUtilities::Matrix4x4<float> View;
    CommonUtilities::Matrix4x4<float> Projection;

    CommonUtilities::Vector3<float> Color;
    float Intensity;

    CommonUtilities::Vector3<float> Position;
    float CastShadows;
    CommonUtilities::Vector3<float> Direction;
    float Radius;
    float InnerConeAngle;
    float OuterConeAngle;

	unsigned int _padding[2];
};

struct DirectionalLightData
{
	CommonUtilities::Matrix4x4<float> View;
	CommonUtilities::Matrix4x4<float> Projection;

	CommonUtilities::Vector3<float> Color;
	float Intensity;

	CommonUtilities::Vector3<float> Direction;
	float CastShadows;

private:
};

struct PointLightData
{
	CommonUtilities::Vector3<float> Color;
	float Intensity;

	CommonUtilities::Vector3<float> Position;
	float CastShadows;

	float Radius;

private:
	int _padding[3];
};

struct SpotLightData
{
	CommonUtilities::Matrix4x4<float> View;
	CommonUtilities::Matrix4x4<float> Projection;

	CommonUtilities::Vector3<float> Color;
	float Intensity;

	CommonUtilities::Vector3<float> Position;
	float CastShadows;

	CommonUtilities::Vector3<float> Direction;
	float Radius;

	float InnerConeAngle;
	float OuterConeAngle;

private:
	int _padding[2];
};
//
//struct LightBuffer
//{
//	DirectionalLightData DirectionalLights[LB_MAX_DIRECTIONAL_LIGHTS];
//
//	PointLightData PointLights[LB_MAX_POINT_LIGHTS];
//
//	SpotLightData SpotLights[LB_MAX_SPOT_LIGHTS];
//
//	unsigned int NumDirectionalLights;
//	unsigned int NumPointLights;
//	unsigned int NumSpotLights;
//	
//	unsigned int _padding[1];
//};