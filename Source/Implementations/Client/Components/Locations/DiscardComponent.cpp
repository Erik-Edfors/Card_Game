#include "Client.pch.h"
#include "DiscardComponent.h"
#include "Components/CardComponent.h"
#include <Core/Engine/Components/TransformComponent.h>

namespace client
{
	DiscardComponent::DiscardComponent(cardgame::LocationID aID) : 
		LocationComponent(aID)
	{
	}
	DiscardComponent::~DiscardComponent()
	{
	}
	void DiscardComponent::OnAdd(CardComponent* aCard)
	{
		aCard;
		UpdateCardPositions();
	}
	void DiscardComponent::OnRemove(CardComponent* aCard)
	{
		aCard;
		UpdateCardPositions();
	}
	void DiscardComponent::UpdateCardPositions()
	{
		const std::vector<cs::ComponentRef<CardComponent>>& cards = GetCards();

		for (size_t i = 0; i < cards.size(); i++)
		{
			CardComponent* const card = cards[i].Get();
			const cs::EntityHandle entity = card->GetEntityHandle();

			TransformComponent* transform = GetSystem().GetComponent<TransformComponent>(entity);

			const float offset = static_cast<float>(i);

			transform->SetTranslation({ 0.f, 0.f, -offset * 0.01f });
			transform->SetRotation(CommonUtilities::Quaternionf({ 0.f, 1.f, 0.f }, 0.f));
		}
	}
}
