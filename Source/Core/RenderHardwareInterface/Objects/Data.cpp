#include "RenderHardwareInterface.pch.h"
#include "Data.h"

namespace graphics
{
	Data::Data(GraphicsID aID, const DataDesc& aDesc) :
		myDesc(aDesc),
		GraphicsObject(aID)
	{
		if (aDesc.Size != 0)
		{
			myDesc.Size = aDesc.Size;
			myDesc.Data = malloc(aDesc.Size);
			memcpy(myDesc.Data, aDesc.Data, aDesc.Size);
		}
		else
		{
			myDesc.Data = nullptr;
			myDesc.Size = 0;
		}
	}
	Data::~Data()
	{
		if (myDesc.Data != nullptr)
		{
			free(myDesc.Data);
			myDesc.Data = nullptr;
		}
	}
	const void* Data::GetData() const
	{
		return myDesc.Data;
	}
	size_t Data::GetSize() const
	{
		return myDesc.Size;
	}
}
