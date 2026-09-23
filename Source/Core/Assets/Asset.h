#pragma once

namespace assets
{
	class Asset
	{
	public:
		virtual bool Load(const wchar_t* aPath) = 0;
		virtual bool Save(const wchar_t* aPath) = 0;
	};
}