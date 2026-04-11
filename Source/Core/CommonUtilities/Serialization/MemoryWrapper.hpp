#pragma once
#include "Exceptions.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

#include <limits>
#include <cstring>

namespace CommonUtilities
{
	class MemoryReader : public Reader
	{
	public:
		MemoryReader(const void* aMemory, size_t aSize);
		~MemoryReader();

		size_t Read(void* outData, size_t aSize) override;
		bool Readable() const override { return myCursor < mySize && Reader::Readable(); }

		void SetCursor(size_t aCursor) noexcept { myCursor = aCursor; }
		size_t GetCursor() const noexcept { return myCursor; }

	private:
		const void* myMemory;
		size_t mySize;
		size_t myCursor;
	};

	class MemoryWriter : public Writer
	{
	public:
		MemoryWriter(void* aMemory, size_t aSize);
		~MemoryWriter();

		size_t Write(const void* aData, size_t aSize) override;
		bool Writable() const override { return myCursor < mySize && Writer::Writable(); }

		void SetCursor(size_t aCursor) noexcept { myCursor = aCursor; }
		size_t GetCursor() const noexcept { return myCursor; }

	private:
		void* myMemory;
		size_t mySize;
		size_t myCursor;
	};



	inline MemoryReader::MemoryReader(const void* aMemory, size_t aSize) :
		myMemory(aMemory),
		mySize(aSize),
		myCursor(0)
	{
	}
	inline MemoryReader::~MemoryReader() = default;

	inline size_t MemoryReader::Read(void* outData, size_t aSize)
	{
		if (not Readable())
		{
			throw ReadException(std::format("Could not read! Reader was not readable.").c_str());
		}

		if (GetCount() + aSize > GetConstraints().MaxSize)
		{
			throw Exception(std::format("Read exceeded constraint MaxSize! Read '{} -> {}' would exceed constraint '{}'.", GetCount(), GetCount() + aSize, GetConstraints().MaxSize).c_str());
		}

		size_t readCount = aSize;
		if (myCursor + aSize >= mySize)
		{
			readCount = mySize - myCursor;
		}
		memcpy_s(outData, aSize, reinterpret_cast<const char*>(myMemory) + myCursor, readCount);
		myCursor += readCount;

		ReportRead(readCount);

		return readCount;
	}



	inline MemoryWriter::MemoryWriter(void* aMemory, size_t aSize) :
		myMemory(aMemory),
		mySize(aSize),
		myCursor(0)
	{
	}
	inline MemoryWriter::~MemoryWriter() = default;

	inline size_t MemoryWriter::Write(const void* aData, size_t aSize)
	{
		if (not Writable())
		{
			throw WriteException(std::format("Could not write! Writer was not writable.").c_str());
		}

		if (GetCount() + aSize > GetConstraints().MaxSize)
		{
			throw Exception(std::format("Write exceeded constraint MaxSize! Write '{} -> {}' would exceed constraint '{}'.", GetCount(), GetCount() + aSize, GetConstraints().MaxSize).c_str());
		}

		size_t writeCount = aSize;
		if (myCursor + aSize >= mySize)
		{
			writeCount = mySize - myCursor;
		}
		memcpy_s(reinterpret_cast<char*>(myMemory) + myCursor, mySize - myCursor, aData, writeCount);
		myCursor += writeCount;

		ReportWrite(writeCount);

		return writeCount;
	}
}