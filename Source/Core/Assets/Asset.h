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

		void Acquire() noexcept;
		void Release() noexcept;

		size_t GetReferenceCount() const noexcept;

	protected:
		Asset();

	private:
		ASSET_RETENTION myRetention;
		size_t myReferenceCount;
	};
}