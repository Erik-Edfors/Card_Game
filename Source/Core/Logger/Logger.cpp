#include "Logger.h"
#include <sstream>

namespace logger
{
	Logger Logger::ourLogger;

	void Logger::SetVerbosity(Verbosity aVerbosity)
	{
		std::lock_guard<std::mutex> lk(ourLogger.myLogMX);
		ourLogger.myVerbosity = aVerbosity;
	}

	void Logger::SetOutputStream(std::ostream* aStream)
	{
		std::lock_guard<std::mutex> lk(ourLogger.myLogMX);
		ourLogger.myOutputStream = aStream;
		ourLogger.myStateCV.notify_all();
	}
	void Logger::SetOutputFile(const char* aPath)
	{
		std::lock_guard<std::mutex> lk(ourLogger.myLogMX);
		ourLogger.myOutputFilePath = aPath;
		ourLogger.myStateCV.notify_all();
	}

	void Logger::Wait()
	{
		std::unique_lock<std::mutex> lk(ourLogger.myLogMX);
		while (ourLogger.myQueue.empty() == false && ourLogger.myExiting == false)
		{
			ourLogger.myStateCV.wait(lk);
		}
	}

	Logger::Logger() :
		mySteadyStartPoint(std::chrono::steady_clock::now()),
		mySystemStartPoint(std::chrono::system_clock::now()),
		myVerbosity(Verbosity::Verbose),
		myOutputStream(nullptr)
	{
		myThread = std::thread(std::bind(&Logger::logRoutine, this));
		SetOutputFile("Log.txt");
	}
	Logger::~Logger()
	{
		myExiting = true;
		myStateCV.notify_all();
		myThread.join();
	}
	void Logger::logRoutine()
	{
		std::function<void(std::ostream&)> func;
		while (true)
		{
			std::stringstream ss;

			{
				std::unique_lock<std::mutex> lk(myLogMX);
				myStateCV.wait(lk,
					[this]() {
						return myQueue.empty() == false || myExiting;
					}
				);
				if (myQueue.empty() == false)
				{
					func = myQueue.front();
					myQueue.pop();
				}
			}

			if (myExiting)
			{
				break;
			}

			if (func)
			{
				(func)(ss);
				func = nullptr;
			}
			if (myOutputStream)
			{
				*myOutputStream << ss.str();
			}

			if (myOutputFileStream.is_open() == false && myOutputFilePath.empty() == false)
			{
				myOutputFileStream.open(myOutputFilePath, std::ofstream::out | std::ofstream::app | std::ofstream::ate);

				myOutputFileStream << fflush;

				myOutputFileStream << std::endl <<
					"###############################################################" "\n"
					"# LOG: " << ourLogger.mySystemStartPoint << "\n" <<
					"###############################################################" << std::endl;
			}

			if (myOutputFileStream.is_open())
			{
				myOutputFileStream << ss.str();
				myOutputFileStream.flush();
			}

			myStateCV.notify_all();
		}
	}
}
