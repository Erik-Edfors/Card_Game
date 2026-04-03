#pragma once
#include <vector>

namespace CommonUtilities
{
	template<typename _Weight, typename _Value>
	class InterpolationVector
	{
	public:
		struct Between {
			_Value First;
			_Value Second;
			_Weight Factor;
		};

		InterpolationVector();
		~InterpolationVector();

		Between operator[](const _Weight& aWeight);

		void emplace(const _Weight& aWeight, const _Value& aValue);

		_Weight Low() const;
		_Weight High() const;

		size_t Size() const;
		bool Empty() const;

	private:
		struct Entry
		{
			_Weight Weight;
			_Value Value;

			bool operator<(Entry& other) const
			{
				return Weight < other.Weight;
			}
		};

		size_t myEntryCount;
		size_t myEntryCapacity;
		void* myEntries;
	};

	template<typename _Weight, typename _Value>
	inline InterpolationVector<_Weight, _Value>::InterpolationVector() :
		myEntryCount(0),
		myEntryCapacity(1),
		myEntries(operator new(sizeof(Entry)))
	{}

	template<typename _Weight, typename _Value>
	inline InterpolationVector<_Weight, _Value>::~InterpolationVector()
	{
		for (size_t i = 0; i < myEntryCount; i++)
		{
			reinterpret_cast<Entry*>(myEntries)[i].~Entry();
		}
		delete myEntries;
	}

	template<typename _Weight, typename _Value>
	inline InterpolationVector<_Weight, _Value>::Between InterpolationVector<_Weight, _Value>::operator[](const _Weight& aWeight)
	{
		if (aWeight < Low())
		{
			return Between{ _Value(), reinterpret_cast<Entry*>(myEntries)->Value, 1.f };
			//return reinterpret_cast<Entry*>(myEntries)->Value;
		}

		if (High() < aWeight)
		{
			return Between{ (reinterpret_cast<Entry*>(myEntries) + myEntryCount - 1)->Value, _Value(), 0.f };
			//return (reinterpret_cast<Entry*>(myEntries) + myEntryCount - 1)->Value;
		}

		Entry* first = reinterpret_cast<Entry*>(myEntries);
		Entry* last = reinterpret_cast<Entry*>(myEntries) + myEntryCount;
		Entry* middle = first + std::distance(first, last) / 2;

		while (first != middle)
		{
			if (middle->Weight < aWeight)
			{
				first = middle;
			}
			else
			{
				last = middle;
			}
			middle = first + std::distance(first, last) / 2;
		}

		return Between{ first->Value, last->Value, (aWeight - first->Weight) / (last->Weight - first->Weight) };

		//return middle->Value;

		//_Weight w1 = first->Weight;
		//_Weight w2 = last->Weight;

		//_Value v1 = first->Value;
		//_Value v2 = last->Value;

		//_Weight d1 = aWeight - w1;
		//_Weight d2 = w2 - aWeight;
		//_Weight d3 = w2 - w1;

		//return v1 * (d2 / d3) + v2 * (d1 / d3);
	}

	template<typename _Weight, typename _Value>
	inline void InterpolationVector<_Weight, _Value>::emplace(const _Weight& aWeight, const _Value& aValue)
	{
		if (myEntryCount == myEntryCapacity)
		{
			myEntryCapacity = myEntryCapacity << 1;
			void* newEntries = operator new(myEntryCapacity * sizeof(Entry));
			memcpy(newEntries, myEntries, myEntryCount * sizeof(Entry));
			delete myEntries;
			myEntries = newEntries;
		}

		Entry* first = reinterpret_cast<Entry*>(myEntries);
		Entry* last = reinterpret_cast<Entry*>(myEntries) + myEntryCount;
		Entry* middle = first + std::distance(first, last) / 2;

		while (first != middle)
		{
			if (middle->Weight < aWeight)
			{
				first = middle;
			}
			else
			{
				last = middle;
			}
			middle = first + std::distance(first, last) / 2;
		}

		Entry* end = reinterpret_cast<Entry*>(myEntries) + myEntryCount;
		while (end > last)
		{
			*end = *(end - 1);
			--end;
		}

		::new(last) Entry{ aWeight, aValue };
		++myEntryCount;
	}

	template<typename _Weight, typename _Value>
	inline _Weight InterpolationVector<_Weight, _Value>::Low() const
	{
		if (myEntryCount == 0)
		{
			throw std::exception("InterpolationVector has no entries.");
		}

		return reinterpret_cast<Entry*>(myEntries)->Weight;
	}

	template<typename _Weight, typename _Value>
	inline _Weight InterpolationVector<_Weight, _Value>::High() const
	{
		if (myEntryCount == 0)
		{
			throw std::exception("InterpolationVector has no entries.");
		}

		return (reinterpret_cast<Entry*>(myEntries) + myEntryCount - 1)->Weight;
	}

	template<typename _Weight, typename _Value>
	inline size_t InterpolationVector<_Weight, _Value>::Size() const
	{
		return myEntryCount;
	}

	template<typename _Weight, typename _Value>
	inline bool InterpolationVector<_Weight, _Value>::Empty() const
	{
		return myEntryCount == 0;
	}
}
