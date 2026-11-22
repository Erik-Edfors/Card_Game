#include "Client.pch.h"
#include "OpponentHandComponent.h"
#include "Components/CardComponent.h"
#include <Core/Engine/Components/TransformComponent.h>

namespace client
{
	OpponentHandComponent::OpponentHandComponent(cardgame::LocationID aID) :
		LocationComponent(aID)
	{
	}
	OpponentHandComponent::~OpponentHandComponent()
	{
	}
	void OpponentHandComponent::OnAdd(CardComponent* aCard)
	{
		aCard;
		UpdateCardPositions();
	}
	void OpponentHandComponent::OnRemove(CardComponent* aCard)
	{
		aCard;
		UpdateCardPositions();
	}
	void OpponentHandComponent::UpdateCardPositions()
	{
		const std::vector<cs::ComponentRef<CardComponent>>& cards = GetCards();

		for (size_t i = 0; i < cards.size(); i++)
		{
			CardComponent* const card = cards[i].Get();
			const cs::EntityHandle entity = card->GetEntityHandle();

			TransformComponent* transform = GetSystem().GetComponent<TransformComponent>(entity);

			const float fromCenter = static_cast<float>(i) - static_cast<float>(cards.size()) / 2.f;

			transform->SetTranslation({ fromCenter * 0.5f, 0.f, fromCenter * 0.1f });
			transform->SetRotation(CommonUtilities::Quaternionf({ 0.f, 1.f, 0.f }, -0.1f * fromCenter));
		}
	}
}