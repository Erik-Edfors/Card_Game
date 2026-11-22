#include "Client.pch.h"
#include "PlayComponent.h"
#include "Components/CardComponent.h"
#include <Core/Engine/Components/TransformComponent.h>

namespace client
{
	PlayComponent::PlayComponent(cardgame::LocationID aID) :
		myWidth(5),
		LocationComponent(aID)
	{
	}
	PlayComponent::~PlayComponent()
	{
	}

	void PlayComponent::SetWidth(unsigned int aWidth) noexcept
	{
		myWidth = aWidth;
	}
	unsigned int PlayComponent::GetWidth() const noexcept
	{
		return myWidth;
	}

	void PlayComponent::OnAdd(CardComponent* aCard)
	{
		aCard;
		UpdateCardPositions();
	}
	void PlayComponent::OnRemove(CardComponent* aCard)
	{
		aCard;
		UpdateCardPositions();
	}

	void PlayComponent::UpdateCardPositions()
	{
		const std::vector<cs::ComponentRef<CardComponent>>& cards = GetCards();

		for (unsigned int i = 0; i < cards.size(); i++)
		{
			CardComponent* const card = cards[i].Get();
			const cs::EntityHandle entity = card->GetEntityHandle();

			TransformComponent* transform = GetSystem().GetComponent<TransformComponent>(entity);

			const float xOffset = static_cast<float>(i % myWidth) * 0.125f;
			const float yOffset = static_cast<float>(i / myWidth) * 0.225f;

			transform->SetTranslation({ xOffset, -yOffset, 0.f });
			transform->SetRotation(CommonUtilities::Quaternionf({ 0.f, 1.f, 0.f }, 0.f));
		}
	}
}
