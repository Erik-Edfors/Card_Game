#pragma once
#include "Defines.h"
#include <string>

namespace cardgame
{
	enum : unsigned char
	{
		TASK_TARGET_PLAYERS,
		TASK_TARGET_CARDS,
		TASK_TARGET_LOCATIONS,
		TASK_AMOUNT,
		TASK_OPTION,
	};
	using TASK_TYPE = unsigned char;

	class TargetPlayersTask;
	class TargetCardsTask;
	class TargetLocationsTask;
	class AmountTask;
	class OptionTask;

	class Task
	{
	public:
		virtual TASK_TYPE GetType() const = 0;
		virtual const char* GetDescriptor() const = 0;
		virtual bool IsFinished() const = 0;

		TargetPlayersTask& AsTargetPlayers();
		TargetCardsTask& AsTargetCards();
		TargetLocationsTask& AsTargetLocations();
		AmountTask& AsAmount();
		OptionTask& AsOption();
		const TargetPlayersTask& AsTargetPlayers() const;
		const TargetCardsTask& AsTargetCards() const;
		const TargetLocationsTask& AsTargetLocations() const;
		const AmountTask& AsAmount() const;
		const OptionTask& AsOption() const;
	};

	template<class _TargetIDTy>
	class TargetsTask : public Task
	{
	public:
		using TargetID = _TargetIDTy;

		virtual unsigned int GetMinTargets() const = 0;
		virtual unsigned int GetMaxTargets() const = 0;
		virtual bool IsValidTarget(TargetID aID) const = 0;
		virtual bool Submit(TargetID* aTargets, size_t aNumTargets) = 0;
	};

	class TargetPlayersTask : public TargetsTask<PlayerID>
	{
	};
	class TargetCardsTask : public TargetsTask<CardID>
	{
	};
	class TargetLocationsTask : public TargetsTask<LocationID>
	{
	};

	class AmountTask : public Task
	{
	public:
		virtual int GetMin() const = 0;
		virtual int GetMax() const = 0;
		virtual bool Submit(int aAmount) = 0;
	};
	class OptionTask : public Task
	{
	public:
		struct Option
		{
			std::string Descriptor;
		};

		virtual size_t GetNumOptions() const = 0;
		virtual const Option* GetOption(size_t aIndex) const = 0;
		virtual bool Submit(size_t aIndex) = 0;
	};

	inline TargetPlayersTask& Task::AsTargetPlayers()
	{
		return static_cast<TargetPlayersTask&>(*this);
	}
	inline TargetCardsTask& Task::AsTargetCards()
	{
		return static_cast<TargetCardsTask&>(*this);
	}
	inline TargetLocationsTask& Task::AsTargetLocations()
	{
		return static_cast<TargetLocationsTask&>(*this);
	}
	inline AmountTask& Task::AsAmount()
	{
		return static_cast<AmountTask&>(*this);
	}
	inline OptionTask& Task::AsOption()
	{
		return static_cast<OptionTask&>(*this);
	}
	inline const TargetPlayersTask& Task::AsTargetPlayers() const
	{
		return static_cast<const TargetPlayersTask&>(*this);
	}
	inline const TargetCardsTask& Task::AsTargetCards() const
	{
		return static_cast<const TargetCardsTask&>(*this);
	}
	inline const TargetLocationsTask& Task::AsTargetLocations() const
	{
		return static_cast<const TargetLocationsTask&>(*this);
	}
	inline const AmountTask& Task::AsAmount() const
	{
		return static_cast<const AmountTask&>(*this);
	}
	inline const OptionTask& Task::AsOption() const
	{
		return static_cast<const OptionTask&>(*this);
	}
}