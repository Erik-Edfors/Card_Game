#pragma once

namespace CommonUtilities
{
	struct ReadConstraints
	{
		size_t MaxSize;
	};

	class Reader
	{
	public:
		Reader() : 
			myConstraints({}),
			myCount(0u)
		{
			myConstraints.MaxSize = SIZE_MAX;
		};

		void SetConstraints(const ReadConstraints& aConstraints) noexcept { myConstraints = aConstraints; }
		const ReadConstraints& GetConstraints() const noexcept { return myConstraints; }

		size_t GetCount() const noexcept { return myCount; }

		virtual size_t Read(void* outData, size_t aSize) = 0;
		virtual bool Readable() const { return myCount < GetConstraints().MaxSize; }

	protected:
		void ReportRead(size_t aAmount) 
		{ 
			myCount += aAmount;
			if (myCount > GetConstraints().MaxSize)
			{
				throw ReadException(std::format("Read too much! Read above MaxSize constraint ({}/{}).", myCount, GetConstraints().MaxSize).c_str());
			}
		}

	private:
		ReadConstraints myConstraints;
		size_t myCount;
	};
}