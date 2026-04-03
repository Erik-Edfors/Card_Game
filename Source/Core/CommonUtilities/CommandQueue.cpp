#include "CommandQueue.h"

size_t CommonUtilities::CommandQueue::Count() const
{
	return myQueue.size();
}

bool CommonUtilities::CommandQueue::Empty() const
{
	return myQueue.empty();
}

void CommonUtilities::CommandQueue::RunCommand()
{
	if (myQueue.empty() == false)
	{
		myQueue.front()();
		myQueue.pop();
	}
}
