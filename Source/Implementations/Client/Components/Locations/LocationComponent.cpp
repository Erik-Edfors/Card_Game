#include "Client.pch.h"
#include "LocationComponent.h"

#include "Components/CardComponent.h"

#include <Core/Engine/Components/TransformComponent.h>

namespace client
{
	LocationComponent::LocationComponent(cardgame::LocationID aID) :
		myID(aID)
	{

	}
	LocationComponent::~LocationComponent()
	{
	}

	cardgame::LocationID LocationComponent::GetID() const noexcept
	{
		return myID;
	}

	void LocationComponent::Add(CardComponent* aCard)
	{
		myCards.push_back(aCard);
		TransformComponent* transform = GetSystem().GetComponent<TransformComponent>(aCard->GetEntityHandle());
		transform->SetParent(GetTransform());
		Clean();
		OnAdd(aCard);
	}
	bool LocationComponent::Remove(CardComponent* aCard)
	{
		const bool result = std::erase(myCards, aCard) != 0;
		if (result)
		{
			Clean();
			TransformComponent* transform = GetSystem().GetComponent<TransformComponent>(aCard->GetEntityHandle());
			if (transform->GetParent() == GetTransform())
			{
				transform->SetParent(nullptr);
			}
			OnRemove(aCard);
		}
		return result;
	}
	const std::vector<cs::ComponentRef<CardComponent>> LocationComponent::GetCards() noexcept
	{
		Clean();
		return myCards;
	}
	TransformComponent* LocationComponent::GetTransform() const noexcept
	{
		if (not myTransform)
		{
			myTransform = GetSystem().GetComponent<TransformComponent>(GetEntityHandle());
		}
		return myTransform.Get();
	}
	void LocationComponent::Clean()
	{
		std::erase_if(
			myCards,
			[](const cs::ComponentRef<CardComponent>& aCard)
			{
				return aCard.Expired();
			}
		);
	}
}