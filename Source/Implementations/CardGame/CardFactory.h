#pragma once
#include "Defines.h"
#include "Card.h"

#include <memory>

namespace cardgame
{
	class Game;

	class CardFactory
	{
	public:
		CardFactory();
		~CardFactory();

		std::unique_ptr<Card> CreateCard(CardFactoryID aFactoryID, Game* aGame, CardID aCardID);

		template<class _CardTy>
		void RegisterCard(CardFactoryID aID);

	private:
		template<class _CardTy>
		static std::unique_ptr<Card> ConstructCard(Game* aGame, CardID aID);

		using CardConstructor = std::unique_ptr<Card>(*)(Game* aGame, CardID aID);

		std::unordered_map<CardFactoryID, CardConstructor> myConstructors;
	};

	template<class _CardTy>
	inline void CardFactory::RegisterCard(CardFactoryID aID)
	{
		myConstructors[aID] = &ConstructCard<_CardTy>;
	}

	template<class _CardTy>
	inline std::unique_ptr<Card> CardFactory::ConstructCard(Game* aGame, CardID aID)
	{
		_CardTy* card = new _CardTy();
		card->Init(aGame, aID);
		return std::unique_ptr<Card>(card);
	}
}