#pragma once
#include <utility>

namespace CommonUtilities
{
	template<typename _FstTy, typename _SndType>
	class Tuple
	{
	public:
		Tuple() = default;
		Tuple(Tuple<_FstTy, _SndType>&& aTuple);
		Tuple(const Tuple<_FstTy, _SndType>& aTuple);
		Tuple(_FstTy&& aFst, _SndType&& aSnd);
		Tuple(const _FstTy& aFst, const _SndType& aSnd);

		Tuple<_FstTy, _SndType>& operator=(const Tuple<_FstTy, _SndType>& aTuple);
		Tuple<_FstTy, _SndType>& operator=(Tuple<_FstTy, _SndType>&& aTuple);

		_FstTy first;
		_SndType second;
	};
	template<typename _FstTy, typename _SndType>
	inline Tuple<_FstTy, _SndType>::Tuple(Tuple<_FstTy, _SndType>&& aTuple) :
		first(std::move(aTuple.first)),
		second(std::move(aTuple.second))
	{
	}
	template<typename _FstTy, typename _SndType>
	inline Tuple<_FstTy, _SndType>::Tuple(const Tuple<_FstTy, _SndType>& aTuple) : 
		first(aTuple.first),
		second(aTuple.second)
	{
	}
	template<typename _FstTy, typename _SndType>
	inline Tuple<_FstTy, _SndType>::Tuple(_FstTy&& aFst, _SndType&& aSnd) : 
		first(aFst),
		second(aSnd)
	{
	}
	template<typename _FstTy, typename _SndType>
	inline Tuple<_FstTy, _SndType>::Tuple(const _FstTy& aFst, const _SndType& aSnd) : 
		first(aFst),
		second(aSnd)
	{
	}
	template<typename _FstTy, typename _SndType>
	inline Tuple<_FstTy, _SndType>& Tuple<_FstTy, _SndType>::operator=(const Tuple<_FstTy, _SndType>& aTuple)
	{
		first = aTuple.first;
		second = aTuple.second;
		return *this;
	}
	template<typename _FstTy, typename _SndType>
	inline Tuple<_FstTy, _SndType>& Tuple<_FstTy, _SndType>::operator=(Tuple<_FstTy, _SndType>&& aTuple)
	{
		first = std::move(aTuple.first);
		second = std::move(aTuple.second);
	}
}