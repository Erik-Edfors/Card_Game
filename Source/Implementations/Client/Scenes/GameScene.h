#pragma once
#include "../Client.h"
#include "../Components/ButtonComponent.h"

#include <Core/Engine/Scene.h>
#include <Core/Engine/CommandParser.h>
#include <Core/ComponentSystem/ComponentRef.hpp>

#include <Implementations/CardGame/Game.h>
#include <Implementations/CardGame/Commands.h>

#include <Core/CommonUtilities/Events.hpp>

#include <set>

class CameraComponent;
class SpriteComponent;

namespace graphics
{
	class Window;
}

namespace client
{
	class ClientApp;
	class CardComponent;
	class LocationComponent;
	class PlayerComponent;
	class PlayerInfoComponent;

	class GameScene : public Scene
	{
	public:
		GameScene(ClientApp* aApp);
		~GameScene();

		void PlayCard(cardgame::CardID aCard);
		void EndTurn();
		void InspectCard(cardgame::CardID aCard);

		void SetPlayerID(cardgame::PlayerID aID);
		cardgame::PlayerID GetPlayerID() const noexcept;

		cardgame::Game& GetGame() noexcept;

		Client& GetClient() const noexcept;

		bool CanPlayerBeSelected(cardgame::PlayerID aPlayer) const noexcept;
		bool CanCardBeSelected(cardgame::CardID aCard) const noexcept;
		bool CanLocationBeSelected(cardgame::LocationID aLocation) const noexcept;

		bool SelectPlayer(cardgame::PlayerID aPlayer) noexcept;
		bool SelectCard(cardgame::CardID aCard) noexcept;
		bool SelectLocation(cardgame::LocationID aLocation) noexcept;

		bool UnselectPlayer(cardgame::PlayerID aPlayer) noexcept;
		bool UnselectCard(cardgame::CardID aCard) noexcept;
		bool UnselectLocation(cardgame::LocationID aLocation) noexcept;

		bool UnselectAll() noexcept;

		bool IsPlayerSelected(cardgame::PlayerID aPlayer) const noexcept;
		bool IsCardSelected(cardgame::CardID aCard) const noexcept;
		bool IsLocationSelected(cardgame::LocationID aLocation) const noexcept;

		void SubmitSelected() noexcept;

		void SubmitSelectedPlayers() noexcept;
		void SubmitSelectedCards() noexcept;
		void SubmitSelectedLocations() noexcept;

	protected:
		virtual void OnInit() override;
		virtual void OnShutdown() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;
		virtual void OnStart() override;
		virtual void OnUpdate(float aDeltaTime) override;

	private:
		static void OnCmdGame(const CommandParser::CallbackData& aData);

		static void OnAssignPlayer(const Client::MessageCallbackData<shared::NM_AssignPlayer>& aData);
		static void OnCommand(const Client::MessageCallbackData<shared::NM_Command>& aData);
		static void OnSubmissionResult(const Client::MessageCallbackData<shared::NM_SubmissionResult>& aData);

		static void OnSubmitClicked(const ButtonComponent::CallbackData& aData);
		static void OnCancelClicked(const ButtonComponent::CallbackData& aData);
		static void OnEndTurnClicked(const ButtonComponent::CallbackData& aData);

		void PushPendingCommands();


		void OnCardCreated(cardgame::Game::EventCardCreated aEvent);
		void OnCardRemoved(cardgame::Game::EventCardRemoved aEvent);
		void OnCardMoved(cardgame::Game::EventCardMoved aEvent);
		void OnLocationCreated(cardgame::Game::EventLocationCreated aEvent);
		void OnLocationRemoved(cardgame::Game::EventLocationRemoved aEvent);
		void OnPlayerCreated(cardgame::Game::EventPlayerCreated aEvent);
		void OnPlayerRemoved(cardgame::Game::EventPlayerRemoved aEvent);

		void PositionPlayers();
		void DisplayWinningPlayer(cardgame::PlayerID aPlayer);

		graphics::Window* myWindow;
		float myAspectRatio;

		cs::EntityHandle myInspectedCard;
		unsigned int myInspectionImmunityFrames;

		std::unordered_map<cardgame::CardID, cs::ComponentRef<CardComponent>> myCards;
		std::unordered_map<cardgame::LocationID, cs::ComponentRef<LocationComponent>> myLocations;
		std::unordered_map<cardgame::PlayerID, cs::ComponentRef<PlayerComponent>> myPlayers;
		std::unordered_map<cardgame::PlayerID, cs::ComponentRef<PlayerInfoComponent>> myPlayerInfos;

		bool mySelectionLocked;
		std::set<cardgame::PlayerID> mySelectedPlayers;
		std::set<cardgame::CardID> mySelectedCards;
		std::set<cardgame::LocationID> mySelectedLocations;

		CommonUtilities::EventListener myGameListener;

		std::unordered_map<size_t, cardgame::Command> myPendingCommands;

		cs::ComponentRef<CameraComponent> myCamera;
		cs::ComponentRef<ButtonComponent> mySubmitButton;
		cs::ComponentRef<ButtonComponent> myCancelButton;
		cs::ComponentRef<ButtonComponent> myEndTurnButton;

		cardgame::Game myGame;
		cardgame::PlayerID myPlayerID;
		ClientApp* myApp;
	};
}