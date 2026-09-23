#include "GraphicsEngine.pch.h"
#include "Common.h"
#include <fstream>

bool LoadFileIntoMemory(void** outBuffer, size_t* outSize, const char* aFilePath, size_t aInitialBufferSize, size_t aMaxSize)
{
	std::ifstream file(aFilePath, std::ios::binary);

	if (file.is_open() == false)
	{
		return false;
	}

	//size_t bufferSize = aInitialBufferSize;	//1 MB
	//size_t cursor = 0;
	//char* buffer = new char[bufferSize];

	//do
	//{
	//	if (cursor == bufferSize)
	//	{
	//		bufferSize = bufferSize << 1;
	//		if (bufferSize > aMaxSize)
	//		{
	//			delete[] buffer;
	//			file.close();
	//			return false;
	//		}
	//		char* newBuffer = new char[bufferSize];
	//		memcpy(newBuffer, buffer, cursor);
	//		delete[] buffer;
	//		buffer = newBuffer;
	//	}

	//	file.read(&buffer[cursor], bufferSize - cursor);

	//	cursor += file.gcount();

	//} while (file.eof() == false);

	//file.close();

	//*outBuffer = buffer;
	//*outSize = cursor;
	return LoadStreamIntoMemory(outBuffer, outSize, file, aInitialBufferSize, aMaxSize);
}

bool LoadStreamIntoMemory(void** outBuffer, size_t* outSize, std::istream& aStream, size_t aInitialBufferSize, size_t aMaxSize)
{
	size_t bufferSize = aInitialBufferSize;	//1 MB
	size_t cursor = 0;
	char* buffer = new char[bufferSize];

	do
	{
		if (cursor == bufferSize)
		{
			bufferSize = bufferSize << 1;
			if (bufferSize > aMaxSize)
			{
				delete[] buffer;
				return false;
			}
			char* newBuffer = new char[bufferSize];
			memcpy(newBuffer, buffer, cursor);
			delete[] buffer;
			buffer = newBuffer;
		}

		aStream.read(&buffer[cursor], bufferSize - cursor);

		cursor += aStream.gcount();

	} while (aStream.eof() == false && not aStream.fail());

	*outBuffer = buffer;
	*outSize = cursor;
	return true;
}
