#pragma once
#include <vector>

namespace CommonUtilities
{
	class Referable;
	class GenericReference;

	class GenericReference
	{
		friend class Referable;
	public:
		GenericReference();
		GenericReference(Referable* aReferable);
		virtual ~GenericReference();
		GenericReference(GenericReference&& aReference) noexcept;
		GenericReference(const GenericReference& aReference) noexcept;

		Referable* Get() const;

		bool Expired() const;

		void Reset();

		GenericReference& operator=(GenericReference&& aReference) noexcept;
		GenericReference& operator=(const GenericReference& aReference) noexcept;

		GenericReference& operator=(Referable* aReferable) noexcept;

		bool operator==(const GenericReference& aReference) const noexcept;
		bool operator!=(const GenericReference& aReference) const noexcept;
		bool operator<(const GenericReference& aReference) const noexcept;
		bool operator>(const GenericReference& aReference) const noexcept;

	private:
		void Update(Referable* aReferable);

		Referable* myReference;
	};

	template<typename _Ty>
	class Reference : private GenericReference
	{
	public:
		Reference();
		Reference(_Ty* aReferable);
		~Reference();

		Reference(Reference&& aReference) noexcept;
		Reference(const Reference& aReference) noexcept;

		_Ty* Get() const;

		bool Expired() const;

		void Reset();

		Reference& operator=(Reference&& aReference) noexcept;
		Reference& operator=(const Reference& aReference) noexcept;

		Reference& operator=(_Ty* aReferable) noexcept;

		_Ty* operator->() const noexcept;
		_Ty& operator*() const noexcept;

		bool operator==(const Reference& aReference) const noexcept;
		bool operator!=(const Reference& aReference) const noexcept;
		bool operator<(const Reference& aReference) const noexcept;
		bool operator>(const Reference& aReference) const noexcept;

	private:

	};

	class Referable
	{
		friend class GenericReference;
	public:
		Referable();
		virtual ~Referable();

		Referable(Referable&& aReferable) noexcept;
		Referable(const Referable& aReferable) noexcept;

		Referable& operator=(Referable&& aReferable) noexcept;
		Referable& operator=(const Referable& aReferable) noexcept;

	protected:
		void UpdateReferences();

	private:
		void ClearReferences();

		void AddReference(GenericReference* aReference);
		void RemoveReference(GenericReference* aReference);

		std::vector<GenericReference*> myReferences;
	};

	inline GenericReference::GenericReference() : 
		myReference(nullptr)
	{
	}
	inline GenericReference::GenericReference(Referable* aReferable)
	{
		myReference = aReferable;
		if (not Expired())
		{
			myReference->AddReference(this);
		}
	}
	inline GenericReference::~GenericReference()
	{
		if (not Expired())
		{
			myReference->RemoveReference(this);
			myReference = nullptr;
		}
	}
	inline GenericReference::GenericReference(GenericReference&& aReference) noexcept
	{
		*this = std::move(aReference);
	}
	inline GenericReference::GenericReference(const GenericReference& aReference) noexcept
	{
		*this = aReference;
	}
	inline Referable* GenericReference::Get() const
	{
		return myReference;
	}
	inline bool GenericReference::Expired() const
	{
		return myReference == nullptr;
	}
	inline void GenericReference::Reset()
	{
		if (not Expired())
		{
			myReference->RemoveReference(this);
		}
		myReference = nullptr;
	}
	inline GenericReference& GenericReference::operator=(GenericReference&& aReference) noexcept
	{
		Reset();
		myReference = aReference.myReference;
		aReference.Reset();

		if (not Expired())
		{
			myReference->AddReference(this);
		}
		return *this;
	}
	inline GenericReference& GenericReference::operator=(const GenericReference& aReference) noexcept
	{
		Reset();
		myReference = aReference.myReference;

		if (not Expired())
		{
			myReference->AddReference(this);
		}
		return *this;
	}
	inline GenericReference& GenericReference::operator=(Referable* aReferable) noexcept
	{
		Reset();
		myReference = aReferable;
		if (not Expired())
		{
			myReference->AddReference(this);
		}
		return *this;
	}
	inline bool GenericReference::operator==(const GenericReference& aReference) const noexcept
	{
		return Get() == aReference.Get();
	}
	inline bool GenericReference::operator!=(const GenericReference& aReference) const noexcept
	{
		return Get() != aReference.Get();
	}
	inline bool GenericReference::operator<(const GenericReference& aReference) const noexcept
	{
		return Get() < aReference.Get();
	}
	inline bool GenericReference::operator>(const GenericReference& aReference) const noexcept
	{
		return Get() > aReference.Get();
	}
	inline void GenericReference::Update(Referable* aReferable)
	{
		myReference = aReferable;
	}



	template<typename _Ty>
	inline Reference<_Ty>::Reference() :
		GenericReference()
	{
	}
	template<typename _Ty>
	inline Reference<_Ty>::Reference(_Ty* aReferable) :
		GenericReference(aReferable)
	{
	}
	template<typename _Ty>
	inline Reference<_Ty>::~Reference()
	{
	}
	template<typename _Ty>
	inline Reference<_Ty>::Reference(Reference&& aReference) noexcept
	{
		*this = std::move(aReference);
	}
	template<typename _Ty>
	inline Reference<_Ty>::Reference(const Reference& aReference) noexcept
	{
		*this = aReference;
	}
	template<typename _Ty>
	inline _Ty* Reference<_Ty>::Get() const
	{
		return static_cast<_Ty*>(GenericReference::Get());
	}
	template<typename _Ty>
	inline bool Reference<_Ty>::Expired() const
	{
		return GenericReference::Expired();
	}
	template<typename _Ty>
	inline void Reference<_Ty>::Reset()
	{
		if (not Expired())
		{
			myReference->RemoveReference(this);
		}
		myReference = nullptr;
	}
	template<typename _Ty>
	inline Reference<_Ty>& Reference<_Ty>::operator=(Reference&& aReference) noexcept
	{
		GenericReference::operator=(std::move(aReference));
		return *this;
	}
	template<typename _Ty>
	inline Reference<_Ty>& Reference<_Ty>::operator=(const Reference& aReference) noexcept
	{
		GenericReference::operator=(aReference);
		return *this;
	}
	template<typename _Ty>
	inline Reference<_Ty>& Reference<_Ty>::operator=(_Ty* aReferable) noexcept
	{
		GenericReference::operator=(aReferable);
		return *this;
	}
	template<typename _Ty>
	inline _Ty* Reference<_Ty>::operator->() const noexcept
	{
		return Get();
	}
	template<typename _Ty>
	inline _Ty& Reference<_Ty>::operator*() const noexcept
	{
		return *Get();
	}

	template<typename _Ty>
	inline bool Reference<_Ty>::operator==(const Reference& aReference) const noexcept
	{
		return GenericReference::operator==(aReference);
	}

	template<typename _Ty>
	inline bool Reference<_Ty>::operator!=(const Reference& aReference) const noexcept
	{
		return GenericReference::operator!=(aReference);
	}

	template<typename _Ty>
	inline bool Reference<_Ty>::operator<(const Reference& aReference) const noexcept
	{
		return GenericReference::operator<(aReference);
	}

	template<typename _Ty>
	inline bool Reference<_Ty>::operator>(const Reference& aReference) const noexcept
	{
		return GenericReference::operator>(aReference);
	}



	inline Referable::Referable()
	{
	}
	inline Referable::~Referable()
	{
		ClearReferences();
	}
	inline Referable::Referable(Referable&& aReferable) noexcept
	{
		*this = std::move(aReferable);
	}
	inline Referable::Referable(const Referable& aReferable) noexcept
	{
		*this = aReferable;
	}
	inline Referable& Referable::operator=(Referable&& aReferable) noexcept
	{
		for (GenericReference* reference : myReferences)
		{
			reference->Reset();
		}
		myReferences = std::move(aReferable.myReferences);
		aReferable.myReferences.clear();
		UpdateReferences();
		return *this;
	}
	inline Referable& Referable::operator=(const Referable& aReferable) noexcept
	{
		aReferable;
		ClearReferences();
		return *this;
	}
	inline void Referable::UpdateReferences()
	{
		for (GenericReference* reference : myReferences)
		{
			reference->Update(this);
		}
	}
	inline void Referable::ClearReferences()
	{
		std::vector<GenericReference*> staged = myReferences;
		for (GenericReference* reference : staged)
		{
			reference->Reset();
		}
		myReferences.clear();
	}
	inline void Referable::AddReference(GenericReference* aReference)
	{
		myReferences.push_back(aReference);
	}
	inline void Referable::RemoveReference(GenericReference* aReference)
	{
		myReferences.erase(std::find(myReferences.begin(), myReferences.end(), aReference));
	}
}