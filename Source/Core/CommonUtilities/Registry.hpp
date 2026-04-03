#pragma once
#include <unordered_map>
#include <vector>

namespace CommonUtilities
{
	template<typename _KeyTy, typename _ItemTy>
	class Registry
	{
	public:
		typedef _KeyTy key;
		typedef _ItemTy item;

		typedef typename std::vector<item>::iterator iterator;
		typedef typename std::vector<item>::const_iterator const_iterator;

		Registry() = default;
		~Registry() = default;

		// If register is smaller than aSize, register will reallocate with the new size.
		void reserve(
			size_t aSize		// The targeted size of the register.
		);

		size_t size() const;
		bool empty() const;

		void insert(const key& aKey, item&& aItem);
		void insert(const key& aKey, const item& aItem);

		bool erase(const key& aKey);
		iterator erase(iterator aWhere);
		const_iterator erase(const_iterator aWhere);
		void clear();

		item& at(const key& aKey);
		const item& at(const key& aKey) const;

		item* find(const key& aKey);
		const item* find(const key& aKey) const;

		iterator begin();
		const_iterator begin() const;

		iterator end();
		const_iterator end() const;

		const std::vector<item>& GetItems() const;

		item& operator[](const key& aKey);
		const item& operator[](const key& aKey) const;

	private:
		void _push_back(const key& aKey, item&& aItem);
		void _push_back(const key& aKey, const item& aItem);

		void _pop_back();

		std::unordered_map<key, size_t> myIndexTable;
		std::vector<item> myItems;
		std::vector<key> myKeys;
	};
	template<typename _KeyTy, typename _ItemTy>
	inline void Registry<_KeyTy, _ItemTy>::reserve(size_t aSize)
	{
		myItems.reserve(aSize);
		myKeys.reserve(aSize);
		myIndexTable.reserve(2 * aSize);
	}
	template<typename _KeyTy, typename _ItemTy>
	inline size_t Registry<_KeyTy, _ItemTy>::size() const
	{
		return myItems.size();
	}
	template<typename _KeyTy, typename _ItemTy>
	inline bool Registry<_KeyTy, _ItemTy>::empty() const
	{
		return myItems.empty();
	}
	template<typename _KeyTy, typename _ItemTy>
	inline void Registry<_KeyTy, _ItemTy>::insert(const key& aKey, item&& aItem)
	{
		auto it = myIndexTable.find(aKey);
		if (it == myIndexTable.end())
		{
			myIndexTable.emplace(aKey, size());
			_push_back(aKey, std::move(aItem));
		}
		else
		{
			myItems[it->second] = std::move(aItem);
		}
	}
	template<typename _KeyTy, typename _ItemTy>
	inline void Registry<_KeyTy, _ItemTy>::insert(const key& aKey, const item& aItem)
	{
		auto it = myIndexTable.find(aKey);
		if (it == myIndexTable.end())
		{
			myIndexTable.emplace(aKey, size());
			_push_back(aKey, aItem);
		}
		else
		{
			myItems[it->second] = aItem;
		}
	}
	template<typename _KeyTy, typename _ItemTy>
	inline bool Registry<_KeyTy, _ItemTy>::erase(const key& aKey)
	{
		auto indexIt = myIndexTable.find(aKey);
		if (indexIt == myIndexTable.end())
		{
			return false;
		}
		size_t index = indexIt->second;
		myIndexTable.erase(aKey);
		if (size() > 1)
		{
			myItems[index] = std::move(myItems.back());
			myKeys[index] = std::move(myKeys.back());
			myIndexTable[myKeys[index]] = index;
		}
		_pop_back();
		return true;
	}
	template<typename _KeyTy, typename _ItemTy>
	inline typename Registry<_KeyTy, _ItemTy>::iterator Registry<_KeyTy, _ItemTy>::erase(iterator aWhere)
	{
		size_t index = aWhere - begin();
		erase(myKeys[index]);
		return begin() + index;
	}
	template<typename _KeyTy, typename _ItemTy>
	inline typename Registry<_KeyTy, _ItemTy>::const_iterator Registry<_KeyTy, _ItemTy>::erase(const_iterator aWhere)
	{
		size_t index = aWhere - begin();
		erase(myKeys[index]);
		return begin() + index;
	}
	template<typename _KeyTy, typename _ItemTy>
	inline void Registry<_KeyTy, _ItemTy>::clear()
	{
		myIndexTable.clear();
		myItems.clear();
		myKeys.clear();
	}
	template<typename _KeyTy, typename _ItemTy>
	inline typename Registry<_KeyTy, _ItemTy>::item& Registry<_KeyTy, _ItemTy>::at(const key& aKey)
	{
		return myItems[myIndexTable.at(aKey)];
	}
	template<typename _KeyTy, typename _ItemTy>
	inline const typename Registry<_KeyTy, _ItemTy>::item& Registry<_KeyTy, _ItemTy>::at(const key& aKey) const
	{
		return myItems[myIndexTable.at(aKey)];
	}
	template<typename _KeyTy, typename _ItemTy>
	inline typename Registry<_KeyTy, _ItemTy>::item* Registry<_KeyTy, _ItemTy>::find(const key& aKey)
	{
		auto indexIt = myIndexTable.find(aKey);
		if (indexIt == myIndexTable.end())
		{
			return nullptr;
		}
		size_t index = indexIt->second;
		return &myItems[index];
	}
	template<typename _KeyTy, typename _ItemTy>
	inline const typename Registry<_KeyTy, _ItemTy>::item* Registry<_KeyTy, _ItemTy>::find(const key& aKey) const
	{
		auto indexIt = myIndexTable.find(aKey);
		if (indexIt == myIndexTable.end())
		{
			return nullptr;
		}
		size_t index = indexIt->second;
		return &myItems[index];
	}
	template<typename _KeyTy, typename _ItemTy>
	inline typename Registry<_KeyTy, _ItemTy>::iterator Registry<_KeyTy, _ItemTy>::begin()
	{
		return myItems.begin();
	}
	template<typename _KeyTy, typename _ItemTy>
	inline typename Registry<_KeyTy, _ItemTy>::const_iterator Registry<_KeyTy, _ItemTy>::begin() const
	{
		return myItems.begin();
	}
	template<typename _KeyTy, typename _ItemTy>
	inline typename Registry<_KeyTy, _ItemTy>::iterator Registry<_KeyTy, _ItemTy>::end()
	{
		return myItems.end();
	}
	template<typename _KeyTy, typename _ItemTy>
	inline typename Registry<_KeyTy, _ItemTy>::const_iterator Registry<_KeyTy, _ItemTy>::end() const
	{
		return myItems.end();
	}
	template<typename _KeyTy, typename _ItemTy>
	inline const std::vector<typename Registry<_KeyTy, _ItemTy>::item>& Registry<_KeyTy, _ItemTy>::GetItems() const
	{
		return myItems;
	}
	template<typename _KeyTy, typename _ItemTy>
	inline typename Registry<_KeyTy, _ItemTy>::item& Registry<_KeyTy, _ItemTy>::operator[](const key& aKey)
	{
		return myItems[myIndexTable.at(aKey)];
	}
	template<typename _KeyTy, typename _ItemTy>
	inline const typename Registry<_KeyTy, _ItemTy>::item& Registry<_KeyTy, _ItemTy>::operator[](const key& aKey) const
	{
		return myItems[myIndexTable.at(aKey)];
	}
	template<typename _KeyTy, typename _ItemTy>
	inline void Registry<_KeyTy, _ItemTy>::_push_back(const key& aKey, item&& aItem)
	{
		myKeys.push_back(aKey);
		myItems.push_back(std::move(aItem));
	}
	template<typename _KeyTy, typename _ItemTy>
	inline void Registry<_KeyTy, _ItemTy>::_push_back(const key& aKey, const item& aItem)
	{
		myKeys.push_back(aKey);
		myItems.push_back(std::move(aItem));
	}
	template<typename _KeyTy, typename _ItemTy>
	inline void Registry<_KeyTy, _ItemTy>::_pop_back()
	{
		myKeys.pop_back();
		myItems.pop_back();
	}
}