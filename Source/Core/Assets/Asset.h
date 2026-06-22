#pragma once
#include <memory>
#include <istream>

namespace assets
{
	enum ASSET_RETENTION : uint8_t
	{
		ASSET_RETENTION_WEAK,
		ASSET_RETENTION_MODERATE,
		ASSET_RETENTION_STRONG
	};

	class Asset
	{
	public:
		virtual ~Asset() noexcept = default;

		virtual bool Load(const char* aFile) = 0;

		void SetRetention(ASSET_RETENTION aRetention) noexcept;
		ASSET_RETENTION GetRetention() const noexcept;

	protected:
		Asset() = default;

	private:
		ASSET_RETENTION myRetention;
	};
}