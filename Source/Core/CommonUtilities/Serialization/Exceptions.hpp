#pragma once
#include <exception>
#include <format>

namespace CommonUtilities
{
	using Exception = std::exception;

	//class StreamException : public Exception
	//{
	//public:
	//	StreamException();
	//	StreamException(const char* aMessage);
	//};

	class AllocationException : public Exception
	{
	public:
		AllocationException();
		AllocationException(const char* aMessage);
	};

	class ReadException : public Exception
	{
	public: 
		ReadException();
		ReadException(const char* aMessage);
	};

	class WriteException : public Exception
	{
	public:
		WriteException();
		WriteException(const char* aMessage);
	};

	class MalformedDataException : public Exception
	{
	public:
		MalformedDataException();
		MalformedDataException(const char* aMessage);
	};



	inline AllocationException::AllocationException() :
		Exception("Data was malformed!")
	{
	}
	inline AllocationException::AllocationException(const char* aMessage) :
		Exception(std::format("Allocation Failed! {}", aMessage).c_str())
	{
	}



	inline MalformedDataException::MalformedDataException() : 
		Exception("Data was malformed!")
	{
	}
	inline MalformedDataException::MalformedDataException(const char* aMessage) : 
		Exception(std::format("Data was malformed! {}", aMessage).c_str())
	{
	}



	inline ReadException::ReadException() : 
		Exception("Failed to read from stream!")
	{
	}
	inline ReadException::ReadException(const char* aMessage) :
		Exception(std::format("Failed to read from stream! {}", aMessage).c_str())
	{
	}


	inline WriteException::WriteException() :
		Exception("Failed to write to stream!")
	{
	}
	inline WriteException::WriteException(const char* aMessage) :
		Exception(std::format("Failed to write to stream! {}", aMessage).c_str())
	{
	}
}