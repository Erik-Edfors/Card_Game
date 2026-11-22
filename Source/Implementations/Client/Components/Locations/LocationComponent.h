#pragma once
#include <Core/ComponentSystem/Component.h>
#include <Core/ComponentSystem/ComponentRef.hpp>
#include <Core/CommonUtilities/Math/Vector3.hpp>
#include <Implementations/CardGame/Defines.h>

#include <unordered_map>

class TransformComponent;

namespace client
{
	class CardComponent;

	class LocationComponent : public cs::Component
	{
	public:
		LocationComponent(cardgame::LocationID aID);
		virtual ~LocationComponent();

		cardgame::LocationID GetID() const noexcept;

		void Add(CardComponent* aCard);
		bool Remove(CardComponent* aCard);

		const std::vector<cs::ComponentRef<CardComponent>> GetCards() noexcept;

		TransformComponent* GetTransform() const noexcept;

	protected:
		virtual void OnAdd(CardComponent* aCard) { aCard; };
		virtual void OnRemove(CardComponent* aCard) { aCard; };

		void Clean();

	private:
		std::vector<cs::ComponentRef<CardComponent>> myCards;
		mutable cs::ComponentRef<TransformComponent> myTransform;
		cardgame::LocationID myID;
	};
}