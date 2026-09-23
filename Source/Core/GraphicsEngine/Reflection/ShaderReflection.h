#pragma once
#include <string>
#include <vector>

enum class ShaderType : uint8_t
{
	Unknown,
	Vertex,
	Hull,
	Domain,
	Geometry,
	Pixel,
	Compute
};

enum class FieldType : uint8_t
{
	Unknown,
	Structure,
	Float,
	Int,
	UInt,
	Bool
};

enum class ResourceType : uint8_t
{
	Unknown,
	Texture1D,
	Texture2D,
	Texture3D,
	SamplerState,
	SamplerComparisonState,
	Buffer
};

enum class RegisterType : uint8_t
{
	Unknown,
	ConstantBuffer,
	ResourceView,
	UnorderedAccessView,
	SamplerState,
};

struct RegisterReflection
{
	RegisterType Type;
	unsigned int Index;
	unsigned int Count;
};

struct FieldReflection
{
	FieldType Type;
	std::string Signature;
	unsigned int ArraySize;
	unsigned int StructureIndex;
};

struct StructureMemberReflection
{
	unsigned int MemoryOffset;
	FieldReflection Field;
};

struct StructureReflection
{
	std::vector<StructureMemberReflection> Members;
};

struct BufferReflection
{
	std::string Signature;
	unsigned int StructureIndex;
};

struct ResourceReflection
{
	ResourceType Type;
	std::string Signature;
	RegisterReflection Register;
};

struct ShaderReflection
{
	ShaderType Type;
	std::vector<FieldReflection> StaticFields;
	std::vector<StructureReflection> Structures;
	std::vector<ResourceReflection> Resources;
	std::vector<BufferReflection> Buffers;
};

bool ReflectShader(const void* aByteCode, size_t aByteCodeSize, ShaderReflection* outReflection);