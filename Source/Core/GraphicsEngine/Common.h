#pragma once
#include <istream>

bool LoadFileIntoMemory(void** outBuffer, size_t* outSize, const char* aFilePath, size_t aInitialBufferSize = 1048576ull, size_t aMaxSize = 1073741824ull);
bool LoadStreamIntoMemory(void** outBuffer, size_t* outSize, std::istream& aStream, size_t aInitialBufferSize = 1048576ull, size_t aMaxSize = 1073741824ull);