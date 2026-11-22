#include "Client.pch.h"
#include "GameScene.h"
#include "../ClientApp.h"
#include "../Client.h"
#include "ServerSelectScene.h"
#include "GameOverScene.h"
#include "EntityFactories.h"
#include "Components/PlayerComponent.h"
#include "Components/CardComponent.h"
#include "Components/PlayerInfoComponent.h"
#include "Components/Locations/LocationComponent.h"
#include "Components/Locations/DeckComponent.h"
#include "Components/Locations/HandComponent.h"
#include "Components/Locations/PlayComponent.h"
#include "Components/Locations/DiscardComponent.h"

#include <Core/Engine/Engine.h>
#include <Core/Engine/Console.h>
#include <Core/Engine/Components/TransformComponent.h>
#include <Core/Engine/Components/CameraComponent.h>
#include <Core/Engine/Components/CameraControllerComponent.h>

#include <Core/GraphicsEngine/GraphicsEngine.h>
#include <Core/GraphicsEngine/Projection.h>
#include <Core/GraphicsEngine/RenderLayers/SpriteRenderLayer.h>
#include <Core/GraphicsEngine/RenderLayers/TextRenderLayer.h>

#include <Core/RenderHardwareInterface/Window.h>

#include <Core/CommonUtilities/ArgsParser.hpp>
#include <Core/CommonUtilities/InputManager.h>

#include <Core/imgui/imgui.h>
#include <Core/imgui/misc/cpp/imgui_stdlib.h>

#include <Implementations/Shared/Cards/Cards.h>

#include <Implementations/CardGame/Task.h>

#define FILE_CATEGORY "GameScene.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->GameScene"

namespace client
{
	GameScene::GameScene(ClientApp* aApp) :
		myWindow(nullptr),
		myAspectRatio(1.f),
		myInspectedCard(cs::INVALID_ENTITY_HANDLE),
		myInspectionImmunityFrames(0),
		myApp(aApp),
		myPlayerID(0),
		mySelectionLocked(false)
	{
	}
	GameScene::~GameScene()
	{
	}

	void GameScene::PlayCard(cardgame::CardID aCard)
	{
		Client& client = GetClient();
		const shared::NM_PlayCardRequest playCard = {
			.Player = myPlayerID,
			.Card = aCard
		};
		shared::NetworkManager::NetworkMessage message;
		client.CreateMessage(&message, playCard, true);
		client.Send(message);
	}

	void GameScene::EndTurn()
	{
		Client& client = GetClient();
		const shared::NM_EndTurnRequest endTurn = {
		};
		shared::NetworkManager::NetworkMessage message;
		client.CreateMessage(&message, endTurn, true);
		client.Send(message);
	}

	void GameScene::InspectCard(cardgame::CardID aCard)
	{
		if (myInspectionImmunityFrames != 0)
		{
			return;
		}

		cs::ComponentSystem& system = GetComponentSystem();
		if (myInspectedCard != cs::INVALID_ENTITY_HANDLE)
		{
			system.Destroy(myInspectedCard);
			myInspectedCard = cs::INVALID_ENTITY_HANDLE;
		}

		myInspectedCard = CreateDisplayCard(*myApp, *this, myGame, aCard, myPlayerID);
		myInspectionImmunityFrames = 5;

		cs::ComponentRef<TransformComponent> transform = system.GetComponent<TransformComponent>(myInspectedCard);
		transform->SetTranslation({ 0.5f, 0.5f, 0.1f });
	}

	void GameScene::SetPlayerID(cardgame::PlayerID aID)
	{
		myPlayerID = aID;
	}

	cardgame::PlayerID GameScene::GetPlayerID() const noexcept
	{
		return myPlayerID;
	}

	cardgame::Game& GameScene::GetGame() noexcept
	{
		return myGame;
	}

	Client& GameScene::GetClient() const noexcept
	{
		return myApp->GetClient();
	}

	bool GameScene::CanPlayerBeSelected(cardgame::PlayerID aPlayer) const noexcept
	{
		if (not myGame.HasTasks())
		{
			return false;
		}

		const cardgame::Task& task = *myGame.GetTask();

		if (task.GetType() != cardgame::TASK_TARGET_PLAYERS)
		{
			return false;
		}

		const cardgame::TargetPlayersTask& targetsTask = task.AsTargetPlayers();

		if (mySelectedPlayers.size() >= std::min<unsigned int>(targetsTask.GetMaxTargets(), cardgame::MAX_SELECTED_PLAYERS))
		{
			return false;
		}

		if (not targetsTask.IsValidTarget(aPlayer))
		{
			return false;
		}

		return true;
	}
	bool GameScene::CanCardBeSelected(cardgame::CardID aCard) const noexcept
	{
		if (not myGame.HasTasks())
		{
			return false;
		}

		const cardgame::Task& task = *myGame.GetTask();

		if (task.GetType() != cardgame::TASK_TARGET_CARDS)
		{
			return false;
		}

		const cardgame::TargetCardsTask& targetsTask = task.AsTargetCards();

		if (mySelectedCards.size() >= std::min<unsigned int>(targetsTask.GetMaxTargets(), cardgame::MAX_SELECTED_CARDS))
		{
			return false;
		}

		if (not targetsTask.IsValidTarget(aCard))
		{
			return false;
		}

		return true;
	}
	bool GameScene::CanLocationBeSelected(cardgame::LocationID aLocation) const noexcept
	{
		if (not myGame.HasTasks())
		{
			return false;
		}

		const cardgame::Task& task = *myGame.GetTask();

		if (task.GetType() != cardgame::TASK_TARGET_LOCATIONS)
		{
			return false;
		}

		const cardgame::TargetLocationsTask& targetsTask = task.AsTargetLocations();

		if (mySelectedLocations.size() >= std::min<unsigned int>(targetsTask.GetMaxTargets(), cardgame::MAX_SELECTED_LOCATIONS))
		{
			return false;
		}

		if (not targetsTask.IsValidTarget(aLocation))
		{
			return false;
		}

		return true;
	}

	bool GameScene::SelectPlayer(cardgame::PlayerID aPlayer) noexcept
	{
		if (not mySelectionLocked && CanPlayerBeSelected(aPlayer))
		{
			mySelectedPlayers.insert(aPlayer);
			return true;
		}
		return false;
	}
	bool GameScene::SelectCard(cardgame::CardID aCard) noexcept
	{
		if (not mySelectionLocked && CanCardBeSelected(aCard))
		{
			mySelectedCards.insert(aCard);
			return true;
		}
		return false;
	}
	bool GameScene::SelectLocation(cardgame::LocationID aLocation) noexcept
	{
		if (not mySelectionLocked && CanLocationBeSelected(aLocation))
		{
			mySelectedLocations.insert(aLocation);
			return true;
		}
		return false;
	}

	bool GameScene::UnselectPlayer(cardgame::PlayerID aPlayer) noexcept
	{
		if (mySelectionLocked)
		{
			return false;
		}
		return mySelectedPlayers.erase(aPlayer) != 0;
	}

	bool GameScene::UnselectCard(cardgame::CardID aCard) noexcept
	{
		if (mySelectionLocked)
		{
			return false;
		}
		return mySelectedCards.erase(aCard) != 0;
	}

	bool GameScene::UnselectLocation(cardgame::LocationID aLocation) noexcept
	{
		if (mySelectionLocked)
		{
			return false;
		}
		return mySelectedLocations.erase(aLocation) != 0;
	}

	bool GameScene::UnselectAll() noexcept
	{
		if (mySelectionLocked)
		{
			return false;
		}
		mySelectedPlayers.clear();
		mySelectedCards.clear();
		mySelectedLocations.clear();
		return true;
	}

	bool GameScene::IsPlayerSelected(cardgame::PlayerID aPlayer) const noexcept
	{
		return mySelectedPlayers.contains(aPlayer);
	}
	bool GameScene::IsCardSelected(cardgame::CardID aCard) const noexcept
	{
		return mySelectedCards.contains(aCard);
	}
	bool GameScene::IsLocationSelected(cardgame::LocationID aLocation) const noexcept
	{
		return mySelectedLocations.contains(aLocation);
	}

	void GameScene::SubmitSelected() noexcept
	{
		if (not myGame.HasTasks())
		{
			return;
		}

		const cardgame::Task& task = *myGame.GetTask();

		switch (task.GetType())
		{
		case cardgame::TASK_TARGET_PLAYERS: SubmitSelectedPlayers(); break;
		case cardgame::TASK_TARGET_CARDS: SubmitSelectedCards(); break;
		case cardgame::TASK_TARGET_LOCATIONS: SubmitSelectedLocations(); break;
			//TODO: Implement other tasks
		}
	}

	void GameScene::SubmitSelectedPlayers() noexcept
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SubmitSelectedPlayers"

		if (mySelectionLocked)
		{
			return;
		}

		shared::NM_SubmitPlayers submitPlayers = {
			.Players = {},
			.Count = 0
		};

		for (cardgame::PlayerID player : mySelectedPlayers)
		{
			if (submitPlayers.Count >= cardgame::MAX_SELECTED_PLAYERS)
			{
				break;
			}
			submitPlayers.Players[submitPlayers.Count++] = player;
		}

		shared::NetworkManager::NetworkMessage message;
		if (not GetClient().CreateMessage(&message, submitPlayers, true))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to submit selected players. Client::CreateMessage failed.");
			return;
		}
		if (not GetClient().Send(message))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to submit selected players. Client::Send failed.");
			return;
		}

		mySelectionLocked = true;

#undef LOG_CATEGORY
	}
	void GameScene::SubmitSelectedCards() noexcept
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SubmitSelectedCards"

		if (mySelectionLocked)
		{
			return;
		}

		shared::NM_SubmitCards submitCards = {
			.Cards = {},
			.Count = 0
		};

		for (cardgame::CardID card : mySelectedCards)
		{
			if (submitCards.Count >= cardgame::MAX_SELECTED_CARDS)
			{
				break;
			}
			submitCards.Cards[submitCards.Count++] = card;
		}

		shared::NetworkManager::NetworkMessage message;
		if (not GetClient().CreateMessage(&message, submitCards, true))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to submit selected cards. Client::CreateMessage failed.");
			return;
		}
		if (not GetClient().Send(message))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to submit selected cards. Client::Send failed.");
			return;
		}

		mySelectionLocked = true;

#undef LOG_CATEGORY
	}
	void GameScene::SubmitSelectedLocations() noexcept
	{
#define LOG_CATEGORY CLASS_CATEGORY"::SubmitSelectedCards"

		if (mySelectionLocked)
		{
			return;
		}

		shared::NM_SubmitLocations submitLocations = {
			.Locations = {},
			.Count = 0
		};

		for (cardgame::LocationID location : mySelectedLocations)
		{
			if (submitLocations.Count >= cardgame::MAX_SELECTED_LOCATIONS)
			{
				break;
			}
			submitLocations.Locations[submitLocations.Count++] = location;
		}

		shared::NetworkManager::NetworkMessage message;
		if (not GetClient().CreateMessage(&message, submitLocations, true))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to submit selected locations. Client::CreateMessage failed.");
			return;
		}
		if (not GetClient().Send(message))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to submit selected locations. Client::Send failed.");
			return;
		}

		mySelectionLocked = true;

#undef LOG_CATEGORY
	}

	void GameScene::OnInit()
	{
		myWindow = GetEngine().GetGraphicsEngine().GetWindow();

		if (not RegisterComponentTypes(GetComponentSystem()))
		{
			//TODO: Log.
		}

		myGameListener.SetPusher(&myGame.GetEventPusher());
		myGameListener.BeginListen(cardgame::Game::EVENT_CARD_CREATED);
		myGameListener.BeginListen(cardgame::Game::EVENT_CARD_REMOVED);
		myGameListener.BeginListen(cardgame::Game::EVENT_CARD_MOVED);
		myGameListener.BeginListen(cardgame::Game::EVENT_PLAYER_CREATED);
		myGameListener.BeginListen(cardgame::Game::EVENT_PLAYER_REMOVED);
		myGameListener.BeginListen(cardgame::Game::EVENT_TASK_FINISHED);

		cs::ComponentSystem& cs = GetComponentSystem();

		const cs::EntityHandle cameraHandle = CreateCamera(*myApp, *this);
		myCamera = cs.GetComponent<CameraComponent>(cameraHandle);

		const cs::EntityHandle submitButtonHandle = CreateButton(*myApp, *this, L"Submit", OnSubmitClicked, this);
		cs::ComponentRef<TransformComponent> submitButtonTransform = cs.GetComponent<TransformComponent>(submitButtonHandle);
		submitButtonTransform->SetTranslation({0.9f, 0.6f, 0.1f});
		mySubmitButton = cs.GetComponent<ButtonComponent>(submitButtonHandle);

		const cs::EntityHandle cancelButtonHandle = CreateButton(*myApp, *this, L"Cancel", OnCancelClicked, this);
		cs::ComponentRef<TransformComponent> cancelButtonTransform = cs.GetComponent<TransformComponent>(cancelButtonHandle);
		cancelButtonTransform->SetTranslation({ 0.9f, 0.56f, 0.1f });
		myCancelButton = cs.GetComponent<ButtonComponent>(cancelButtonHandle);

		const cs::EntityHandle endTurnButtonHandle = CreateButton(*myApp, *this, L"End Turn", OnEndTurnClicked, this);
		cs::ComponentRef<TransformComponent> endTurnTransform = cs.GetComponent<TransformComponent>(endTurnButtonHandle);
		endTurnTransform->SetTranslation({ 0.9f, 0.51f, 0.1f });
		myEndTurnButton = cs.GetComponent<ButtonComponent>(endTurnButtonHandle);

		cardgame::CardFactory factory;
		shared::RegisterCards(factory);

		if (not myGame.Initialize(factory))
		{

		}
	}
	void GameScene::OnShutdown()
	{
		Client& client = myApp->GetClient();

		client.SetMessageCallback<shared::NM_Command>(nullptr, this);
		client.SetMessageCallback<shared::NM_AssignPlayer>(nullptr, this);
		GetEngine().GetCommandParser().RemoveCommand("game");
	}
	void GameScene::OnEnter()
	{
		Client& client = myApp->GetClient();

		client.SetMessageCallback<shared::NM_Command>(&OnCommand, this);
		client.SetMessageCallback<shared::NM_AssignPlayer>(&OnAssignPlayer, this);
		client.SetMessageCallback<shared::NM_SubmissionResult>(&OnSubmissionResult, this);

		GetEngine().GetCommandParser().AddCommand("game", &OnCmdGame, this);
	}
	void GameScene::OnExit()
	{
		Client& client = myApp->GetClient();

		client.SetMessageCallback<shared::NM_Command>(nullptr, nullptr);
		client.SetMessageCallback<shared::NM_AssignPlayer>(nullptr, nullptr);
		client.SetMessageCallback<shared::NM_SubmissionResult>(nullptr, nullptr);

		GetEngine().GetCommandParser().RemoveCommand("game");
	}
	void GameScene::OnStart()
	{
		Client& client = myApp->GetClient();

		shared::NM_GameReady gameReady;
		shared::NetworkManager::NetworkMessage message;
		client.CreateMessage(&message, gameReady, true);
		client.Send(message);
	}
	void GameScene::OnUpdate(float aDeltaTime)
	{
		aDeltaTime;

		if (GetClient().GetConnectionStatus() != CLIENT_CONNECTION_CONNECTED)
		{
			GetEngine().ChangeScene<ServerSelectScene>(myApp);
			return;
		}

		cs::ComponentSystem& system = GetComponentSystem();
		const CommonUtilities::InputManager& input = GetEngine().GetInputManager();

		const float aspectRatio = myWindow->GetClientWidth() / myWindow->GetClientHeight();
		if (aspectRatio != myAspectRatio)
		{
			if (aspectRatio < 1.f)
			{
				const float padding = ((1.f / aspectRatio) - 1.f) / 2.f;
				myCamera->SetProjection(CreateOrthographicProjection(0.f, 1.f, 0.f - padding, 1.f + padding, 0.f, 1.f));
			}
			else
			{
				const float padding = (aspectRatio - 1.f) / 2.f;
				myCamera->SetProjection(CreateOrthographicProjection(0.f - padding, 1.f + padding, 0.f, 1.f, 0.f, 1.f));
			}
			myAspectRatio = aspectRatio;
		}

		const CommonUtilities::Matrix4x4f view = myCamera->GetView();
		const CommonUtilities::Matrix4x4f projection = myCamera->GetProjection();

		SpriteRenderLayer* spriteRL = GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
		TextRenderLayer* textRL = GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();

		spriteRL->SetView(view);
		spriteRL->SetProjection(projection);

		textRL->SetView(view);
		textRL->SetProjection(projection);

		Client& client = myApp->GetClient();
		client;

		if (client.GetConnectionStatus() != client::CLIENT_CONNECTION_CONNECTED)
		{
			GetEngine().ChangeScene<ServerSelectScene>(myApp);
			return;
		}

		CommonUtilities::Event event;
		while (myGameListener.GetEvent(&event))
		{
			switch (event.ID)
			{
			case cardgame::Game::EVENT_CARD_CREATED: OnCardCreated(std::any_cast<cardgame::Game::EventCardCreated>(event.Data)); break;
			case cardgame::Game::EVENT_CARD_REMOVED: OnCardRemoved(std::any_cast<cardgame::Game::EventCardRemoved>(event.Data)); break;
			case cardgame::Game::EVENT_CARD_MOVED: OnCardMoved(std::any_cast<cardgame::Game::EventCardMoved>(event.Data)); break;
			case cardgame::Game::EVENT_LOCATION_CREATED: OnLocationCreated(std::any_cast<cardgame::Game::EventLocationCreated>(event.Data)); break;
			case cardgame::Game::EVENT_LOCATION_REMOVED: OnLocationRemoved(std::any_cast<cardgame::Game::EventLocationRemoved>(event.Data)); break;
			case cardgame::Game::EVENT_PLAYER_CREATED: OnPlayerCreated(std::any_cast<cardgame::Game::EventPlayerCreated>(event.Data)); break;
			case cardgame::Game::EVENT_PLAYER_REMOVED: OnPlayerRemoved(std::any_cast<cardgame::Game::EventPlayerRemoved>(event.Data)); break;
			}
		}

		if (myInspectionImmunityFrames != 0)
		{
			--myInspectionImmunityFrames;
		}
		else if (myInspectedCard != cs::INVALID_ENTITY_HANDLE)
		{
			cs::ComponentRef<CardComponent> card = system.GetComponent<CardComponent>(myInspectedCard);
			if (not card.Expired())
			{
				const bool clicked = input.GetKeyDown(CommonUtilities::Keys::MOUSELBUTTON)
					|| input.GetKeyDown(CommonUtilities::Keys::MOUSERBUTTON);

				if (clicked && not card->IsHovered())
				{
					system.Destroy(myInspectedCard);
					myInspectedCard = cs::INVALID_ENTITY_HANDLE;
				}
			}
			else
			{
				system.Destroy(myInspectedCard);
				myInspectedCard = cs::INVALID_ENTITY_HANDLE;
			}
		}

		const bool canEndTurn = myGame.GetTurnPlayer() != nullptr && myGame.GetTurnPlayer() == myGame.GetPlayer(myPlayerID);
		const bool canDoTask = myGame.HasTasks() && myGame.GetTask()->GetAuthority() == myPlayerID;

		myEndTurnButton->SetButtonEnabled(canEndTurn);
		mySubmitButton->SetButtonEnabled(canDoTask);
		myCancelButton->SetButtonEnabled(canDoTask);
	}

	void GameScene::OnCmdGame(const CommandParser::CallbackData& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		Console& console = scene.GetEngine().GetConsole();

		CommonUtilities::ArgsParser parser;
		parser.Parse(aData.Arguments, aData.NumArguments);

		size_t numArguments, numOptions, numFlags;
		const CommonUtilities::Argument* arguments;
		const CommonUtilities::Option* options;
		const CommonUtilities::Flag* flags;

		parser.GetArguments(&arguments, &numArguments);
		parser.GetOptions(&options, &numOptions);
		parser.GetFlags(&flags, &numFlags);

		if (numArguments >= 2)
		{
			switch (CommonUtilities::StringHash(arguments[1].GetValue()))
			{
			case "hash"_hash:
			{
				console.PrintLine(std::format("Hash: {:#0x}", scene.myGame.GetGamestateHash()));
			}
			break;
			case "stack"_hash:
			{
				size_t numCommands = scene.myGame.GetNumCommands();
				cardgame::Command* commands = new cardgame::Command[numCommands];
				scene.myGame.GetCommands(commands, 0, numCommands);
				for (size_t i = 0; i < numCommands; i++)
				{
					const cardgame::Command& command = commands[i];
					unsigned int hash = CommonUtilities::TStringHash<cardgame::Command>(&command, 1);

					console.PrintLine(std::format("[{}]: {:#0x}", command.Type, hash));
				}
				delete[] commands;
			}
			break;
			default:
				console.PrintLine(std::format("Unknown argument! {}", arguments[1].GetValue()));
				break;
			}
		}
	}

	void GameScene::OnAssignPlayer(const Client::MessageCallbackData<shared::NM_AssignPlayer>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

		scene.myPlayerID = aData.Message.PlayerID;

		scene.PositionPlayers();
	}
	void GameScene::OnCommand(const Client::MessageCallbackData<shared::NM_Command>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

		const cardgame::Command& command = aData.Message.Command;
		const size_t index = aData.Message.Index;
		const size_t expectedIndex = scene.myGame.GetNumCommands();

		scene.GetEngine().GetConsole().PrintLine(std::format("Recieved Command: {}.", command.Type));

		// If a command is recieved out of order.
		// It is saved and later pushed when all previous command have been recieved and pushed.
		if (index != expectedIndex)
		{
			if (index < expectedIndex)
			{
				//TODO: Log error!
			}
			else
			{
				scene.myPendingCommands[index] = command;
			}
		}
		else
		{
			if (not scene.myGame.PushCommand(command))
			{
				//TODO: Handle invalid command.
			}
			scene.PushPendingCommands();
		}

	}
	void GameScene::OnSubmissionResult(const Client::MessageCallbackData<shared::NM_SubmissionResult>& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

		scene.mySelectionLocked = false;

		if (aData.Message.Succeeded)
		{
			scene.UnselectAll();
		}
	}
	void GameScene::OnSubmitClicked(const ButtonComponent::CallbackData& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

		scene.SubmitSelected();
	}
	void GameScene::OnCancelClicked(const ButtonComponent::CallbackData& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);

		scene.UnselectAll();
	}
	void GameScene::OnEndTurnClicked(const ButtonComponent::CallbackData& aData)
	{
		GameScene& scene = *reinterpret_cast<GameScene*>(aData.UserData);
		scene.EndTurn();
	}
	void GameScene::PushPendingCommands()
	{
		size_t expectedIndex = myGame.GetNumCommands();

		auto it = myPendingCommands.find(expectedIndex);
		while (it != myPendingCommands.end())
		{
			if (not myGame.PushCommand(it->second))
			{
				//TODO: Log failure to push command.
			}

			myPendingCommands.erase(it);
			expectedIndex = myGame.GetNumCommands();
			it = myPendingCommands.find(expectedIndex);
		}
	}
	void GameScene::OnCardCreated(cardgame::Game::EventCardCreated aEvent)
	{
		cs::ComponentSystem& system = GetComponentSystem();
		cs::EntityHandle entity = CreateCard(*myApp, *this, myGame, aEvent.Card, myPlayerID);

		cs::ComponentRef<CardComponent> card = system.GetComponent<CardComponent>(entity);
		myCards[card->GetCard()->GetID()] = card;

		cs::ComponentRef<TransformComponent> transform = GetComponentSystem().GetComponent<TransformComponent>(entity);
		transform->SetTranslation({ 0.75f * static_cast<float>(aEvent.Card), 0.25f, 0.5f });

		auto locationIt = myLocations.find(aEvent.Location);
		if (locationIt != myLocations.end())
		{
			locationIt->second->Add(system.GetComponent<CardComponent>(entity));
		}
	}
	void GameScene::OnCardRemoved(cardgame::Game::EventCardRemoved aEvent)
	{
		auto it = myCards.find(aEvent.Card);
		if (it == myCards.end())
		{
			return;
		}
		const cs::ComponentRef<CardComponent> card = it->second;
		GetComponentSystem().Destroy(card->GetEntityHandle());
		myCards.erase(it);
	}
	void GameScene::OnCardMoved(cardgame::Game::EventCardMoved aEvent)
	{
		auto cardIt = myCards.find(aEvent.Card);
		if (cardIt == myCards.end())
		{
			//TODO: Log.
			return;
		}
		CardComponent* const card = cardIt->second;

		auto fromIt = myLocations.find(aEvent.From);
		if (fromIt != myLocations.end())
		{
			LocationComponent* const fromLocation = fromIt->second.Get();
			fromLocation->Remove(card);
		}

		auto toIt = myLocations.find(aEvent.To);
		if (toIt != myLocations.end())
		{
			LocationComponent* const toLocation = toIt->second.Get();
			toLocation->Add(card);
		}
	}
	void GameScene::OnLocationCreated(cardgame::Game::EventLocationCreated aEvent)
	{
		aEvent;
	}
	void GameScene::OnLocationRemoved(cardgame::Game::EventLocationRemoved aEvent)
	{
		auto it = myLocations.find(aEvent.Location);
		if (it == myLocations.end())
		{
			return;
		}
		const LocationComponent* location = it->second.Get();
		GetComponentSystem().Destroy(location->GetEntityHandle());
		myLocations.erase(it);
	}
	void GameScene::OnPlayerCreated(cardgame::Game::EventPlayerCreated aEvent)
	{
		const cardgame::Player* player = myGame.GetPlayer(aEvent.Player);

		cs::ComponentSystem& cs = GetComponentSystem();

		const cardgame::PlayerID id = player->GetID();
		const cardgame::LocationID deck = player->GetDeckLocation();
		const cardgame::LocationID hand = player->GetHandLocation();
		const cardgame::LocationID play = player->GetPlayLocation();
		const cardgame::LocationID discard = player->GetDiscardLocation();

		cs::ComponentRef<PlayerComponent> playerComp = cs.GetComponent<PlayerComponent>(CreatePlayer(*myApp, *this, myGame, id));

		myPlayers[id] = playerComp;
		myLocations[deck] = playerComp->GetDeck();
		myLocations[hand] = playerComp->GetHand();
		myLocations[play] = playerComp->GetPlay();
		myLocations[discard] = playerComp->GetDiscard();

		PositionPlayers();
	}
	void GameScene::OnPlayerRemoved(cardgame::Game::EventPlayerRemoved aEvent)
	{
		auto it = myPlayers.find(aEvent.Player);
		if (it == myPlayers.end())
		{
			return;
		}

		cs::ComponentSystem& system = GetComponentSystem();

		cs::ComponentRef<PlayerComponent> player = it->second;

		system.Destroy(player->GetEntityHandle());

		myPlayers.erase(aEvent.Player);

		PositionPlayers();

		if (myPlayers.size() == 1)
		{
			DisplayWinningPlayer(myPlayers.begin()->first);
		}
	}

	struct _LocationPositionData
	{
		CommonUtilities::Vector3f Position;
		CommonUtilities::Quaternionf Rotation;
	};

	struct _PlayerPositionData
	{
		_LocationPositionData DeckPosition;
		_LocationPositionData HandPosition;
		_LocationPositionData PlayPosition;
		_LocationPositionData DiscardPosition;
		_LocationPositionData InfoPosition;
		_LocationPositionData PlayerPosition;
	};

	void GameScene::PositionPlayers()
	{
		static constexpr float _Factor = 1.f;

		static constexpr CommonUtilities::Vector2f cardsOffset = { 0.5f * 0.125f, 0.5f * -0.25f };

		static constexpr const _PlayerPositionData localData = {
			.DeckPosition = {.Position = CommonUtilities::Vector3f(0.0f + cardsOffset.x, 0.1f + cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(1, 0, 0, 0)},
			.HandPosition = {.Position = CommonUtilities::Vector3f(0.5f + cardsOffset.x, 0.15f + cardsOffset.y, 0.5f) * _Factor, .Rotation = CommonUtilities::Quaternionf(1, 0, 0, 0)},
			.PlayPosition = {.Position = CommonUtilities::Vector3f(0.3f + cardsOffset.x, 0.475f + cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(1, 0, 0, 0)},
			.DiscardPosition = {.Position = CommonUtilities::Vector3f(0.0f + cardsOffset.x, 0.3f + cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(1, 0, 0, 0)},
			.InfoPosition = {.Position = CommonUtilities::Vector3f(0.0f, 0.5f, 0.25f) * _Factor, .Rotation = CommonUtilities::Quaternionf(1, 0, 0, 0)},
			.PlayerPosition = {.Position = CommonUtilities::Vector3f(0.9f, 0.1f, 0.25f) * _Factor, .Rotation = CommonUtilities::Quaternionf(1, 0, 0, 0)}
		};

		static constexpr const _PlayerPositionData singularOpponentData = {
			.DeckPosition = {.Position = CommonUtilities::Vector3f(1.f - cardsOffset.x, 0.9f - cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0, 0, 0, 1) },
			.HandPosition = {.Position = CommonUtilities::Vector3f(0.5f - cardsOffset.x, 0.85f - cardsOffset.y, 0.5f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0, 0, 0, 1) },
			.PlayPosition = {.Position = CommonUtilities::Vector3f(0.7f - cardsOffset.x, 0.525f - cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0, 0, 0, 1) },
			.DiscardPosition = {.Position = CommonUtilities::Vector3f(1.f - cardsOffset.x, 0.7f - cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0, 0, 0, 1) },
			.InfoPosition = {.Position = CommonUtilities::Vector3f(0.0f, 0.45f, 0.25f) * _Factor, .Rotation = CommonUtilities::Quaternionf(1, 0, 0, 0) },
			.PlayerPosition = {.Position = CommonUtilities::Vector3f(0.1f, 0.9f, 0.25f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0, 0, 0, 1)}
		};

		static constexpr const _PlayerPositionData twoOpponentData[2] = {
			{
				.DeckPosition = {.Position = CommonUtilities::Vector3f(0.8f - cardsOffset.x, 0.1f - cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0.7071068f, 0.f, -0.7071068f, 0.f) },
				.HandPosition = {.Position = CommonUtilities::Vector3f(0.5f - cardsOffset.x, 0.05f - cardsOffset.y, 0.5f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0.7071068f, 0.f, -0.7071068f, 0.f) },
				.PlayPosition = {.Position = CommonUtilities::Vector3f(0.2f - cardsOffset.x, 0.1f - cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0.7071068f, 0.f, -0.7071068f, 0.f) },
				.DiscardPosition = {.Position = CommonUtilities::Vector3f(0.8f - cardsOffset.x, 0.2f - cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0.7071068f, 0.f, -0.7071068f, 0.f) },
				.InfoPosition = {.Position = CommonUtilities::Vector3f(0.05f, 0.1f, 0.25f) * _Factor, .Rotation = CommonUtilities::Quaternionf(1, 0, 0, 0) },
				.PlayerPosition = {.Position = CommonUtilities::Vector3f(0.1f, 0.1f, 0.25f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0, 0, 0, 1) }
			},
			{
				.DeckPosition = {.Position = CommonUtilities::Vector3f(0.8f - cardsOffset.x, 0.1f - cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0.7071068f, 0.f, 0.7071068f, 0.f) },
				.HandPosition = {.Position = CommonUtilities::Vector3f(0.5f - cardsOffset.x, 0.05f - cardsOffset.y, 0.5f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0.7071068f, 0.f, 0.7071068f, 0.f) },
				.PlayPosition = {.Position = CommonUtilities::Vector3f(0.2f - cardsOffset.x, 0.1f - cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0.7071068f, 0.f, 0.7071068f, 0.f) },
				.DiscardPosition = {.Position = CommonUtilities::Vector3f(0.8f - cardsOffset.x, 0.2f - cardsOffset.y, 0.75f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0.7071068f, 0.f, 0.7071068f, 0.f) },
				.InfoPosition = {.Position = CommonUtilities::Vector3f(0.05f, 0.1f, 0.25f) * _Factor, .Rotation = CommonUtilities::Quaternionf(1, 0, 0, 0) },
				.PlayerPosition = {.Position = CommonUtilities::Vector3f(0.9f, 0.9f, 0.25f) * _Factor, .Rotation = CommonUtilities::Quaternionf(0, 0, 0, 1) }			}
		};

		cs::ComponentSystem& system = GetComponentSystem();

		size_t opponentIndex = 0;
		size_t numOpponents = 0;

		for (auto& entry : myPlayers)
		{
			if (entry.first != myPlayerID)
			{
				++numOpponents;
			}
		}

		for (auto& entry : myPlayers)
		{
			const cardgame::PlayerID playerID = entry.first;
			cs::ComponentRef<PlayerComponent> player = entry.second;

			const _PlayerPositionData* data = nullptr;

			if (playerID == myPlayerID)
			{
				data = &localData;
			}
			else
			{
				if (numOpponents == 1)
				{
					data = &singularOpponentData;
				}
				else if (numOpponents == 2)
				{
					data = &twoOpponentData[opponentIndex];
				}
				else
				{
					continue;
				}

				++opponentIndex;
			}

			{
				TransformComponent* transform = system.GetComponent<TransformComponent>(player->GetDeck()->GetEntityHandle());
				transform->SetTranslation(data->DeckPosition.Position);
				transform->SetRotation(data->DeckPosition.Rotation);
			}
			{
				TransformComponent* transform = system.GetComponent<TransformComponent>(player->GetHand()->GetEntityHandle());
				transform->SetTranslation(data->HandPosition.Position);
				transform->SetRotation(data->HandPosition.Rotation);
			}
			{
				TransformComponent* transform = system.GetComponent<TransformComponent>(player->GetPlay()->GetEntityHandle());
				transform->SetTranslation(data->PlayPosition.Position);
				transform->SetRotation(data->PlayPosition.Rotation);
			}
			{
				TransformComponent* transform = system.GetComponent<TransformComponent>(player->GetDiscard()->GetEntityHandle());
				transform->SetTranslation(data->DiscardPosition.Position);
				transform->SetRotation(data->DiscardPosition.Rotation);
			}
			{
				TransformComponent* transform = system.GetComponent<TransformComponent>(player->GetPlayerInfo()->GetEntityHandle());
				transform->SetTranslation(data->InfoPosition.Position);
				transform->SetRotation(data->InfoPosition.Rotation);
			}
			{
				TransformComponent* transform = system.GetComponent<TransformComponent>(player->GetEntityHandle());
				transform->SetTranslation(data->PlayerPosition.Position);
				transform->SetRotation(data->PlayerPosition.Rotation);
			}
		}
	}
	void GameScene::DisplayWinningPlayer(cardgame::PlayerID aPlayer)
	{
		GameOverScene* scene = GetEngine().ChangeScene<GameOverScene>(myApp);

		const cardgame::Player* player = myGame.GetPlayer(aPlayer);

		if (player != nullptr)
		{
			scene->SetWinnerName(player->GetName());
		}
		else
		{
			scene->SetWinnerName("Nobody");
		}

		GetClient().Disconnect(shared::NETWORK_DISCONNECT_END_OF_SESSION);
	}
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY