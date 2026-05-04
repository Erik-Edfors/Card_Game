#pragma once
#include <utility>

namespace shared
{
	class CommandBase
	{
	public:
		virtual void Do() = 0;
		virtual void Undo() = 0;
	};

	template<class... _Args>
	class Command : public CommandBase
	{
	public:

	protected:
		virtual void OnInit(_Args&&... aArgs) { aArgs; };

	private:
		void Initialize(_Args&&... aArgs);
	};

	template<class ..._Args>
	inline void Command<_Args...>::Initialize(_Args && ...aArgs)
	{
		OnInit(std::forward(aArgs)...);
	}
}