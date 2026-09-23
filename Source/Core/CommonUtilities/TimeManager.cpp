#include "TimeManager.h"

namespace CommonUtilities
{
	TimeManager::TimeManager() : 
		myFactor(1.f),
		myUnmodifiedDelta(0.f),
		myDelta(0.f)
	{
		myStartTimePoint = std::chrono::steady_clock::now();
		myLastTimePoint = myStartTimePoint;
	}
	TimeManager::~TimeManager()
	{
	}
	void TimeManager::Update()
	{
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

		myUnmodifiedDelta = std::chrono::duration_cast<std::chrono::duration<float>>(now - myLastTimePoint).count();
		myDelta = myUnmodifiedDelta * myFactor;

		myLastTimePoint = now;
		myTotal = std::chrono::duration_cast<std::chrono::duration<float>>(now - myStartTimePoint).count();
	}
	void TimeManager::SetFactor(float aFactor) noexcept
	{
		myFactor = aFactor;
	}
	float TimeManager::GetDelta() const noexcept
	{
		return myDelta;
	}
	float TimeManager::GetUnmodifiedDelta() const noexcept
	{
		return myUnmodifiedDelta;
	}
	float TimeManager::GetTotal() const noexcept
	{
		return myTotal;
	}
}
