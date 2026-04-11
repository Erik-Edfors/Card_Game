#pragma once

namespace CommonUtilities
{
	struct WriteConstraints
	{
		size_t MaxSize;
	};

	class Writer
	{
	public:
		Writer() :
			myConstraints({}),
			myCount(0u)
		{
			myConstraints.MaxSize = SIZE_MAX;
		};

		void SetConstraints(const WriteConstraints& aConstraints) noexcept { myConstraints = aConstraints; }
		const WriteConstraints& GetConstraints() const noexcept { return myConstraints; }

		size_t GetCount() const noexcept { return myCount; }

		virtual size_t Write(const void* aData, size_t aSize) = 0;
		virtual bool Writable() const { return myCount < GetConstraints().MaxSize; };

	protected:
		void ReportWrite(size_t aAmount)
		{
			myCount += aAmount;
			if (myCount > GetConstraints().MaxSize)
			{
				throw WriteException(std::format("Wrote too much! Wrote above MaxSize constraint ({}/{}).", myCount, GetConstraints().MaxSize).c_str());
			}
		}

	private:
		WriteConstraints myConstraints;
		size_t myCount;
	};
}