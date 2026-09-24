#pragma once

namespace networking
{
	class NetworkObject
	{
	public:
		virtual void AddRef() noexcept = 0;
		virtual void Release() noexcept = 0;
		virtual unsigned int GetRefCount() const noexcept = 0;
	};
}