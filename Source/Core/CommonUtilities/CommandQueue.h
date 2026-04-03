#pragma once
#include <functional>
#include <queue>

namespace CommonUtilities
{
	class CommandQueue
	{
	public:
		template<typename _Func, typename ..._Args>
		void PushCommand(_Func aFunc, _Args&&... aArgs);

		size_t Count() const;
		bool Empty() const;

		void RunCommand();

	private:
		std::queue<std::function<void()>> myQueue;
	};

	template<typename _Func, typename ..._Args>
	inline void CommandQueue::PushCommand(_Func aFunc, _Args && ...aArgs)
	{
		myQueue.push(std::bind(aFunc, aArgs...));
	}
}