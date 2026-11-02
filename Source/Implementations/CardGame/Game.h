#pragma once
#include "Commands.h"
#include "CardFactory.h"
#include "Player.h"
#include "Location.h"

#include <Core/CommonUtilities/Events.hpp>

#include <map>
#include <queue>

namespace cardgame
{
	class GameObject;

	class GameObjectInterface
	{
		friend class GameObject;
	private:
		virtual bool PushTask(Task* aTask) = 0;

		virtual bool Do(SubmitPlayersCommand aCommand) = 0;
		virtual bool Do(SubmitCardsCommand aCommand) = 0;
		virtual bool Do(SubmitLocationsCommand aCommand) = 0;
		virtual bool Do(SubmitAmountCommand aCommand) = 0;
		virtual bool Do(SubmitOptionCommand aCommand) = 0;

		virtual bool Do(PlayCommand aCommand) = 0;
		virtual bool Do(MoveCardCommand aCommand) = 0;
		virtual bool Do(CreateCardCommand aCommand) = 0;
		virtual bool Do(StartCommand aCommand) = 0;
		virtual bool Do(EndTurnCommand aCommand) = 0;
		virtual bool Do(HealPlayerCommand aCommand) = 0;
		virtual bool Do(DamagePlayerCommand aCommand) = 0;
		virtual bool Do(AddPlayerResourcesCommand aCommand) = 0;
		virtual bool Do(RemovePlayerResourcesCommand aCommand) = 0;
		virtual bool Do(SetNameCommand aCommand) = 0;
	};

	class Game : public GameObjectInterface
	{
	public:
		Game();
		~Game();

		static constexpr CommonUtilities::StringHash EVENT_TASK_SUBMITTED = "EVENT_TASK_SUBMITTED";

		static constexpr CommonUtilities::StringHash EVENT_CARD_MOVED = "EVENT_CARD_MOVED";
		static constexpr CommonUtilities::StringHash EVENT_CARD_PLAYED = "EVENT_CARD_PLAYED";
		static constexpr CommonUtilities::StringHash EVENT_CARD_CREATED = "EVENT_CARD_CREATED";

		static constexpr CommonUtilities::StringHash EVENT_TURN_BEGIN = "EVENT_TURN_BEGIN";
		static constexpr CommonUtilities::StringHash EVENT_TURN_END = "EVENT_TURN_END";

		static constexpr CommonUtilities::StringHash EVENT_PLAYER_HEALED = "EVENT_PLAYER_HEALED";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_DAMAGED = "EVENT_PLAYER_DAMAGED";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_HEALTH_CHANGED = "EVENT_PLAYER_HEALTH_CHANGED";

		static constexpr CommonUtilities::StringHash EVENT_PLAYER_GAINED_RESOURCES = "EVENT_PLAYER_GAINED_RESOURCES";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_LOST_RESOURCES = "EVENT_PLAYER_LOST_RESOURCES";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_RESOURCES_CHANGED = "EVENT_PLAYER_RESOURCES_CHANGED";

		struct EventTaskSubmitted { };

		struct EventCardMoved { CardID Card; PlayerID Owner; LocationID From; LocationID To; };
		struct EventCardPlayed { CardID Card; PlayerID Owner; ResourceCount Cost; ResourceCount Payed; };
		struct EventCardCreated { CardID Card; PlayerID Owner; LocationID Location; };
		
		struct EventTurnBegin { PlayerID Owner; };
		struct EventTurnEnd { PlayerID Owner; };
		
		struct EventPlayerHealed { CardID Source; PlayerID Target; Health Amount; };
		struct EventPlayerDamaged { CardID Source; PlayerID Target; Health Amount; };
		struct EventPlayerHealthChanged { CardID Source; PlayerID Target; Health Amount; };
		
		struct EventPlayerGainedResources { CardID Source; PlayerID Target; ResourceCount Amount; };
		struct EventPlayerLostResources { CardID Source; PlayerID Target; ResourceCount Amount; };
		struct EventPlayerResourcesChanged { CardID Source; PlayerID Target; ResourceCount Amount; };

		bool Initialize(size_t aPlayerCount, const CardFactory& aCardFactory);
		bool Shutdown();

		bool PushCommand(const Command& aCommand);

		bool HasTasks() const noexcept;
		const Task* GetTask() const noexcept;

		const Player* GetPlayer(PlayerID aID) const noexcept;
		const Card* GetCard(CardID aID) const noexcept;
		const Location* GetLocation(LocationID aID) const noexcept;

		size_t GetNumPlayers() const noexcept;
		size_t GetPlayers(const Player** outPlayers, size_t aNumPlayers) const noexcept;

		size_t GetNumLocations() const noexcept;
		size_t GetLocations(const Location** outLocations, size_t aNumLocations) const noexcept;

		TurnIndex GetTurnIndex() const noexcept;
		const Player* GetTurnPlayer() const noexcept;

		size_t GetNumCommands() const noexcept;
		size_t GetCommands(Command* outCommands, size_t aStart, size_t aNumCommands) const noexcept;

		CardFactory& GetCardFactory() noexcept;
		CommonUtilities::EventPusher& GetEventPusher() noexcept;

		unsigned int GetGamestateHash() const noexcept;

		// Some function to retrieve a task, which can then be submitted.
		// Task answers should be serializable, to allow for sending them over network.

	private:
		Player* FindPlayer(PlayerID aID) noexcept;
		Card* FindCard(CardID aID) noexcept;
		Location* FindLocation(LocationID aID) noexcept;

		void Update();

		CardID CreateCardID() noexcept;
		LocationID CreateLocationID() noexcept;
		PlayerID CreatePlayerID() noexcept;

		bool PushTask(Task* aTask) override;

		bool Do(SubmitPlayersCommand aCommand) override;
		bool Do(SubmitCardsCommand aCommand) override;
		bool Do(SubmitLocationsCommand aCommand) override;
		bool Do(SubmitAmountCommand aCommand) override;
		bool Do(SubmitOptionCommand aCommand) override;

		bool Do(PlayCommand aCommand) override;
		bool Do(MoveCardCommand aCommand) override;
		bool Do(CreateCardCommand aCommand) override;
		bool Do(StartCommand aCommand) override;
		bool Do(EndTurnCommand aCommand) override;
		bool Do(HealPlayerCommand aCommand) override;
		bool Do(DamagePlayerCommand aCommand) override;
		bool Do(AddPlayerResourcesCommand aCommand) override;
		bool Do(RemovePlayerResourcesCommand aCommand) override;
		bool Do(SetNameCommand aCommand) override;

		CardID myNextCardID;
		LocationID myNextLocationID;
		PlayerID myNextPlayerID;

		CommonUtilities::EventManager myEventManager;

		TurnIndex myTurnIndex;
		Player* myTurnPlayer;

		std::map<PlayerID, std::unique_ptr<Player>> myPlayers;
		std::map<LocationID, std::unique_ptr<Location>> myLocations;
		std::map<CardID, std::unique_ptr<Card>> myCards;

		std::queue<Task*> myTaskQueue;
		std::vector<Command> myCommandStack;
		CardFactory myCardFactory;
	};
}