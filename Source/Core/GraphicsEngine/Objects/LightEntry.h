#pragma once
#include <Core/RenderHardwareInterface/PImpl.hpp>
#include "../GraphicsEngineDefs.h"
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>
#include "../ConstantBuffers/LightBuffer.h"

#include <Core/RenderHardwareInterface/Objects/DepthStencil.h>
#include <Core/RenderHardwareInterface/Objects/ShaderResource.h>

namespace graphics
{
	class DepthStencil;
	class ShaderResource;
}

typedef int LIGHT_TYPE;
enum
{
	LIGHT_TYPE_UNKNOWN = -1,
	LIGHT_TYPE_DIRECTIONAL_LIGHT,
	LIGHT_TYPE_SPOT_LIGHT,
	LIGHT_TYPE_POINT_LIGHT
};

struct LightEntry
{
	LightEntry();
	~LightEntry();

	LIGHT_TYPE LightType;
	graphics::PImpl<graphics::DepthStencil> ShadowDepthStencil;
	graphics::PImpl<graphics::ShaderResource> ShadowResource;

	union LightData
	{
		LightData();
		~LightData();

		PointLightData PointLightData;

		DirectionalLightData DirectionalLightData;

		SpotLightData SpotLightData;
	} Data;
};

//class LightEntry
//{
//public:
//
//	void SetView(const CommonUtilities::Matrix4x4<float>& aView);
//	const CommonUtilities::Matrix4x4<float>& GetView() const;
//
//	void SetPosition(const CommonUtilities::Vector3<float>& aPosition);
//	const CommonUtilities::Vector3<float>& GetPosition() const;
//
//	void SetColor(const CommonUtilities::Vector3<float>& aColor);
//	const CommonUtilities::Vector3<float>& GetColor() const;
//
//	void SetIntensity(float aIntensity);
//	float GetIntensity() const;
//
//	void SetRadius(float aRadius);
//	float GetRadius() const;
//
//	void SetCastsShadows(bool aCastsShadows);
//	bool GetCastsShadows() const;
//
//	unsigned int AssignLightBuffer(LightBuffer* aLightBuffer) const;
//
//private:
//	CommonUtilities::Matrix4x4<float> myView;
//	CommonUtilities::Vector3<float> myPosition;
//	CommonUtilities::Vector3<float> myColor;
//	float myIntensity;
//	float myRadius;
//	bool myCastsShadows;
//};