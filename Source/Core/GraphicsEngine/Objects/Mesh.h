#pragma once
#include <Core/RenderHardwareInterface/PImpl.hpp>
#include <Core/RenderHardwareInterface/Objects/Buffers/VertexBuffer.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/IndexBuffer.h>
#include <Core/CommonUtilities/Math/Sphere.hpp>

namespace graphics
{
	class VertexBuffer;
	class IndexBuffer;
}

struct Mesh
{
	size_t VertexStride;
	graphics::PImpl<graphics::VertexBuffer> VertexBuffer;
	size_t myNumIndices;
	graphics::PImpl<graphics::IndexBuffer> IndexBuffer;
	CommonUtilities::Sphere<float> EncompassingSphere;
};