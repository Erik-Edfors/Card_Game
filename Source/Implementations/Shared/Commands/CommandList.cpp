#include "Shared.pch.h"
#include "CommandList.h"

namespace shared
{
	CommandList::CommandList() :
		myNextID(INVALID_ID + 1)
	{
	}
	CommandList::~CommandList()
	{
	}
	size_t CommandList::Undo(ID aID)
	{
		const size_t index = IndexOf(aID);
		if (index >= myCommands.size())
		{
			return 0;
		}

		size_t count = 0;
		while (index < myCommands.size())
		{
			if (Undo())
			{
				count++;
			}
			else
			{
				break;
			}
		}

		return count;
	}
	size_t CommandList::Undo(ID aFirst, ID aLast)
	{
		const size_t first = IndexOf(aFirst);
		const size_t last = IndexOf(aLast);

		if (first >= myCommands.size() || last >= myCommands.size() || first > last)
		{
			return 0;
		}

		size_t current = myCommands.size();
		size_t count = 0;
		do
		{
			--current;
			++count;
			myCommands[current].Command->Undo();
		} while (current > first);

		for (size_t i = last + 1; i < myCommands.size(); i++)
		{
			myCommands[i].Command->Do();
		}

		myCommands.erase(myCommands.begin() + first, myCommands.begin() + last + 1);
		return count;
	}
	bool CommandList::Undo()
	{
		if (myCommands.empty())
		{
			return false;
		}

		__CommandEntry& entry = myCommands.back();
		entry.Command->Undo();
		myCommands.pop_back();
		return true;
	}
	CommandList::ID CommandList::Current() const noexcept
	{
		if (myCommands.empty())
		{
			return INVALID_ID;
		}
		
		return myCommands.back().ID;
	}
	CommandList::ID CommandList::Next(ID aID) const noexcept
	{
		if (myCommands.empty())
		{
			return INVALID_ID;
		}

		size_t index = IndexOf(aID);
		if (index >= myCommands.size() - 1)
		{
			return INVALID_ID;
		}
		else
		{
			return myCommands[index + 1].ID;
		}
	}
	CommandList::ID CommandList::Previous(ID aID) const noexcept
	{
		if (myCommands.empty())
		{
			return INVALID_ID;
		}

		size_t index = IndexOf(aID);
		if (index >= myCommands.size() || index == 0)
		{
			return INVALID_ID;
		}
		else
		{
			return myCommands[index - 1].ID;
		}
	}
	CommandList::ID CommandList::GenID() noexcept
	{
		return myNextID++;
	}
	size_t CommandList::IndexOf(ID aID) const noexcept
	{
		for (size_t i = 0; i < myCommands.size(); i++)
		{
			if (myCommands[i].ID == aID)
			{
				return i;
			}
		}
		return myCommands.size();
	}
}
