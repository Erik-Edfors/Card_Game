#pragma once
#include "Exceptions.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

#include <istream>
#include <ostream>

namespace CommonUtilities
{
	class StreamReader : public Reader
	{
	public:
		StreamReader(std::istream& aStream);
		~StreamReader();

		size_t Read(void* outData, size_t aSize) override;
		bool Readable() const override { return not myStream.eof(); }

	private:
		std::istream& myStream;
	};

	class StreamWriter : public Writer
	{
	public:
		StreamWriter(std::ostream& aStream);
		~StreamWriter();

		size_t Write(const void* aData, size_t aSize) override;
		bool Writable() const override { return not myStream.fail(); }

	private:
		std::ostream& myStream;
	};



	inline StreamReader::StreamReader(std::istream& aStream) :
		myStream(aStream)
	{
	}
	inline StreamReader::~StreamReader() = default;

	inline size_t StreamReader::Read(void* outData, size_t aSize)
	{
		if (not Readable())
		{
			throw ReadException(std::format("Could not read! Reader was not readable.").c_str());
		}

		//if (GetCount() + aSize > GetConstraints().MaxSize)
		//{
		//	throw Exception(std::format("Read exceeded constraint MaxSize! Read '{} -> {}' would exceed constraint '{}'.", GetCount(), GetCount() + aSize, GetConstraints().MaxSize).c_str());
		//}

		size_t readCount = 0;
		while (myStream && readCount != aSize)
		{
			myStream.read(reinterpret_cast<char*>(outData), aSize - readCount);
			readCount += myStream.gcount();
		}

		//ReportRead(readCount);

		return readCount;
	}



	inline StreamWriter::StreamWriter(std::ostream& aStream) :
		myStream(aStream)
	{
	}
	inline StreamWriter::~StreamWriter() = default;

	inline size_t StreamWriter::Write(const void* aData, size_t aSize)
	{
		if (not Writable())
		{
			throw WriteException(std::format("Could not write! Writer was not writable.").c_str());
		}

		//if (GetCount() + aSize > GetConstraints().MaxSize)
		//{
		//	throw Exception(std::format("Write exceeded constraint MaxSize! Write '{} -> {}' would exceed constraint '{}'.", GetCount(), GetCount() + aSize, GetConstraints().MaxSize).c_str());
		//}

		size_t count = myStream.rdbuf()->sputn(reinterpret_cast<const char*>(aData), aSize);

		//ReportWrite(count);

		return count;
	}
}