#pragma once
#include "GraphicsObject.h"
#include "../Result.h"

namespace graphics
{
	class Device;

	struct Texture2DDesc
	{
		GRAPHICS_BIND_FLAGS BindFlags;										//Creation flags for the texture.
		GRAPHICS_RESOURCE_FLAGS ResourceFlags;								//Creation flags for the texture.
		std::string Name;													//A name to assign the object.
		unsigned int Width;													//The width of the texture.
		unsigned int Height;												//The height of the texture.
		unsigned int MipLevels;												//The amount of mip levels to create with the texture.
		unsigned int NumTextures;											//The amount of textures.
		const TextureSubresourceData* Data;									//An array of structures describing data of the textures subresources.
		size_t NumData;														//The number of elements in the aData array.
		GRAPHICS_FORMAT Format;												//The format of the texture.
		GRAPHICS_USAGE Usage;												//How the texture will be used.
		GRAPHICS_CPU_ACCESS_FLAGS CPUAccessFlags;							//CPU Access flags.
	};

	class Texture2D : public GraphicsObject
	{
	public:
		const Texture2DDesc& GetDesc() const;
		unsigned int GetVersion() const;

		virtual void SetName(const char* aName) override;

	protected:
		Texture2D(GraphicsID aID, const Texture2DDesc& aDesc);

		void SetVersion(unsigned int aVersion);
		void SetDesc(const Texture2DDesc& aDesc);
	private:
		Texture2DDesc myDesc;
		unsigned int myVersion;
	};
}