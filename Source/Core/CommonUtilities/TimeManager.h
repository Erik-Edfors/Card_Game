#pragma once
#include <chrono>

namespace CommonUtilities
{
	class TimeManager
	{
	public:
		TimeManager();
		~TimeManager();

		void Update();

		[[nodiscard]] double GetDelta() const;
		[[nodiscard]] double GetTotal() const;

	private:
		std::chrono::high_resolution_clock::time_point myStartTimePoint;
		std::chrono::high_resolution_clock::time_point myLastTimePoint;

		double myDelta;
		double myTotal;
	};
}

