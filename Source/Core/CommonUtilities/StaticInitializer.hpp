#pragma once
#include <utility>

namespace CommonUtilities
{
	template<class _Initializer, class ..._Args>
	class StaticInitializer
	{
	public:
		using Initializer = _Initializer;

		StaticInitializer(Initializer aInitializer, _Args&&... aArgs)
		{
			(aInitializer)(std::forward<_Args>(aArgs)...);
		}
	};
}