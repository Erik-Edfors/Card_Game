#pragma once
#include "Reference.hpp"
#include "StringHash.hpp"
#include <any>
#include <set>
#include <unordered_map>
#include <queue>
#include <mutex>

namespace CommonUtilities
{
	template<typename _IDTy, typename _DataTy>
	struct TEvent;

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	class TEventListener;

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	class TEventPusher;

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	class TEventManager;

	template<typename _IDTy, typename _DataTy>
	struct TEvent
	{
		_IDTy ID;
		_DataTy Data;
	};

	template<typename _IDTy, typename _DataTy, class _IDHasher = std::hash<_IDTy>>
	class TEventPusher : public Referable
	{
		friend class TEventListener<_IDTy, _DataTy, _IDHasher>;
	public:
		using ID = _IDTy;
		using Data = _DataTy;
		using Hasher = _IDHasher;

		using EventListener = TEventListener<_IDTy, _DataTy, _IDHasher>;
		using Event = TEvent<_IDTy, _DataTy>;

	protected:
		virtual void AddListener(EventListener* aListener, ID aID) = 0;
		virtual void RemoveListener(EventListener* aListener, ID aID) = 0;
	};

	template<typename _IDTy, typename _DataTy, class _IDHasher = std::hash<_IDTy>>
	class TEventManager : public TEventPusher<_IDTy, _DataTy, _IDHasher>
	{
		friend class TEventListener<_IDTy, _DataTy, _IDHasher>;
	public:
		using EventPusher = TEventPusher<_IDTy, _DataTy, _IDHasher>;

		using ID = typename EventPusher::ID;
		using Data = typename EventPusher::Data;
		using Hasher = typename EventPusher::Hasher;

		using EventListener = typename EventPusher::EventListener;
		using Event = typename EventPusher::Event;

	public:
		TEventManager();
		virtual ~TEventManager();

		TEventManager(TEventManager&& aEventManager) noexcept;
		TEventManager(const TEventManager&) = delete;

		void DispatchEvents();

		void Push(ID aID);
		void Push(ID aID, const Data& aData);

		TEventManager& operator=(TEventManager&& aEventManager) noexcept;

	protected:
		virtual void AddListener(EventListener* aListener, ID aID) override;
		virtual void RemoveListener(EventListener* aListener, ID aID) override;

	private:
		typedef void(EventListener::* OnListenFunc)(const Event&);

		std::vector<Event> myEvents;
		std::unordered_map<ID, std::set<Reference<EventListener>>, Hasher> myListeners;
		mutable std::mutex myMX;
	};

	template<typename _IDTy, typename _DataTy, class _IDHasher = std::hash<_IDTy>>
	class TGlobalEventManager : public TEventManager<_IDTy, _DataTy, _IDHasher>
	{
	public:
		~TGlobalEventManager();
		static TGlobalEventManager* Get() noexcept;

	private:
		TGlobalEventManager();

		static TGlobalEventManager* ourInstance;
	};

	template<typename _IDTy, typename _DataTy, class _IDHasher = std::hash<_IDTy>>
	class TEventListener : public Referable
	{
	public:
		typedef _IDTy ID;
		typedef _DataTy Data;
		typedef _IDHasher Hasher;

		typedef TEventPusher<_IDTy, _DataTy, _IDHasher> EventPusher;
		typedef TGlobalEventManager<_IDTy, _DataTy, _IDHasher> GlobalEventManager;
		typedef TEvent<_IDTy, _DataTy> Event;

	private:
		friend class EventPusher;

	public:
		TEventListener() noexcept;
		~TEventListener() noexcept;
		TEventListener(TEventListener&& aEventListener) noexcept;
		TEventListener(const TEventListener& aEventListener) noexcept;

		void SetPusher(const Reference<EventPusher>& aEventPusher) noexcept;

		bool BeginListen(ID aEventID) noexcept;
		bool StopListen(ID aEventID) noexcept;
		bool StopListen() noexcept;

		bool GetEvent(Event* outEvent) noexcept;

		TEventListener& operator=(TEventListener&& aEventListener) noexcept;
		TEventListener& operator=(const TEventListener& aEventListener) noexcept;

	private:
		void OnListen(const Event& aEvent);

		Reference<EventPusher> myEventPusher;
		std::set<ID> myListeningEventIDs;
		std::queue<Event> myEvents;
		std::mutex myMutex;
	};

	typedef TEvent<StringHash, std::any> Event;
	typedef TEventListener<StringHash, std::any> EventListener;
	typedef TEventPusher<StringHash, std::any> EventPusher;
	typedef TEventManager<StringHash, std::any> EventManager;
	typedef TGlobalEventManager<StringHash, std::any> GlobalEventManager;

#pragma region EventManager

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventManager<_IDTy, _DataTy, _IDHasher>::TEventManager()
	{
	}
	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventManager<_IDTy, _DataTy, _IDHasher>::~TEventManager() = default;

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventManager<_IDTy, _DataTy, _IDHasher>::TEventManager(TEventManager&& aEventManager) noexcept
	{
		*this = std::move(aEventManager);
	}
	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline void TEventManager<_IDTy, _DataTy, _IDHasher>::DispatchEvents()
	{
		if (myEvents.empty())
		{
			return;
		}

		{
			std::scoped_lock<std::mutex> lk(myMX);

			for (const Event& event : myEvents)
			{
				auto it = myListeners.find(event.ID);
				if (it != myListeners.end())
				{
					for (const Reference<EventListener>& listener : it->second)
					{
						listener->OnListen(event);
					}
				}
			}
			myEvents.clear();
		}
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline void TEventManager<_IDTy, _DataTy, _IDHasher>::AddListener(EventListener* aListener, ID aID)
	{
		std::scoped_lock<std::mutex> lk(myMX);
		myListeners[aID].insert(aListener);
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline void TEventManager<_IDTy, _DataTy, _IDHasher>::RemoveListener(EventListener* aListener, ID aID)
	{
		std::scoped_lock<std::mutex> lk(myMX);
		myListeners[aID].erase(aListener);
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline void TEventManager<_IDTy, _DataTy, _IDHasher>::Push(ID aID)
	{
		std::scoped_lock<std::mutex> lk(myMX);
		myEvents.push_back(Event{ aID });
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline void TEventManager<_IDTy, _DataTy, _IDHasher>::Push(ID aID, const Data& aData)
	{
		std::scoped_lock<std::mutex> lk(myMX);
		myEvents.push_back(Event{ aID, aData });
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventManager<_IDTy, _DataTy, _IDHasher>& TEventManager<_IDTy, _DataTy, _IDHasher>::operator=(TEventManager&& aEventManager) noexcept
	{
		std::scoped_lock<std::mutex> localLock(myMX);
		std::scoped_lock<std::mutex> otherLock(aEventManager.myMX);

		Referable::operator=(std::move(aEventManager));

		myEvents = std::move(aEventManager.myEvents);
		aEventManager.myEvents = {};

		myListeners = std::move(aEventManager.myListeners);
		aEventManager.myListeners = {};

		return *this;
	}

#pragma endregion

#pragma region GlobalEventManager

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TGlobalEventManager<_IDTy, _DataTy, _IDHasher>* TGlobalEventManager<_IDTy, _DataTy, _IDHasher>::ourInstance = nullptr;

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TGlobalEventManager<_IDTy, _DataTy, _IDHasher>::TGlobalEventManager()
	{
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TGlobalEventManager<_IDTy, _DataTy, _IDHasher>::~TGlobalEventManager()
	{
		if (ourInstance == this)
		{
			ourInstance = nullptr;
		}
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TGlobalEventManager<_IDTy, _DataTy, _IDHasher>* TGlobalEventManager<_IDTy, _DataTy, _IDHasher>::Get() noexcept
	{
		if (ourInstance == nullptr)
		{
			ourInstance = new TGlobalEventManager();
		}
		return ourInstance;
	}

#pragma endregion

#pragma region EventListener

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventListener<_IDTy, _DataTy, _IDHasher>::TEventListener() noexcept
	{
	}
	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventListener<_IDTy, _DataTy, _IDHasher>::~TEventListener() noexcept
	{
		StopListen();
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventListener<_IDTy, _DataTy, _IDHasher>::TEventListener(TEventListener&& aEventListener) noexcept
	{
		*this = std::move(aEventListener);
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventListener<_IDTy, _DataTy, _IDHasher>::TEventListener(const TEventListener& aEventListener)noexcept
	{
		*this = aEventListener;
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline void TEventListener<_IDTy, _DataTy, _IDHasher>::SetPusher(const Reference<EventPusher>& aEventPusher) noexcept
	{
		std::scoped_lock lk(myMutex);

		while (not myEvents.empty())
		{
			myEvents.pop();
		}
		if (not myEventPusher.Expired())
		{
			for (const ID& id : myListeningEventIDs)
			{
				myEventPusher->RemoveListener(this, id);
			}
		}
		myEventPusher = aEventPusher;
		if (not myEventPusher.Expired())
		{
			for (const ID& id : myListeningEventIDs)
			{
				myEventPusher->AddListener(this, id);
			}
		}
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline bool TEventListener<_IDTy, _DataTy, _IDHasher>::BeginListen(ID aEventID) noexcept
	{
		std::scoped_lock lk(myMutex);

		myListeningEventIDs.insert(aEventID);
		if (not myEventPusher.Expired())
		{
			myEventPusher->AddListener(this, aEventID);
		}

		return true;
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline bool TEventListener<_IDTy, _DataTy, _IDHasher>::StopListen(ID aEventID) noexcept
	{
		std::scoped_lock lk(myMutex);

		myListeningEventIDs.erase(aEventID);
		if (not myEventPusher.Expired())
		{
			myEventPusher->RemoveListener(this, aEventID);
		}
		return true;
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline bool TEventListener<_IDTy, _DataTy, _IDHasher>::StopListen() noexcept
	{
		std::scoped_lock lk(myMutex);

		if (not myEventPusher.Expired())
		{
			for (const ID& id : myListeningEventIDs)
			{
				myEventPusher->RemoveListener(this, id);
			}
		}
		myListeningEventIDs.clear();
		return true;
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline bool TEventListener<_IDTy, _DataTy, _IDHasher>::GetEvent(Event* outEvent) noexcept
	{
		if (not myEvents.empty())
		{
			std::scoped_lock lk(myMutex);
			if (outEvent == nullptr)
			{
				return true;
			}
			*outEvent = myEvents.front();
			myEvents.pop();
			return true;
		}
		return false;
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventListener<_IDTy, _DataTy, _IDHasher>& TEventListener<_IDTy, _DataTy, _IDHasher>::operator=(TEventListener&& aEventListener) noexcept
	{
		Referable::operator=(std::move(aEventListener));
		
		StopListen();

		myEventPusher = std::move(aEventListener.myEventPusher);

		myListeningEventIDs = std::move(aEventListener.myListeningEventIDs);
		aEventListener.myListeningEventIDs = {};

		myEvents = std::move(aEventListener.myEvents);
		aEventListener.myEvents = {};

		return *this;
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline TEventListener<_IDTy, _DataTy, _IDHasher>& TEventListener<_IDTy, _DataTy, _IDHasher>::operator=(const TEventListener& aEventListener) noexcept
	{
		Referable::operator=(aEventListener);

		StopListen();

		SetPusher(aEventListener.myEventPusher);
		for (ID id : aEventListener.myListeningEventIDs)
		{
			BeginListen(id);
		}
		myEvents = aEventListener.myEvents;

		return *this;
	}

	template<typename _IDTy, typename _DataTy, class _IDHasher>
	inline void TEventListener<_IDTy, _DataTy, _IDHasher>::OnListen(const Event& aEvent)
	{
		myEvents.push(aEvent);
	}

#pragma endregion
}