#pragma once
#include "Registry.hpp"
#include <functional>

namespace CommonUtilities
{
	typedef size_t EventKey;

	template<typename ..._Args>
	class Event
	{
	public:
		Event();
		~Event() = default;

		EventKey Subscribe(const std::function<void(_Args...)>& aFunction);
		EventKey operator+=(const std::function<void(_Args...)>& aFunction);

		EventKey Subscribe(std::function<void(_Args...)>&& aFunction);
		EventKey operator+=(std::function<void(_Args...)>&& aFunction);

		bool Unsubscribe(EventKey aKey);
		bool operator-=(EventKey aKey);

		void Clear();
		void operator()(_Args... aArgs);

	private:
		EventKey _reserveKey();

		EventKey myCurrentKey;
		Registry<Key, std::function<void(_Args...)>> mySubscribers;
	};
	template<typename ..._Args>
	inline Event<_Args...>::Event() : 
		myCurrentKey(0ull)
	{
	}
	template<typename ..._Args>
	inline EventKey Event<_Args...>::Subscribe(const std::function<void(_Args...)>& aFunction)
	{
		EventKey EventKey = _reserveKey();
		mySubscribers.emplace(key, aFunction);
		return key;
	}
	template<typename ..._Args>
	inline EventKey Event<_Args...>::operator+=(const std::function<void(_Args...)>& aFunction)
	{
		return Subscribe(aFunction);
	}
	template<typename ..._Args>
	inline EventKey Event<_Args...>::Subscribe(std::function<void(_Args...)>&& aFunction)
	{
		EventKey EventKey = _reserveKey();
		mySubscribers.emplace(key, aFunction);
		return key;
	}
	template<typename ..._Args>
	inline EventKey Event<_Args...>::operator+=(std::function<void(_Args...)>&& aFunction)
	{
		return Subscribe(aFunction);
	}
	template<typename ..._Args>
	inline bool Event<_Args...>::Unsubscribe(EventKey aKey)
	{
		return mySubscribers.erase(aKey);
	}
	template<typename ..._Args>
	inline bool Event<_Args...>::operator-=(EventKey aKey)
	{
		return Unsubscribe(aKey);
	}
	template<typename ..._Args>
	inline void Event<_Args...>::Clear()
	{
		mySubscribers.clear();
	}
	template<typename ..._Args>
	inline void Event<_Args...>::operator()(_Args ...aArgs)
	{
		for (auto& subscriber : mySubscribers)
		{
			subscriber.operator()(aArgs...);
		}
	}
	template<typename ..._Args>
	inline EventKey Event<_Args...>::_reserveKey()
	{
		return myCurrentKey++;
	}
}