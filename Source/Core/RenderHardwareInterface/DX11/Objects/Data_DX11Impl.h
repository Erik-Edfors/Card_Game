#pragma once
#include "../../Objects/Data.h"

namespace graphics
{
	class Data_DX11Impl : public Data
	{
	public:
		Data_DX11Impl(GraphicsID aID, const DataDesc& aDesc);

		void SetName(const char* aName) override;
	};
}
