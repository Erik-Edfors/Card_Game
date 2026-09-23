#include "GraphicsEngine.pch.h"
#include "ShaderReflection.h"

#include <d3dcompiler.h>
#include <wrl.h>

#undef FILE_CATEGORY
#define FILE_CATEGORY "ShaderReflection.cpp"

static RegisterType GetRegisterType(D3D_SHADER_INPUT_TYPE aType)
{
#define LOG_CATEGORY FILE_CATEGORY"->GetRegisterType"
	switch (aType)
	{
	case D3D_SIT_CBUFFER: return RegisterType::ConstantBuffer;
	case D3D_SIT_STRUCTURED: return RegisterType::ConstantBuffer;
	case D3D_SIT_TBUFFER: return RegisterType::ResourceView;
	case D3D_SIT_TEXTURE: return RegisterType::ResourceView;
	case D3D_SIT_UAV_RWTYPED: return RegisterType::UnorderedAccessView;
	case D3D_SIT_UAV_APPEND_STRUCTURED: return RegisterType::UnorderedAccessView;
	case D3D_SIT_UAV_CONSUME_STRUCTURED: return RegisterType::UnorderedAccessView;
	case D3D_SIT_UAV_FEEDBACKTEXTURE: return RegisterType::UnorderedAccessView;
	case D3D_SIT_UAV_RWBYTEADDRESS: return RegisterType::UnorderedAccessView;
	case D3D_SIT_UAV_RWSTRUCTURED: return RegisterType::UnorderedAccessView;
	case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER: return RegisterType::UnorderedAccessView;
	case D3D_SIT_SAMPLER: return RegisterType::SamplerState;

	case D3D_SIT_BYTEADDRESS: return RegisterType::ConstantBuffer;	//TODO: Is this right???
	default:
		LOG_WARNING(LOG_CATEGORY, "Encountered unknown type! Type %u.", aType);
		return RegisterType::Unknown;
	}
#undef LOG_CATEGORY
}

static ResourceType GetResourceType(D3D_SHADER_INPUT_TYPE aType, D3D_SHADER_INPUT_FLAGS aFlags, D3D_SRV_DIMENSION aDimension)
{
#define LOG_CATEGORY FILE_CATEGORY"->GetResourceType"
	switch (aType)
	{
	case D3D_SIT_TBUFFER: case D3D_SIT_TEXTURE:
		switch (aDimension)
		{
		case D3D_SRV_DIMENSION_TEXTURE1D: case D3D_SRV_DIMENSION_TEXTURE1DARRAY:
			return ResourceType::Texture1D;
		case D3D_SRV_DIMENSION_TEXTURE2D: case D3D_SRV_DIMENSION_TEXTURE2DARRAY: case D3D_SRV_DIMENSION_TEXTURECUBE: case D3D_SRV_DIMENSION_TEXTURECUBEARRAY:
		case D3D_SRV_DIMENSION_TEXTURE2DMS: case D3D_SRV_DIMENSION_TEXTURE2DMSARRAY:	//TODO: Is this right?
			return ResourceType::Texture2D;
		case D3D_SRV_DIMENSION_TEXTURE3D:
			return ResourceType::Texture3D;
		default:
			LOG_WARNING(LOG_CATEGORY, "Encountered unknown dimension type! Type %u.", aDimension);
			return ResourceType::Unknown;
		}

	case D3D_SIT_CBUFFER: case D3D_SIT_STRUCTURED:
	case D3D_SIT_BYTEADDRESS:	//TODO: Is this right?
		return ResourceType::Buffer;

	case D3D_SIT_SAMPLER:
		if (aFlags & D3D_SIF_COMPARISON_SAMPLER)
		{
			return ResourceType::SamplerComparisonState;
		}
		else
		{
			return ResourceType::SamplerState;
		}

	default:
		LOG_WARNING(LOG_CATEGORY, "Encountered unknown resource type! Type %u.", aType);
		return ResourceType::Unknown;
	}
#undef LOG_CATEGORY
}

static ShaderType GetShaderType(UINT aShaderVersion)
{
#define LOG_CATEGORY FILE_CATEGORY"->GetShaderType"
	switch (D3D11_SHVER_GET_TYPE(aShaderVersion))
	{
	case D3D11_SHVER_PIXEL_SHADER: return ShaderType::Pixel;
	case D3D11_SHVER_VERTEX_SHADER: return ShaderType::Vertex;
	case D3D11_SHVER_GEOMETRY_SHADER: return ShaderType::Geometry;
	case D3D11_SHVER_HULL_SHADER: return ShaderType::Hull;
	case D3D11_SHVER_DOMAIN_SHADER: return ShaderType::Domain;
	case D3D11_SHVER_COMPUTE_SHADER: return ShaderType::Compute;
	default:
		LOG_WARNING(LOG_CATEGORY, "Failed to resolve shader type! Shader Version: 0x%x", aShaderVersion);
		return ShaderType::Unknown;
	}
#undef LOG_CATEGORY
}

static FieldType GetFieldType(D3D_SHADER_VARIABLE_CLASS aClass, D3D_SHADER_VARIABLE_TYPE aType)
{
#define LOG_CATEGORY FILE_CATEGORY"->GetFieldType"
	switch (aClass)
	{
	case D3D_SVC_STRUCT: return FieldType::Structure;
	default:
		switch (aType)
		{
		case D3D_SVT_BOOL: return FieldType::Bool;
		case D3D_SVT_FLOAT: return FieldType::Float;
		case D3D_SVT_INT: return FieldType::Int;
		case D3D_SVT_UINT: return FieldType::UInt;
		default:
			LOG_WARNING(LOG_CATEGORY, "Failed to resolve field type! Class: 0x%x, Type: 0x%x", aClass, aType);
			return FieldType::Unknown;
		}
	}
#undef LOG_CATEGORY
}

static bool ReflectStructureType(ID3D11ShaderReflectionType* aType, ShaderReflection* inoutReflection)
{
#define LOG_CATEGORY FILE_CATEGORY"->ReflectStructureType"
	if (aType == nullptr)
	{
		LOG_ERROR(LOG_CATEGORY, STRINGIFY(aType) " cannot be nullptr!");
		return false;
	}
	if (inoutReflection == nullptr)
	{
		LOG_ERROR(LOG_CATEGORY, STRINGIFY(inoutReflection) " cannot be nullptr!");
		return false;
	}

	D3D11_SHADER_TYPE_DESC desc = {};
	HRESULT result = aType->GetDesc(&desc);
	if (FAILED(result))
	{
		LOG_ERROR(LOG_CATEGORY, STRINGIFY(ID3D11ShaderReflectionType::GetDesc) " Failed! Error: 0x%x.", result);
		return false;
	}

	if (desc.Class != D3D_SVC_STRUCT)
	{
		return false; // type is not a struct.
	}

	// TODO: Finish recursive struct reflection.
#undef LOG_CATEGORY
}

static bool ReflectBuffer(ID3D11ShaderReflectionConstantBuffer* aReflection, ShaderReflection* inoutReflection)
{
#define LOG_CATEGORY FILE_CATEGORY"->ReflectBuffer"
	if (aReflection == nullptr)
	{
		LOG_ERROR(LOG_CATEGORY, STRINGIFY(aReflection) " cannot be nullptr!");
		return false;
	}
	if (inoutReflection == nullptr)
	{
		LOG_ERROR(LOG_CATEGORY, STRINGIFY(inoutReflection) " cannot be nullptr!");
		return false;
	}

	// TODO: Finish buffer reflection using above structure type reflection function.

	BufferReflection buffer = {};
	StructureReflection structure = {};

	D3D11_SHADER_BUFFER_DESC desc = {};
	HRESULT result = aReflection->GetDesc(&desc);
	if (FAILED(result))
	{
		LOG_ERROR(LOG_CATEGORY, "Call to ID3D11ShaderReflectionConstantBuffer::GetDesc failed! Code: %x.", result);
		return false;
	}

	buffer.Signature = desc.Name;

	for (UINT vInd = 0; vInd < desc.Variables; vInd++)
	{
		StructureMemberReflection member = {};

		ID3D11ShaderReflectionVariable* variable = aReflection->GetVariableByIndex(vInd);
		D3D11_SHADER_VARIABLE_DESC variableDesc = {};
		variable->GetDesc(&variableDesc);

		ID3D11ShaderReflectionType* type = variable->GetType();
		D3D11_SHADER_TYPE_DESC typeDesc = {};
		type->GetDesc(&typeDesc);

		member.MemoryOffset = variableDesc.StartOffset;
		member.Field.Signature = variableDesc.Name;
		member.Field.Type = GetFieldType(typeDesc.Class, typeDesc.Type);
		member.Field.ArraySize = typeDesc.Elements;

		// TODO: Handle structure members.

		structure.Members.push_back(member);
	}

	buffer.StructureIndex = static_cast<unsigned int>(inoutReflection->Structures.size());
	inoutReflection->Structures.push_back(structure);
	inoutReflection->Buffers.push_back(buffer);

	return true;

#undef LOG_CATEGORY
}

bool ReflectShader(const void* aByteCode, size_t aByteCodeSize, ShaderReflection* outReflection)
{
#define LOG_CATEGORY FILE_CATEGORY"->ReflectShader"

	if (aByteCode == nullptr || outReflection == nullptr || aByteCodeSize == 0)
	{
		return false;
	}

	ShaderReflection output = {};

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflection;

	HRESULT result = D3DReflect(aByteCode, aByteCodeSize, __uuidof(ID3D11ShaderReflection), &reflection);
	if (FAILED(result))
	{
		LOG_ERROR(LOG_CATEGORY, "Call to D3DReflect failed! Code: %x.", result);
		return false;
	}

	D3D11_SHADER_DESC shaderDesc = {};
	result = reflection->GetDesc(&shaderDesc);
	if (FAILED(result))
	{
		LOG_ERROR(LOG_CATEGORY, "Call to ID3D11ShaderReflection::GetDesc failed! Code: %x.", result);
		return false;
	}

	// Get shader type.
	output.Type = GetShaderType(shaderDesc.Version);

	// Iterate over resources
	// TODO: Replace with dedicated function.
	for (UINT i = 0; i < shaderDesc.BoundResources; i++)
	{
		ResourceReflection resource = {};

		D3D11_SHADER_INPUT_BIND_DESC bindDesc = {};
		result = reflection->GetResourceBindingDesc(i, &bindDesc);
		if (FAILED(result))
		{
			LOG_ERROR(LOG_CATEGORY, "Call to ID3D11ShaderReflection::GetResourceBindingDesc failed! Code: %x.", result);
			return false;
		}

		resource.Register = RegisterReflection{
			.Type = GetRegisterType(bindDesc.Type),
			.Index = bindDesc.BindPoint,
			.Count = bindDesc.BindCount
		};
		resource.Signature = bindDesc.Name;
		resource.Type = GetResourceType(bindDesc.Type, static_cast<D3D_SHADER_INPUT_FLAGS>(bindDesc.uFlags), bindDesc.Dimension);

		output.Resources.push_back(std::move(resource));
	}

	// TODO: Replace with dedicated function
	for (UINT cInd = 0; cInd < shaderDesc.ConstantBuffers; cInd++)
	{
		BufferReflection buffer = {};
		StructureReflection structure = {};

		ID3D11ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(cInd);

		D3D11_SHADER_BUFFER_DESC desc = {};
		result = constantBuffer->GetDesc(&desc);
		if (FAILED(result))
		{
			LOG_ERROR(LOG_CATEGORY, "Call to ID3D11ShaderReflectionConstantBuffer::GetDesc failed! Code: %x.", result);
			return false;
		}

		buffer.Signature = desc.Name;

		for (UINT vInd = 0; vInd < desc.Variables; vInd++)
		{
			StructureMemberReflection member = {};

			ID3D11ShaderReflectionVariable* variable = constantBuffer->GetVariableByIndex(vInd);
			D3D11_SHADER_VARIABLE_DESC variableDesc = {};
			variable->GetDesc(&variableDesc);

			ID3D11ShaderReflectionType* type = variable->GetType();
			D3D11_SHADER_TYPE_DESC typeDesc = {};
			type->GetDesc(&typeDesc);

			member.MemoryOffset = variableDesc.StartOffset;
			member.Field.Signature = variableDesc.Name;
			member.Field.Type = GetFieldType(typeDesc.Class, typeDesc.Type);
			member.Field.ArraySize = typeDesc.Elements;

			// TODO: Handle structure members.

			structure.Members.push_back(member);
		}

		buffer.StructureIndex = static_cast<unsigned int>(output.Structures.size());
		output.Structures.push_back(structure);
		output.Buffers.push_back(buffer);
	}

	*outReflection = output;

	return true;
#undef LOG_CATEGORY
}

#undef FILE_CATEGORY