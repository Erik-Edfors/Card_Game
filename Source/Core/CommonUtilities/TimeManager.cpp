#include "TimeManager.h"

namespace CommonUtilities
{
	TimeManager::TimeManager()
	{
		this->myStartTimePoint = std::chrono::high_resolution_clock::now();
		this->myLastTimePoint = this->myStartTimePoint;
		this->myDelta = 0;
		this->myTotal = 0;
	}
	TimeManager::~TimeManager()
	{
	}
	void TimeManager::Update()
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

		this->myDelta = std::chrono::duration_cast<std::chrono::duration<double, std::chrono::seconds::period>>(now - this->myLastTimePoint).count();
		this->myLastTimePoint = now;
		this->myTotal = std::chrono::duration_cast<std::chrono::duration<double, std::chrono::seconds::period>>(now - this->myStartTimePoint).count();
	}
	double TimeManager::GetDelta() const
	{
		return this->myDelta;
	}
	double TimeManager::GetTotal() const
	{
		return this->myTotal;
	}
}
