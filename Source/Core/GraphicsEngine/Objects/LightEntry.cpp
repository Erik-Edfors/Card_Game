#include "GraphicsEngine.pch.h"
#include "LightEntry.h"
#include <Core/RenderHardwareInterface/Objects/DepthStencil.h>
#include <Core/RenderHardwareInterface/Objects/ShaderResource.h>

LightEntry::LightEntry() = default;
LightEntry::~LightEntry() = default;

LightEntry::LightData::LightData() = default;
LightEntry::LightData::~LightData() = default;


//void LightEntry::SetView(const CommonUtilities::Matrix4x4<float>& aView)
//{
//	myView = aView;
//}
//
//const CommonUtilities::Matrix4x4<float>& LightEntry::GetView() const
//{
//	return myView;
//}
//
//void LightEntry::SetPosition(const CommonUtilities::Vector3<float>& aPosition)
//{
//	myPosition = aPosition;
//}
//
//const CommonUtilities::Vector3<float>& LightEntry::GetPosition() const
//{
//	return myPosition;
//}
//
//void LightEntry::SetColor(const CommonUtilities::Vector3<float>& aColor)
//{
//	myColor = aColor;
//}
//
//const CommonUtilities::Vector3<float>& LightEntry::GetColor() const
//{
//	return myColor;
//}
//
//void LightEntry::SetIntensity(float aIntensity)
//{
//	myIntensity = aIntensity;
//}
//
//float LightEntry::GetIntensity() const
//{
//	return myIntensity;
//}
//
//void LightEntry::SetRadius(float aRadius)
//{
//	myRadius = aRadius;
//}
//
//float LightEntry::GetRadius() const
//{
//	return myRadius;
//}
//
//void LightEntry::SetCastsShadows(bool aCastsShadows)
//{
//	myCastsShadows = aCastsShadows;
//}
//
//bool LightEntry::GetCastsShadows() const
//{
//	return myCastsShadows;
//}
//
//unsigned int LightEntry::AssignLightBuffer(LightBuffer* aLightBuffer) const
//{
//	if (aLightBuffer == nullptr)
//	{
//		throw GRAPHICS_EXCEPTION("aLightBuffer was nullptr!");
//	}
//	if (aLightBuffer->NumPointLights >= LB_MAX_POINT_LIGHTS)
//	{
//		throw GRAPHICS_EXCEPTION("aLightBuffer LightBuffer::NumPointLights was greater or equal to LB_MAX_POINT_LIGHTS!");
//	}
//
//	aLightBuffer->PointLights[aLightBuffer->NumPointLights].View = myView;
//	aLightBuffer->PointLights[aLightBuffer->NumPointLights].Color = myColor;
//	aLightBuffer->PointLights[aLightBuffer->NumPointLights].Intensity = myIntensity;
//	aLightBuffer->PointLights[aLightBuffer->NumPointLights].Position = myPosition;
//	aLightBuffer->PointLights[aLightBuffer->NumPointLights].CastShadows = myCastsShadows;
//	aLightBuffer->PointLights[aLightBuffer->NumPointLights].Radius = myRadius;
//	return ++aLightBuffer->NumPointLights;
//}
