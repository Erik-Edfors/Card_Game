#pragma once
#include <memory>
#include <vector>
#include "Command.h"

namespace shared
{
	class CommandList
	{
	public:
		using ID = unsigned int;

		static constexpr ID INVALID_ID = 0;

		CommandList();
		~CommandList();

		template<class _CommandTy, class... _Args>
		ID Do(_Args&&... aArgs);

		size_t Undo(ID aID);
		size_t Undo(ID aFirst, ID aLast);
		bool Undo();

		ID Current() const noexcept;
		ID Next(ID aID) const noexcept;
		ID Previous(ID aID) const noexcept;

	private:
		struct __CommandEntry
		{
			ID ID;
			std::unique_ptr<CommandBase> Command;
		};

		ID GenID() noexcept;
		size_t IndexOf(ID aID) const noexcept;

		std::vector<__CommandEntry> myCommands;
		ID myNextID;
	};


	template<class _CommandTy, class ..._Args>
	inline CommandList::ID CommandList::Do(_Args && ...aArgs)
	{
		_CommandTy* command = new _CommandTy();

		__CommandEntry& entry = myCommands.emplace_back();

		entry.Command = std::unique_ptr<CommandBase>(command);
		entry.ID = GenID();

		command->Initialize(std::forward<_Args>(aArgs)...);
		command->Do();
		return entry.ID;
	}
}