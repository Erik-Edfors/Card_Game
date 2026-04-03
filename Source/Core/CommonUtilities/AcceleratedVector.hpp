#pragma once
#include "ContiguousIterator.hpp"
#include <unordered_map>
#include <vector>

namespace std
{
	template <class _Elem>
	class initializer_list;
}

namespace CommonUtilities
{
	template<class _Kty, class _Ty, class _Hasher = std::hash<_Kty>, class _Keyeq = std::equal_to<_Kty>>
	class AcceleratedVector
	{
	public:
		using hasher = _Hasher;
		using key_type = _Kty;
		using mapped_type = _Ty;
		using key_equal = _Keyeq;

		using value_type = std::pair<key_type, mapped_type>;
		using pointer = value_type*;
		using const_pointer = const pointer;
		using reference = value_type&;
		using const_reference = const reference;

	private:
		using _container_type = std::vector<value_type>;

	public:
		using size_type = typename _container_type::size_type;
		using difference_type = typename _container_type::difference_type;

	private:
		using _hash_table_type = std::unordered_map<key_type, size_type, hasher, key_equal>;

	public:

		using iterator = ContiguousIterator<value_type>;
		using const_iterator = ContiguousIterator<const value_type>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		AcceleratedVector() noexcept(
			std::is_nothrow_default_constructible_v<_hash_table_type>
			&& std::is_nothrow_default_constructible_v<_container_type>
			);
		AcceleratedVector(const std::initializer_list<value_type>& aInitializerList);
		~AcceleratedVector() noexcept(
			std::is_nothrow_destructible_v<_hash_table_type>
			&& std::is_nothrow_destructible_v<_container_type>
			);

		iterator insert(const value_type& aValue) noexcept(
			std::is_nothrow_copy_constructible_v<value_type>
			);
		iterator insert(value_type&& aValue) noexcept(
			std::is_nothrow_move_constructible_v<value_type>
			&& std::is_nothrow_copy_constructible_v<key_type>
			);

		iterator insert(iterator aWhere, const mapped_type& aValue) noexcept(
			std::is_nothrow_copy_assignable_v<mapped_type>
			);
		iterator insert(iterator aWhere, mapped_type&& aValue) noexcept(
			std::is_nothrow_move_assignable_v<mapped_type>
			);

		iterator insert(const_iterator aWhere, const mapped_type& aValue) noexcept(
			std::is_nothrow_copy_assignable_v<mapped_type>
			);
		iterator insert(const_iterator aWhere, mapped_type&& aValue) noexcept(
			std::is_nothrow_move_assignable_v<mapped_type>
			);

		iterator emplace(const key_type& aKey, const mapped_type& aValue) noexcept(
			std::is_nothrow_copy_constructible_v<key_type>
			&& std::is_nothrow_copy_constructible_v<mapped_type>
			);
		iterator emplace(const key_type& aKey, mapped_type&& aValue) noexcept(
			std::is_nothrow_copy_constructible_v<key_type>
			&& std::is_nothrow_move_constructible_v<mapped_type>
			);
		iterator emplace(key_type&& aKey, const mapped_type& aValue) noexcept(
			std::is_nothrow_move_constructible_v<key_type>
			&& std::is_nothrow_copy_constructible_v<mapped_type>
			);
		iterator emplace(key_type&& aKey, mapped_type&& aValue) noexcept(
			std::is_nothrow_move_constructible_v<key_type>
			&& std::is_nothrow_move_constructible_v<mapped_type>
			);

		iterator erase(const key_type& aKey);
		iterator erase(iterator aWhere);
		iterator erase(const_iterator aWhere);

		mapped_type& operator[](const key_type& aKey);
		const mapped_type& operator[](const key_type& aKey) const;

		mapped_type& at(const key_type& aKey);
		const mapped_type& at(const key_type& aKey) const;

		iterator find(const key_type& aKey);
		const_iterator find(const key_type& aKey) const;

		bool contains(const key_type& aKey) const noexcept;

		void clear() noexcept(
			std::is_nothrow_destructible_v<value_type>
			);

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		reverse_iterator rbegin() noexcept;
		reverse_iterator rend() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		const_reverse_iterator rend() const noexcept;

	private:
		_hash_table_type myAccelerationMap;
		_container_type myValues;
	};



	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::AcceleratedVector() noexcept(std::is_nothrow_default_constructible_v<_hash_table_type>&& std::is_nothrow_default_constructible_v<_container_type>)
	{
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::AcceleratedVector(const std::initializer_list<value_type>& aInitializerList) :
		myValues(aInitializerList)
	{
		for (size_type i = 0; i < myValues.size(); i++)
		{
			myAccelerationMap.emplace(myValues.at(i).first, i);
		}
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::~AcceleratedVector() noexcept(std::is_nothrow_destructible_v<_hash_table_type>&& std::is_nothrow_destructible_v<_container_type>)
	{
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::insert(const value_type& aValue) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
	{
		size_type index = myValues.size();
		myAccelerationMap.emplace(aValue.first, index);
		myValues.push_back(aValue);
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::insert(value_type&& aValue) noexcept(std::is_nothrow_move_constructible_v<value_type>&& std::is_nothrow_copy_constructible_v<key_type>)
	{
		size_type index = myValues.size();
		myAccelerationMap.emplace(aValue.first, index);
		myValues.push_back(std::move(aValue));
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::insert(iterator aWhere, const mapped_type& aValue) noexcept(std::is_nothrow_copy_assignable_v<mapped_type>)
	{
		myValues.at(myAccelerationMap.at(aWhere->first)).second = aValue;
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::insert(iterator aWhere, mapped_type&& aValue) noexcept(std::is_nothrow_move_assignable_v<mapped_type>)
	{
		myValues.at(myAccelerationMap.at(aWhere->first)).second = std::move(aValue);
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::insert(const_iterator aWhere, const mapped_type& aValue) noexcept(std::is_nothrow_copy_assignable_v<mapped_type>)
	{
		myValues.at(myAccelerationMap.at(aWhere->first)).second = aValue;
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::insert(const_iterator aWhere, mapped_type&& aValue) noexcept(std::is_nothrow_move_assignable_v<mapped_type>)
	{
		myValues.at(myAccelerationMap.at(aWhere->first)).second = std::move(aValue);
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::emplace(const key_type& aKey, const mapped_type& aValue) noexcept(std::is_nothrow_copy_constructible_v<key_type>&& std::is_nothrow_copy_constructible_v<mapped_type>)
	{
		size_type index = myValues.size();
		myAccelerationMap.emplace(aKey, index);
		myValues.push_back(value_type(aKey, aValue));
		return iterator(&myValues[index]);
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::emplace(const key_type& aKey, mapped_type&& aValue) noexcept(std::is_nothrow_copy_constructible_v<key_type>&& std::is_nothrow_move_constructible_v<mapped_type>)
	{
		size_type index = myValues.size();
		myAccelerationMap.emplace(aKey, index);
		myValues.push_back(value_type(aKey, std::move(aValue)));
		return iterator(&myValues[index]);
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::emplace(key_type&& aKey, const mapped_type& aValue) noexcept(std::is_nothrow_move_constructible_v<key_type>&& std::is_nothrow_copy_constructible_v<mapped_type>)
	{
		size_type index = myValues.size();
		myAccelerationMap.emplace(aKey, index);
		myValues.push_back(value_type(std::move(aKey), aValue));
		return iterator(&myValues[index]);
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::emplace(key_type&& aKey, mapped_type&& aValue) noexcept(std::is_nothrow_move_constructible_v<key_type>&& std::is_nothrow_move_constructible_v<mapped_type>)
	{
		size_type index = myValues.size();
		myAccelerationMap.emplace(aKey, index);
		myValues.push_back(value_type(std::move(aKey), std::move(aValue)));
		return iterator(&myValues[index]);
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::erase(const key_type& aKey)
	{
		size_type index = myAccelerationMap.at(aKey);
		myAccelerationMap.erase(aKey);
		if (index + 1 < myValues.size())
		{
			myValues.at(index) = std::move(myValues.back());
			myAccelerationMap.at(myValues.at(index).first) = index;
			myValues.pop_back();
			return iterator(&myValues[index]);
		}
		myValues.pop_back();
		return end();
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::erase(iterator aWhere)
	{
		size_type index = myAccelerationMap.at(aWhere->first);
		myAccelerationMap.erase(aWhere->first);
		if (index + 1 < myValues.size())
		{
			myValues.at(index) = std::move(myValues.back());
			myAccelerationMap.at(myValues.at(index).first) = index;
			myValues.pop_back();
			return iterator(&myValues[index]);
		}
		myValues.pop_back();
		return end();
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::erase(const_iterator aWhere)
	{
		size_type index = myAccelerationMap.at(aWhere->first);
		myAccelerationMap.erase(aWhere->first);
		if (index + 1 < myValues.size())
		{
			myValues.at(index) = std::move(myValues.back());
			myAccelerationMap.at(myValues.at(index).first) = index;
			myValues.pop_back();
			return iterator(&myValues[index]);
		}
		myValues.pop_back();
		return end();
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::mapped_type& CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::operator[](const key_type& aKey)
	{
		return at(aKey);
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline const typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::mapped_type& CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::operator[](const key_type& aKey) const
	{
		return at(aKey);
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::mapped_type& CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::at(const key_type& aKey)
	{
		return myValues.at(myAccelerationMap.at(aKey)).second;
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline const typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::mapped_type& CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::at(const key_type& aKey) const
	{
		return myValues.at(myAccelerationMap.at(aKey)).second;
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::find(const key_type& aKey)
	{
		auto it = myAccelerationMap.find(aKey);
		if (it == myAccelerationMap.end())
		{
			return end();
		}
		else
		{
			return iterator(&myValues.at(it->second));
		}
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::const_iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::find(const key_type& aKey) const
	{
		auto it = myAccelerationMap.find(aKey);
		if (it == myAccelerationMap.end())
		{
			return end();
		}
		else
		{
			return iterator(&myValues.at(it->second));
		}
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline bool CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::contains(const key_type& aKey) const noexcept
	{
		return myAccelerationMap.contains(aKey);
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	void CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::clear() noexcept(std::is_nothrow_destructible_v<value_type>)
	{
		myAccelerationMap.clear();
		myValues.clear();
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::begin() noexcept
	{
		return iterator(myValues.data());
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::end() noexcept
	{
		return iterator(myValues.data() + myValues.size());
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::const_iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::begin() const noexcept
	{
		return const_iterator(myValues.data());
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::const_iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::end() const noexcept
	{
		return const_iterator(myValues.data() + myValues.size());
	}

	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::reverse_iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::rbegin() noexcept
	{
		return reverse_iterator(end());
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::reverse_iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::rend() noexcept
	{
		return reverse_iterator(begin());
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::const_reverse_iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}
	template<class _Kty, class _Ty, class _Hasher, class _Keyeq>
	inline typename CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::const_reverse_iterator CommonUtilities::AcceleratedVector<_Kty, _Ty, _Hasher, _Keyeq>::rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}
}