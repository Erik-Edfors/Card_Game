#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>

namespace CommonUtilities
{
	template<typename _ResultTy>
	class Task;

	template<typename _ResultTy>
	class Contract;

	template<typename _ResultTy>
	struct ContractDetails
	{
		std::mutex Mutex;
		std::condition_variable CV;
		_ResultTy Result;
		bool Finished;
	};

	template<>
	struct ContractDetails<void>
	{
		std::mutex Mutex;
		std::condition_variable CV;
		bool Finished;
	};

	template<typename _ResultTy>
	class Task
	{
	public:
		typedef Contract<_ResultTy> Contract;
		typedef _ResultTy Result;

		Task();
		Task(const _ResultTy& aDefaultResult);

		bool IsFinished() const;

		bool WaitFor(unsigned int aMilliSeconds, Result* outResult) const;
		Result&& Wait() const;

		Contract CreateContract();

	private:
		std::shared_ptr<ContractDetails<Result>> myShared;
	};
	template<>
	class Task<void>
	{
	public:
		typedef Contract<void> Contract;
		typedef void Result;

		Task();

		bool IsFinished() const;

		bool WaitFor(unsigned int aMilliSeconds) const;
		Result Wait() const;

		Contract CreateContract();

	private:
		std::shared_ptr<ContractDetails<Result>> myShared;
	};

	template<typename _ResultTy>
	class Contract
	{
		friend class Task<_ResultTy>;
	public:
		typedef Task<_ResultTy> Task;
		typedef _ResultTy Result;

		bool IsFinished() const;

		bool Finish(const Result& aResult);
		bool Finish(Result&& aResult);
	private:
		Contract(const std::shared_ptr<ContractDetails<Result>>& aSharedPtr);

		std::shared_ptr<ContractDetails<Result>> myShared;
	};
	template<>
	class Contract<void>
	{
		friend class Task<void>;
	public:
		typedef Task<void> Task;
		typedef void Result;

		bool IsFinished() const;

		bool Finish();
	private:
		Contract(const std::shared_ptr<ContractDetails<void>>& aSharedPtr);

		std::shared_ptr<ContractDetails<void>> myShared;
	};

	class ThreadPool
	{
	public:
		ThreadPool();
		ThreadPool(size_t aNumThreads);
		ThreadPool(ThreadPool&& aThreadPool) noexcept;
		~ThreadPool();

		template<typename _RetTy, typename... _Args>
		Task<_RetTy> Push(std::function<_RetTy(_Args...)> aFunction);

		void Resize(size_t aNumThreads);
		size_t Size() const;

		ThreadPool& operator=(ThreadPool&& aThreadPool) noexcept;

	private:
		typedef std::function<void()> WorkFunc;

		struct ThreadResources
		{
			ThreadPool* ThreadPool;
			std::condition_variable CV;
			std::queue<WorkFunc> Work;
			bool Terminating;
			std::mutex Mutex;
		};

		template<typename _RetTy, std::enable_if_t<std::is_same_v<_RetTy, void>, bool> = true>
		static void WorkWork(std::function<_RetTy()> aFunc, Contract<_RetTy> aContract);

		template<typename _RetTy, std::enable_if_t<std::is_same_v<_RetTy, void> == false, bool> = true>
		static void WorkWork(std::function<_RetTy()> aFunc, Contract<_RetTy> aContract);

		static void ThreadProc(ThreadResources* aThreadResources);

		void StartThreads(size_t aNumThreads);
		void TerminateThreads();

		std::unique_ptr<ThreadResources> myThreadResources;
		std::vector<std::thread> myThreads;
	};

	template<typename _ResultTy>
	inline CommonUtilities::Task<_ResultTy>::Task()
	{
		myShared = std::make_shared<ContractDetails<Result>>();
		myShared->Finished = true;
	}
	inline CommonUtilities::Task<void>::Task()
	{
		myShared = std::make_shared<ContractDetails<void>>();
		myShared->Finished = true;
	}
	template<typename _ResultTy>
	inline Task<_ResultTy>::Task(const _ResultTy& aDefaultResult) :
		Task()
	{
		myShared->Result = aDefaultResult;
	}
	template<typename _ResultTy>
	inline bool Task<_ResultTy>::IsFinished() const
	{
		std::scoped_lock<std::mutex> lk(myShared->Mutex);
		return myShared->Finished;
	}
	template<typename _ResultTy>
	inline bool Task<_ResultTy>::WaitFor(unsigned int aMilliSeconds, Result* outResult) const
	{
		std::unique_lock<std::mutex> lk(myShared->Mutex);
		if (myShared->Finished)
		{
			*outResult = std::move(myShared->Result);
			return true;
		}
		myShared->CV.wait_for(lk, std::chrono::milliseconds(aMilliSeconds));
		if (myShared->Finished)
		{
			*outResult = std::move(myShared->Result);
			return true;
		}
		return false;
	}
	inline bool Task<void>::WaitFor(unsigned int aMilliSeconds) const
	{
		std::unique_lock<std::mutex> lk(myShared->Mutex);
		if (myShared->Finished)
		{
			return true;
		}
		myShared->CV.wait_for(lk, std::chrono::milliseconds(aMilliSeconds));
		if (myShared->Finished)
		{
			return true;
		}
		return false;
	}
	template<typename _ResultTy>
	inline typename Task<_ResultTy>::Result&& Task<_ResultTy>::Wait() const
	{
		std::unique_lock<std::mutex> lk(myShared->Mutex);
		while (not myShared->Finished)
		{
			myShared->CV.wait(lk);
		}
		return std::move(myShared->Result);
	}
	inline typename Task<void>::Result Task<void>::Wait() const
	{
		std::unique_lock<std::mutex> lk(myShared->Mutex);
		while (not myShared->Finished)
		{
			myShared->CV.wait(lk);
		}
	}
	template<typename _ResultTy>
	inline typename Task<_ResultTy>::Contract Task<_ResultTy>::CreateContract()
	{
		myShared->Finished = false;
		return Contract(myShared);
	}
	inline typename Task<void>::Contract Task<void>::CreateContract()
	{
		myShared->Finished = false;
		return Contract(myShared);
	}

	template<typename _ResultTy>
	inline Contract<_ResultTy>::Contract(const std::shared_ptr<typename ContractDetails<Result>>& aSharedPtr)
	{
		myShared = aSharedPtr;
	}
	inline Contract<void>::Contract(const std::shared_ptr<ContractDetails<void>>& aSharedPtr)
	{
		myShared = aSharedPtr;
	}
	template<typename _ResultTy>
	inline bool Contract<_ResultTy>::IsFinished() const
	{
		std::scoped_lock<std::mutex> lk(myShared->Mutex);
		return myShared->Finished = true;
	}
	template<typename _ResultTy>
	inline bool Contract<_ResultTy>::Finish(const Result& aResult)
	{
		{
			std::scoped_lock<std::mutex> lk(myShared->Mutex);
			if (myShared->Finished)
			{
				return false;
			}
			myShared->Result = aResult;
			myShared->Finished = true;
		}
		myShared->CV.notify_all();
		return true;
	}
	template<typename _ResultTy>
	inline bool Contract<_ResultTy>::Finish(Result&& aResult)
	{
		{
			std::scoped_lock<std::mutex> lk(myShared->Mutex);
			if (myShared->Finished)
			{
				return false;
			}
			myShared->Result = std::move(aResult);
			myShared->Finished = true;
		}
		myShared->CV.notify_all();
		return true;
	}
	inline bool Contract<void>::Finish()
	{
		{
			std::scoped_lock<std::mutex> lk(myShared->Mutex);
			if (myShared->Finished)
			{
				return false;
			}
			myShared->Finished = true;
		}
		myShared->CV.notify_all();
		return true;
	}

	inline ThreadPool::ThreadPool()
	{
		myThreadResources = std::make_unique<ThreadResources>();
		myThreadResources->ThreadPool = this;
		myThreadResources->Terminating = false;
	}

	inline ThreadPool::ThreadPool(size_t aNumThreads)
	{
		myThreadResources = std::make_unique<ThreadResources>();
		myThreadResources->ThreadPool = this;
		myThreadResources->Terminating = false;

		StartThreads(aNumThreads);
	}
	inline ThreadPool::ThreadPool(ThreadPool&& aThreadPool) noexcept
	{
		*this = std::move(aThreadPool);
	}
	inline ThreadPool::~ThreadPool()
	{
		TerminateThreads();
	}
	template<typename _RetTy, typename ..._Args>
	inline Task<_RetTy> ThreadPool::Push(std::function<_RetTy(_Args...)> aFunction)
	{
		Task<_RetTy> task;
		{
			std::scoped_lock<std::mutex> lk(myThreadResources->Mutex);

			myThreadResources->Work.push(std::bind(&ThreadPool::WorkWork<_RetTy>, aFunction, task.CreateContract()));
		}
		myThreadResources->CV.notify_one();
		return task;
	}
	template<typename _RetTy, std::enable_if_t<std::is_same_v<_RetTy, void>, bool>>
	inline void ThreadPool::WorkWork(std::function<_RetTy()> aFunc, Contract<_RetTy> aContract)
	{
		aFunc();
		aContract.Finish();
	}
	template<typename _RetTy, std::enable_if_t<std::is_same_v<_RetTy, void> == false, bool>>
	inline void ThreadPool::WorkWork(std::function<_RetTy()> aFunc, Contract<_RetTy> aContract)
	{
		aContract.Finish(aFunc());
	}
	inline void ThreadPool::Resize(size_t aNumThreads)
	{
		TerminateThreads();
		StartThreads(aNumThreads);
	}
	inline size_t ThreadPool::Size() const
	{
		return myThreads.size();
	}
	inline ThreadPool& ThreadPool::operator=(ThreadPool&& aThreadPool) noexcept
	{
		size_t numThreads = aThreadPool.Size();
		TerminateThreads();
		aThreadPool.TerminateThreads();

		myThreadResources = std::move(aThreadPool.myThreadResources);
		StartThreads(numThreads);
	}
	inline void ThreadPool::ThreadProc(ThreadResources* aThreadResources)
	{
		while (true)
		{
			WorkFunc work = nullptr;
			{
				std::unique_lock<std::mutex> lk(aThreadResources->Mutex);
				if (aThreadResources->Work.empty())
				{
					aThreadResources->CV.wait(lk, [=]() { return aThreadResources->Terminating || not aThreadResources->Work.empty(); });
					if (aThreadResources->Terminating)
					{
						break;
					}
				}

				if (not aThreadResources->Work.empty())
				{
					work = std::move(aThreadResources->Work.front());
					aThreadResources->Work.pop();
				}
			}
			if (work)
			{
				work();
			}
		}
		return;
	}
	inline void ThreadPool::StartThreads(size_t aNumThreads)
	{
		TerminateThreads();
		{
			std::scoped_lock<std::mutex> lk(myThreadResources->Mutex);
			myThreadResources->Terminating = false;

			for (size_t i = 0; i < aNumThreads; i++)
			{
				myThreads.emplace_back(&ThreadPool::ThreadProc, myThreadResources.get());
			}
		}
	}
	inline void ThreadPool::TerminateThreads()
	{
		{
			std::scoped_lock<std::mutex> lk(myThreadResources->Mutex);
			myThreadResources->Terminating = true;
		}
		myThreadResources->CV.notify_all();
		for (std::thread& thread : myThreads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
		myThreads.clear();
	}
}