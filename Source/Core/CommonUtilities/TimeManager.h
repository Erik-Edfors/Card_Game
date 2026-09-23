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

		void SetFactor(float aFactor) noexcept;

		[[nodiscard]] float GetDelta() const noexcept;
		[[nodiscard]] float GetUnmodifiedDelta() const noexcept;
		[[nodiscard]] float GetTotal() const noexcept;

	private:
		std::chrono::high_resolution_clock::time_point myStartTimePoint;
		std::chrono::high_resolution_clock::time_point myLastTimePoint;

		float myFactor;
		float myDelta;
		float myUnmodifiedDelta;
		float myTotal;
	};
}

