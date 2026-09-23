#pragma once
#include "GraphicsDefs.h"
#include "Result.h"

namespace graphics
{
	class Device;
	class Texture2D;
	class Texture2DArray;

	namespace detail
	{
		graphics::result::Code ReadFile(const wchar_t* aPath, char** outBuffer, size_t* outSize);
	}

	result::Code CreateTexture2DFromFile(
		Device* aDevice, 													//A device to create the texture on
		const char* aName,													//The name the texture will have after creation
		const wchar_t* aFilePath,											//The path to the file containing the texture.
		Texture2D** outTexture2D,											//A pointer to a Texture2D interface that will recieve the created texture.
		GRAPHICS_BIND_FLAGS aFlags,											//Bind flags for the texture.
		GRAPHICS_RESOURCE_FLAGS aResourceFlags,								//Resource flags for the texture.
		GRAPHICS_USAGE aUsage,												//How the texture will be used.
		GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags							//CPU Access flags.
	);

	result::Code CreateTexture2DFromDDS(
		Device* aDevice, 													//A device to create the texture on
		const char* aName,													//The name the texture will have after creation
		const void* aData,													//A pointer to a loaded dds texture in memory
		size_t aDataSize,													//The size of memory to read from aData
		Texture2D** outTexture2D,											//A pointer to a Texture2D interface that will recieve the created texture.
		GRAPHICS_BIND_FLAGS aBindFlags,										//Bind flags for the texture.
		GRAPHICS_RESOURCE_FLAGS aResourceFlags,								//Resource flags for the texture.
		GRAPHICS_USAGE aUsage,												//How the texture will be used.
		GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags							//CPU Access flags.
	);
}