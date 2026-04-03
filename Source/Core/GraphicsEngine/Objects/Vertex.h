#pragma once
#include <Core/RenderHardwareInterface/GraphicsDefs.h>
#include <Core/RenderHardwareInterface/Objects/InputLayout.h>

#define VERTEX_MAX_SKIN_WEIGHTS 4

struct Vertex
{
	Vertex() = default;
	Vertex(
		float x, float y, float z, float w, 
		float nx, float ny, float nz, float nw, 
		float tx, float ty, float tz, float tw, 
		int b1, int b2, int b3, int b4, 
		float w1, float w2, float w3, float w4, 
		float u, float v
	);

	float Position[4];
	float Normal[4];
	float Tangent[4];
	int BoneIDs[VERTEX_MAX_SKIN_WEIGHTS];
	float SkinWeights[VERTEX_MAX_SKIN_WEIGHTS];
	float UV[2];

	//inline static const graphics::LayoutDefinition Layout = {
	//	{ "POSITION", 0u, graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT },
	//	{ "NORMAL", 0u, graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT },
	//	{ "TANGENT", 0u, graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT },
	//	{ "BONEIDS", 0u, graphics::GRAPHICS_FORMAT_R32G32B32A32_SINT },
	//	{ "SKINWEIGHTS", 0u, graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT },
	//	{ "UV", 0u, graphics::GRAPHICS_FORMAT_R32G32_FLOAT },
	//};
};