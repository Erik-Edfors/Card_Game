#include "Client.pch.h"
#include "PlayerInfoComponent.h"

#include "Components/PlayerComponent.h"

#include <Core/Engine/Components/SpriteComponent.h>
#include <Core/Engine/Components/TextComponent.h>

namespace client
{
	PlayerInfoComponent::PlayerInfoComponent(cardgame::Game* aGame, cardgame::PlayerID aPlayer, SpriteComponent* aSprite, TextComponent* aText) :
		myGame(aGame),
		myPlayer(aPlayer),
		mySprite(aSprite),
		myText(aText)
	{
	}
	PlayerInfoComponent::~PlayerInfoComponent()
	{
	}
	void PlayerInfoComponent::OnInit()
	{
		myGameListener.SetPusher(&myGame->GetEventPusher());
		myGameListener.BeginListen(cardgame::Game::EVENT_PLAYER_HEALTH_CHANGED);
		myGameListener.BeginListen(cardgame::Game::EVENT_PLAYER_RESOURCES_CHANGED);
		myGameListener.BeginListen(cardgame::Game::EVENT_PLAYER_NAME_CHANGED);
	}
	void PlayerInfoComponent::OnStart()
	{
		UpdateInfo();
	}
	void PlayerInfoComponent::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		CommonUtilities::Event event;
		while (myGameListener.GetEvent(&event))
		{
			switch (event.ID)
			{
			case cardgame::Game::EVENT_PLAYER_HEALTH_CHANGED:
			case cardgame::Game::EVENT_PLAYER_RESOURCES_CHANGED:
			case cardgame::Game::EVENT_PLAYER_NAME_CHANGED:
				UpdateInfo();
				break;
			}
		}
	}
	void PlayerInfoComponent::UpdateInfo()
	{
		if (not myText.Expired())
		{
			const cardgame::Player* player = myGame->GetPlayer(myPlayer);
			if (player != nullptr)
			{
				myText->SetText(std::format("{:<12} | {:>2}HP | {:>2}RP", player->GetName(), player->GetHealth(), player->GetResourceCount()).c_str());
			}
		}
	}
}
