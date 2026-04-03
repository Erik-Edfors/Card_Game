#pragma once
/* A thread safe logger for c++20 made by Erik Edfors.
* Feel free to use, just do not redistribute modified versions without explicitly stating it has been modified.
*/

#include <string>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <iomanip>
#include <fstream>
#include <chrono>

#ifndef ALLOWED_LOG_VERBOSITY
#ifdef _DEBUG
#define ALLOWED_LOG_VERBOSITY 4
#else
#define ALLOWED_LOG_VERBOSITY 2
#endif
#endif

// Logs a message with specified verbosity, category, line, message, and format arguments.
#define LOG(verbosity, category, line, message, ...) logger::Logger::Log(logger::Logger::Verbosity:: ## verbosity, category, line, message, __VA_ARGS__)

// Logs a message with current line and specified verbosity, category, line, message, and format arguments.
#define LOG_L(verbosity, category, message, ...) LOG(verbosity, category, __LINE__, message, __VA_ARGS__)

#if ALLOWED_LOG_VERBOSITY >= 4
// Logs a message with Verbose verbosity and specified category, message, and format arguments.
#define LOG_VERBOSE(category, message, ...) LOG(Verbose, category, -1, message, __VA_ARGS__)
// Logs a message with current line, Verbose verbosity and specified category, line, message, and format arguments.
#define LOG_VERBOSE_L(category, message, ...) LOG_L(Verbose, category, message, __VA_ARGS__)
#else
// Logs a message with Verbose verbosity and specified category, message, and format arguments.
#define LOG_VERBOSE(category, message, ...) void()
// Logs a message with current line, Verbose verbosity and specified category, line, message, and format arguments.
#define LOG_VERBOSE_L(category, message, ...) void()
#endif

#if ALLOWED_LOG_VERBOSITY >= 3
// Logs a message with Info verbosity and specified category, message, and format arguments.
#define LOG_INFO(category, message, ...) LOG(Info, category, -1, message, __VA_ARGS__)
// Logs a message with current line, Info verbosity and specified category, line, message, and format arguments.
#define LOG_INFO_L(category, message, ...) LOG_L(Info, category, message, __VA_ARGS__)
#else
// Logs a message with Info verbosity and specified category, message, and format arguments.
#define LOG_INFO(category, message, ...) void()
// Logs a message with current line, Info verbosity and specified category, line, message, and format arguments.
#define LOG_INFO_L(category, message, ...) void()
#endif

#if ALLOWED_LOG_VERBOSITY >= 2
// Logs a message with Warning verbosity and specified category, message, and format arguments.
#define LOG_WARNING(category, message, ...) LOG(Warning, category, -1, message, __VA_ARGS__)
// Logs a message with current line, Warning verbosity and specified category, line, message, and format arguments.
#define LOG_WARNING_L(category, message, ...) LOG_L(Warning, category, message, __VA_ARGS__)
#else
// Logs a message with Warning verbosity and specified category, message, and format arguments.
#define LOG_WARNING(category, message, ...) void()
// Logs a message with current line, Warning verbosity and specified category, line, message, and format arguments.
#define LOG_WARNING_L(category, message, ...) void()
#endif


#if ALLOWED_LOG_VERBOSITY >= 1
// Logs a message with Error verbosity and specified category, message, and format arguments.
#define LOG_ERROR(category, message, ...) LOG(Error, category, -1, message, __VA_ARGS__)
// Logs a message with current line, Error verbosity and specified category, line, message, and format arguments.
#define LOG_ERROR_L(category, message, ...) LOG_L(Error, category, message, __VA_ARGS__)
#else
// Logs a message with Error verbosity and specified category, message, and format arguments.
#define LOG_ERROR(category, message, ...) void()
// Logs a message with current line, Error verbosity and specified category, line, message, and format arguments.
#define LOG_ERROR_L(category, message, ...) void()
#endif

#if ALLOWED_LOG_VERBOSITY >= 0
// Logs a message with Fatal verbosity and specified category, message, and format arguments.
#define LOG_FATAL(category, message, ...) LOG(Fatal, category, -1, message, __VA_ARGS__)
// Logs a message with current line, Fatal verbosity and specified category, line, message, and format arguments.
#define LOG_FATAL_L(category, message, ...) LOG_L(Fatal, category, message, __VA_ARGS__)
#else
// Logs a message with Fatal verbosity and specified category, message, and format arguments.
#define LOG_FATAL(category, message, ...) void()
// Logs a message with current line, Fatal verbosity and specified category, line, message, and format arguments.
#define LOG_FATAL_L(category, message, ...) void()
#endif

namespace logger
{
	/* A thread safe logger
	*/
	class Logger
	{
	public:
		enum class Verbosity
		{
			Fatal = 0,			//An irrecoverable error.
			Error = 1,			//An error.
			Warning = 2,		//A warning.
			Info = 3,			//Info about the process.
			Verbose = 4,		//Detailed information for debugging purposes.
			COUNT
		};
		static constexpr const char* VerbosityPrefix[static_cast<size_t>(Verbosity::COUNT)] = {
			"[##|  FATAL  |##]",
			"[++|  ERROR  |++]",
			"[--| WARNING |--]",
			"[..|  INFO   |..]",
			"[  | VERBOSE |  ]"
		};

		/* Logs a message after formatting it with the given arguments.
		*/
		template<typename ..._Args>
		static void Log(Verbosity aVerbosity, const std::string& aCatagory, int aLine, const std::string& aMessage, const _Args&... aArgs);

		/* Sets the maximum Verbosity for output. Any logs with higher verbosity are discarded.
		*/
		static void SetVerbosity(Verbosity aVerbosity);
		/* Sets the output stream.
		*/
		static void SetOutputStream(std::ostream* aStream);
		/* Sets the output file.
		*/
		static void SetOutputFile(const char* aPath);
		/* Waits for all pending logs to be written to assigned outputs.
		*/
		static void Wait();

	private:
		Logger();
		~Logger();
		static Logger ourLogger;
		void logRoutine();

		template<class _Rep, class _Period, typename ..._Args>
		static void printLogToStream(std::ostream& aStream, const std::chrono::duration<_Rep, _Period>& aTime, Verbosity aVerbosity, const std::string& aCatagory, int aLine, const std::string& aMessage, _Args&&... aArgs);

		template<typename ..._Args>
		void internalLog(Verbosity aVerbosity, const std::string& aCatagory, int aLine, const std::string& aString, const _Args&... aArgs);

		std::ofstream myOutputFileStream;
		std::string myOutputFilePath;
		std::mutex myLogMX;
		std::condition_variable myStateCV;
		std::queue<std::function<void(std::ostream&)>> myQueue;
		std::thread myThread;
		std::chrono::system_clock::time_point mySystemStartPoint;
		std::chrono::steady_clock::time_point mySteadyStartPoint;
		std::ostream* myOutputStream;
		Verbosity myVerbosity;
		bool myExiting;
	};

	template<class _Duration>
	inline std::ostream& operator<<(std::ostream& aStream, const std::chrono::time_point<std::chrono::system_clock, _Duration>& aTimePoint)
	{
		std::time_t time = std::chrono::system_clock::to_time_t(aTimePoint);
		tm tm;
		localtime_s(&tm, &time);
		aStream << std::put_time(&tm, "[%Yy/%mm/%dd] [%Hh::%Mm::%Ss]");
		return aStream;
	}

	template<class _Rep, class _Period>
	inline std::ostream& operator<<(std::ostream& aStream, const std::chrono::duration<_Rep, _Period>& aDuration)
	{
		std::chrono::duration<_Rep, _Period> un = aDuration;

		std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(un);
		un -= std::chrono::duration_cast<std::chrono::duration<_Rep, _Period>>(h);

		std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(un);
		un -= std::chrono::duration_cast<std::chrono::duration<_Rep, _Period>>(m);

		std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(un);
		un -= std::chrono::duration_cast<std::chrono::duration<_Rep, _Period>>(s);

		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(un);
		un -= std::chrono::duration_cast<std::chrono::duration<_Rep, _Period>>(ms);

		char fill = aStream.fill();
		std::streamsize width = aStream.width();

		aStream << "[" << std::setfill('0') <<
			std::setw(3) << h.count() << "h::" <<
			std::setw(2) << m.count() << "m::" <<
			std::setw(2) << s.count() << "s::" <<
			std::setw(3) << ms.count() << "ms]" <<
			std::setfill(fill) << std::setw(width);

		return aStream;
	}

	inline std::string AssureStdString(const char* aCString) { return std::string(aCString); }

	template<typename _Ty>
	inline _Ty AssureStdString(_Ty aValue) { return aValue; }

	template<typename ..._Args>
	inline void Logger::Log(Verbosity aVerbosity, const std::string& aCatagory, int aLine, const std::string& aMessage, const _Args& ...aArgs)
	{
		ourLogger.internalLog(aVerbosity, aCatagory, aLine, aMessage, AssureStdString(aArgs)...);
	}
	template<class _Rep, class _Period, typename ..._Args>
	inline void Logger::printLogToStream(std::ostream& aStream, const std::chrono::duration<_Rep, _Period>& aTime, Verbosity aVerbosity, const std::string& aCatagory, int aLine, const std::string& aMessage, _Args&& ...aArgs)
	{
		aStream << aTime;
		aStream << VerbosityPrefix[static_cast<size_t>(aVerbosity)];
		aStream << "[ " << aCatagory << " ]";
		if (aLine != -1)
		{
			std::streamsize w = aStream.width();
			char fill = aStream.fill();
			aStream << "[ line: " << std::setfill('_') << std::setw(6) << aLine << " ]";
			aStream << std::setw(w) << std::setfill(fill);
		}
		aStream << ": ";
		
		static char messageBuffer[1024];
		sprintf_s(messageBuffer, sizeof(messageBuffer), aMessage.c_str(), std::forward<_Args>(aArgs)...);
		aStream << messageBuffer << std::endl;
		//aStream << std::vformat(aMessage, std::make_format_args(aArgs...)) << std::endl;
	}

	inline const char* AssureCString(const std::string& aString) { return aString.c_str(); }

	template<typename _Ty>
	inline _Ty AssureCString(_Ty aValue) { return aValue; }

	template<typename ..._Args>
	inline void Logger::internalLog(Verbosity aVerbosity, const std::string& aCatagory, int aLine, const std::string& aString, const _Args& ...aArgs)
	{
		std::lock_guard<std::mutex> lk(myLogMX);

		if (static_cast<int>(myVerbosity) < static_cast<int>(aVerbosity))
		{
			return;
		}

		auto time = std::chrono::steady_clock::now() - mySteadyStartPoint;
		myQueue.push(
			[=](std::ostream& aStream) {
				Logger::printLogToStream(aStream, time, aVerbosity, aCatagory, aLine, aString, AssureCString(aArgs)...);
			}
		);
		myStateCV.notify_all();
	}
}