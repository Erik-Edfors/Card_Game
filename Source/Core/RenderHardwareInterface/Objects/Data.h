#pragma once
#include "GraphicsObject.h"
#include "../PImpl.hpp"

namespace graphics
{
	class Texture2D;

	struct DataDesc
	{
		std::string Name;
		void* Data;
		size_t Size;
	};

	class Data : public GraphicsObject
	{
	public:
		~Data();

		const void* GetData() const;
		size_t GetSize() const;

		const DataDesc& GetDesc() const;

	protected:
		Data(GraphicsID aID, const DataDesc& aDesc);

		DataDesc myDesc;
	};
}