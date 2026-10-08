#pragma once
#include "PImpl.hpp"
#include <mutex>
#include <condition_variable>

namespace CommonUtilities
{
	enum : unsigned char
	{
		CONTRACT_STATUS_WAITING,
		CONTRACT_STATUS_FINISHED,
		CONTRACT_STATUS_CANCELLED,
	};
	using CONTRACT_STATUS = unsigned char;

	template<class _Ty>
	struct ContractDetails
	{
		mutable std::mutex Mutex;
		mutable std::condition_variable CV;
		_Ty Value;
		CONTRACT_STATUS Status;
	};

	template<>
	struct ContractDetails<void>
	{
		mutable std::mutex Mutex;
		mutable std::condition_variable CV;
		CONTRACT_STATUS Status;
	};

	template<class _Ty>
	class Contract;

	template<class _Ty>
	class Task;

	template<class _Ty>
	class Contract
	{
		friend class Task<_Ty>;
	public:
		using Task = Task<_Ty>;
		using ContractDetails = ContractDetails<_Ty>;

		Contract() noexcept;
		~Contract() noexcept;

		bool Wait(_Ty* outResult) const noexcept;

		CU_NO_DISCARD CONTRACT_STATUS GetStatus() const noexcept;

		CU_NO_DISCARD Task GetTask() const noexcept;

	private:
		Contract(RefWrapper<ContractDetails>* aDetails) noexcept;

		std::unique_lock<std::mutex> Lock() const noexcept;

		PImpl<RefWrapper<ContractDetails>> myDetails;
	};

	template<class _Ty>
	class Task
	{
		friend class Contract<_Ty>;
	public:
		using Contract = Contract<_Ty>;
		using ContractDetails = ContractDetails<_Ty>;

		Task() noexcept;
		~Task() noexcept;

		bool Finish(const _Ty& aResult);
		bool Finish(_Ty&& aResult);

		CU_NO_DISCARD CONTRACT_STATUS GetStatus() const noexcept;

		bool Cancel() noexcept;

		CU_NO_DISCARD Contract GetContract() const noexcept;

	private:
		Task(RefWrapper<ContractDetails>* aDetails) noexcept;

		std::unique_lock<std::mutex> Lock() const noexcept;

		PImpl<RefWrapper<ContractDetails>> myDetails;
	};

	template<>
	class Contract<void>
	{
		friend class Task<void>;
	public:
		using Task = Task<void>;
		using ContractDetails = ContractDetails<void>;

		Contract() noexcept;
		~Contract() noexcept;

		bool Wait() const noexcept;

		CU_NO_DISCARD CONTRACT_STATUS GetStatus() const noexcept;

		CU_NO_DISCARD Task GetTask() const noexcept;

	private:
		Contract(RefWrapper<ContractDetails>* aDetails) noexcept;

		std::unique_lock<std::mutex> Lock() const noexcept;

		PImpl<RefWrapper<ContractDetails>> myDetails;
	};

	template<>
	class Task<void>
	{
		friend class Contract<void>;
	public:
		using Contract = Contract<void>;
		using ContractDetails = ContractDetails<void>;

		Task() noexcept;
		~Task() noexcept;

		bool Finish();

		CU_NO_DISCARD CONTRACT_STATUS GetStatus() const noexcept;

		bool Cancel() noexcept;

		CU_NO_DISCARD Contract GetContract() const noexcept;

	private:
		Task(RefWrapper<ContractDetails>* aDetails) noexcept;

		std::unique_lock<std::mutex> Lock() const noexcept;

		PImpl<RefWrapper<ContractDetails>> myDetails;
	};

#pragma region Contract

	template<class _Ty>
	inline Contract<_Ty>::Contract() noexcept :
		myDetails(nullptr)
	{
		CommonUtilities::CreateRefWrapper(&myDetails);
		ContractDetails& details = myDetails->Get();
		details.Status = CONTRACT_STATUS_WAITING;
	}
	template<class _Ty>
	inline Contract<_Ty>::Contract(RefWrapper<ContractDetails>* aDetails) noexcept :
		myDetails(aDetails)
	{
	}
	template<class _Ty>
	inline Contract<_Ty>::~Contract() noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		// Mutex will be deleted if reference count is 1. So lock must be released.
		if (myDetails->GetRefCount() == 1)
		{
			lk.release();
		}

		myDetails.Reset();
	}

	template<class _Ty>
	inline bool Contract<_Ty>::Wait(_Ty* outResult) const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		ContractDetails& details = myDetails->Get();
		std::condition_variable& cv = details.CV;
		while (details.Status == CONTRACT_STATUS_WAITING)
		{
			cv.wait(lk);
		}
		if (details.Status != CONTRACT_STATUS_FINISHED)
		{
			return false;
		}
		*outResult = details.Value;
		return true;
	}

	template<class _Ty>
	inline CONTRACT_STATUS Contract<_Ty>::GetStatus() const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		const ContractDetails& details = myDetails->Get();
		return details.Status;
	}

	template<class _Ty>
	inline Contract<_Ty>::Task Contract<_Ty>::GetTask() const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		return Task(myDetails.Get());
	}

	template<class _Ty>
	inline std::unique_lock<std::mutex> Contract<_Ty>::Lock() const noexcept
	{
		const ContractDetails& details = myDetails->Get();
		return std::unique_lock<std::mutex>(details.Mutex);
	}



	inline Contract<void>::Contract() noexcept :
		myDetails(nullptr)
	{
		CommonUtilities::CreateRefWrapper(&myDetails);
		ContractDetails& details = myDetails->Get();
		details.Status = CONTRACT_STATUS_WAITING;
	}
	inline Contract<void>::Contract(RefWrapper<ContractDetails>* aDetails) noexcept :
		myDetails(aDetails)
	{
	}
	inline Contract<void>::~Contract() noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		// Mutex will be deleted if reference count is 1. So lock must be released.
		if (myDetails->GetRefCount() == 1)
		{
			lk.release();
		}

		myDetails.Reset();
	}

	inline bool Contract<void>::Wait() const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		ContractDetails& details = myDetails->Get();
		std::condition_variable& cv = details.CV;
		while (details.Status == CONTRACT_STATUS_WAITING)
		{
			cv.wait(lk);
		}
		if (details.Status != CONTRACT_STATUS_FINISHED)
		{
			return false;
		}
		return true;
	}

	inline CONTRACT_STATUS Contract<void>::GetStatus() const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		const ContractDetails& details = myDetails->Get();
		return details.Status;
	}

	inline Contract<void>::Task Contract<void>::GetTask() const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		return Task(myDetails.Get());
	}

	inline std::unique_lock<std::mutex> Contract<void>::Lock() const noexcept
	{
		const ContractDetails& details = myDetails->Get();
		return std::unique_lock<std::mutex>(details.Mutex);
	}

#pragma endregion

#pragma region Task

	template<class _Ty>
	inline Task<_Ty>::Task() noexcept :
		myDetails(nullptr)
	{
		CommonUtilities::CreateRefWrapper(&myDetails);
		ContractDetails& details = myDetails->Get();
		details.Status = CONTRACT_STATUS_WAITING;
	}
	template<class _Ty>
	inline Task<_Ty>::Task(RefWrapper<ContractDetails>* aDetails) noexcept :
		myDetails(aDetails)
	{
	}
	template<class _Ty>
	inline Task<_Ty>::~Task() noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		// Mutex will be deleted if reference count is 1. So lock must be released.
		if (myDetails->GetRefCount() == 1)
		{
			lk.release();
		}

		myDetails.Reset();
	}

	template<class _Ty>
	inline bool Task<_Ty>::Finish(const _Ty& aResult)
	{
		// Position of details in memory is static, so no lock is needed to keep hold of reference.
		ContractDetails& details = myDetails->Get();
		{
			std::unique_lock<std::mutex> lk = Lock();

			if (details.Status != CONTRACT_STATUS_WAITING)
			{
				return false;
			}
			details.Value = aResult;
			details.Status = CONTRACT_STATUS_FINISHED;
		}
		// Notify is done outside the lock, so waiting threads can immediately resume.
		details.CV.notify_all();
		return true;
	}

	template<class _Ty>
	inline bool Task<_Ty>::Finish(_Ty&& aResult)
	{
		// Position of details in memory is static, so no lock is needed to keep hold of reference.
		ContractDetails& details = myDetails->Get();
		{
			std::unique_lock<std::mutex> lk = Lock();

			if (details.Status != CONTRACT_STATUS_WAITING)
			{
				return false;
			}
			details.Value = std::move(aResult);
			details.Status = CONTRACT_STATUS_FINISHED;
		}
		// Notify is done outside the lock, so waiting threads can immediately resume.
		details.CV.notify_all();
		return true;
	}

	template<class _Ty>
	inline CONTRACT_STATUS Task<_Ty>::GetStatus() const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		const ContractDetails& details = myDetails->Get();
		return details.Status;
	}

	template<class _Ty>
	inline bool Task<_Ty>::Cancel() noexcept
	{
		ContractDetails& details = myDetails->Get();
		{
			std::unique_lock<std::mutex> lk = Lock();

			if (details.Status == CONTRACT_STATUS_FINISHED)
			{
				return false;
			}

			details.Status = CONTRACT_STATUS_CANCELLED;
		}
		details.CV.notify_all();
		return true;
	}

	template<class _Ty>
	inline Task<_Ty>::Contract Task<_Ty>::GetContract() const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		return Contract(myDetails.Get());
	}

	template<class _Ty>
	inline std::unique_lock<std::mutex> Task<_Ty>::Lock() const noexcept
	{
		const ContractDetails& details = myDetails->Get();
		return std::unique_lock<std::mutex>(details.Mutex);
	}



	inline Task<void>::Task() noexcept :
		myDetails(nullptr)
	{
		CommonUtilities::CreateRefWrapper(&myDetails);
		ContractDetails& details = myDetails->Get();
		details.Status = CONTRACT_STATUS_WAITING;
	}
	inline Task<void>::Task(RefWrapper<ContractDetails>* aDetails) noexcept :
		myDetails(aDetails)
	{
	}
	inline Task<void>::~Task() noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		// Mutex will be deleted if reference count is 1. So lock must be released.
		if (myDetails->GetRefCount() == 1)
		{
			lk.release();
		}

		myDetails.Reset();
	}

	inline bool Task<void>::Finish()
	{
		// Position of details in memory is static, so no lock is needed to keep hold of reference.
		ContractDetails& details = myDetails->Get();
		{
			std::unique_lock<std::mutex> lk = Lock();

			if (details.Status != CONTRACT_STATUS_WAITING)
			{
				return false;
			}
			//details.Value = std::move(aResult);
			details.Status = CONTRACT_STATUS_FINISHED;
		}
		// Notify is done outside the lock, so waiting threads can immediately resume.
		details.CV.notify_all();
		return true;
	}

	inline CONTRACT_STATUS Task<void>::GetStatus() const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		const ContractDetails& details = myDetails->Get();
		return details.Status;
	}

	inline bool Task<void>::Cancel() noexcept
	{
		ContractDetails& details = myDetails->Get();
		{
			std::unique_lock<std::mutex> lk = Lock();

			if (details.Status == CONTRACT_STATUS_FINISHED)
			{
				return false;
			}

			details.Status = CONTRACT_STATUS_CANCELLED;
		}
		details.CV.notify_all();
		return true;
	}

	inline Task<void>::Contract Task<void>::GetContract() const noexcept
	{
		std::unique_lock<std::mutex> lk = Lock();
		return Contract(myDetails.Get());
	}

	inline std::unique_lock<std::mutex> Task<void>::Lock() const noexcept
	{
		const ContractDetails& details = myDetails->Get();
		return std::unique_lock<std::mutex>(details.Mutex);
	}

#pragma endregion

	//template<typename _ResultTy>
	//class Task;

	//template<typename _ResultTy>
	//class Contract;

	//template<typename _ResultTy>
	//struct ContractDetails
	//{
	//	std::mutex Mutex;
	//	std::condition_variable CV;
	//	_ResultTy Result;
	//	bool Finished;
	//};

	//template<>
	//struct ContractDetails<void>
	//{
	//	std::mutex Mutex;
	//	std::condition_variable CV;
	//	bool Finished;
	//};

	//template<typename _ResultTy>
	//class Task
	//{
	//public:
	//	typedef Contract<_ResultTy> Contract;
	//	typedef _ResultTy Result;

	//	Task();
	//	Task(const _ResultTy& aDefaultResult);

	//	bool IsFinished() const;

	//	bool WaitFor(unsigned int aMilliSeconds, Result* outResult) const;
	//	Result&& Wait() const;

	//	Contract CreateContract();

	//private:
	//	std::shared_ptr<ContractDetails<Result>> myShared;
	//};
	//template<>
	//class Task<void>
	//{
	//public:
	//	typedef Contract<void> Contract;
	//	typedef void Result;

	//	Task();

	//	bool IsFinished() const;

	//	bool WaitFor(unsigned int aMilliSeconds) const;
	//	Result Wait() const;

	//	Contract CreateContract();

	//private:
	//	std::shared_ptr<ContractDetails<Result>> myShared;
	//};

	//template<typename _ResultTy>
	//class Contract
	//{
	//	friend class Task<_ResultTy>;
	//public:
	//	typedef Task<_ResultTy> Task;
	//	typedef _ResultTy Result;

	//	bool IsFinished() const;

	//	bool Finish(const Result& aResult);
	//	bool Finish(Result&& aResult);
	//private:
	//	Contract(const std::shared_ptr<ContractDetails<Result>>& aSharedPtr);

	//	std::shared_ptr<ContractDetails<Result>> myShared;
	//};
	//template<>
	//class Contract<void>
	//{
	//	friend class Task<void>;
	//public:
	//	typedef Task<void> Task;
	//	typedef void Result;

	//	bool IsFinished() const;

	//	bool Finish();
	//private:
	//	Contract(const std::shared_ptr<ContractDetails<void>>& aSharedPtr);

	//	std::shared_ptr<ContractDetails<void>> myShared;
	//};
}