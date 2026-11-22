#pragma once
#include "Defines.h"
#include "Commands.h"
#include "CardFactory.h"
#include "Player.h"
#include "Location.h"

#include <Core/CommonUtilities/Events.hpp>

#include <map>
#include <queue>
#include <random>

namespace cardgame
{
	class GameObject;

	class GameObjectInterface
	{
		friend class GameObject;
	private:
		virtual bool PushTask(Task* aTask) = 0;
		virtual bool RetractTask(Task* aTask) = 0;

		virtual bool Do(SubmitPlayersCommand aCommand, SubmitPlayersCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(SubmitCardsCommand aCommand, SubmitCardsCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(SubmitLocationsCommand aCommand, SubmitLocationsCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(SubmitAmountCommand aCommand, SubmitAmountCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(SubmitOptionCommand aCommand, SubmitOptionCommand::Result* outResult = nullptr) = 0;

		virtual bool Do(PlayCommand aCommand, PlayCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(MoveCardCommand aCommand, MoveCardCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(CreateCardCommand aCommand, CreateCardCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(RemoveCardCommand aCommand, RemoveCardCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(CreateLocationCommand aCommand, CreateLocationCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(RemoveLocationCommand aCommand, RemoveLocationCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(CreatePlayerCommand aCommand, CreatePlayerCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(RemovePlayerCommand aCommand, RemovePlayerCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(StartCommand aCommand, StartCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(EndTurnCommand aCommand, EndTurnCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(HealPlayerCommand aCommand, HealPlayerCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(DamagePlayerCommand aCommand, DamagePlayerCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(AddPlayerResourcesCommand aCommand, AddPlayerResourcesCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(RemovePlayerResourcesCommand aCommand, RemovePlayerResourcesCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(SetNameCommand aCommand, SetNameCommand::Result* outResult = nullptr) = 0;

		virtual bool Do(SetSeedCommand aCommand, SetSeedCommand::Result* outResult = nullptr) = 0;
		virtual bool Do(GenerateUniformRandomIntegerCommand aCommand, GenerateUniformRandomIntegerCommand::Result* outResult = nullptr) = 0;
	};

	class Game : public GameObjectInterface
	{
	public:
		Game();
		~Game();

		static constexpr CommonUtilities::StringHash EVENT_TASK_FINISHED = "EVENT_TASK_FINISHED";

		static constexpr CommonUtilities::StringHash EVENT_CARD_MOVED = "EVENT_CARD_MOVED";
		static constexpr CommonUtilities::StringHash EVENT_CARD_PLAYED = "EVENT_CARD_PLAYED";
		static constexpr CommonUtilities::StringHash EVENT_CARD_CREATED = "EVENT_CARD_CREATED";
		static constexpr CommonUtilities::StringHash EVENT_CARD_REMOVED = "EVENT_CARD_REMOVED";

		static constexpr CommonUtilities::StringHash EVENT_TURN_BEGIN = "EVENT_TURN_BEGIN";
		static constexpr CommonUtilities::StringHash EVENT_TURN_END = "EVENT_TURN_END";

		static constexpr CommonUtilities::StringHash EVENT_LOCATION_CREATED = "EVENT_LOCATION_CREATED";
		static constexpr CommonUtilities::StringHash EVENT_LOCATION_REMOVED = "EVENT_LOCATION_REMOVED";

		static constexpr CommonUtilities::StringHash EVENT_PLAYER_CREATED = "EVENT_PLAYER_CREATED";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_REMOVED = "EVENT_PLAYER_REMOVED";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_HEALED = "EVENT_PLAYER_HEALED";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_DAMAGED = "EVENT_PLAYER_DAMAGED";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_HEALTH_CHANGED = "EVENT_PLAYER_HEALTH_CHANGED";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_NAME_CHANGED = "EVENT_PLAYER_NAME_CHANGED";

		static constexpr CommonUtilities::StringHash EVENT_PLAYER_GAINED_RESOURCES = "EVENT_PLAYER_GAINED_RESOURCES";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_LOST_RESOURCES = "EVENT_PLAYER_LOST_RESOURCES";
		static constexpr CommonUtilities::StringHash EVENT_PLAYER_RESOURCES_CHANGED = "EVENT_PLAYER_RESOURCES_CHANGED";

		struct EventTaskFinished { TASK_TYPE Type; };

		struct EventCardMoved { CardID Card; LocationID From; LocationID To; };
		struct EventCardPlayed { CardID Card; ResourceCount Cost; ResourceCount Payed; };
		struct EventCardCreated { CardID Card; LocationID Location; };
		struct EventCardRemoved { CardID Card; };

		struct EventTurnBegin { PlayerID Owner; };
		struct EventTurnEnd { PlayerID Owner; };

		struct EventLocationCreated { LocationID Location; };
		struct EventLocationRemoved { LocationID Location; };

		struct EventPlayerCreated { PlayerID Player; };
		struct EventPlayerRemoved { PlayerID Player; };
		struct EventPlayerHealed { CardID Source; PlayerID Target; Health Amount; };
		struct EventPlayerDamaged { CardID Source; PlayerID Target; Health Amount; };
		struct EventPlayerHealthChanged { CardID Source; PlayerID Target; Health Amount; };
		struct EventPlayerNameChanged { PlayerID Player; };

		struct EventPlayerGainedResources { CardID Source; PlayerID Target; ResourceCount Amount; };
		struct EventPlayerLostResources { CardID Source; PlayerID Target; ResourceCount Amount; };
		struct EventPlayerResourcesChanged { CardID Source; PlayerID Target; ResourceCount Amount; };

		bool Initialize(const CardFactory& aCardFactory);
		bool Shutdown();

		bool PushCommand(const Command& aCommand, Command::Result* outResult = nullptr);

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
		bool RemoveDeadPlayers();

		CardID CreateCardID() noexcept;
		LocationID CreateLocationID() noexcept;
		PlayerID CreatePlayerID() noexcept;

		bool PushTask(Task* aTask) override;
		bool RetractTask(Task* aTask) override;

		bool Do(SubmitPlayersCommand aCommand, SubmitPlayersCommand::Result* outResult = nullptr) override;
		bool Do(SubmitCardsCommand aCommand, SubmitCardsCommand::Result* outResult = nullptr) override;
		bool Do(SubmitLocationsCommand aCommand, SubmitLocationsCommand::Result* outResult = nullptr) override;
		bool Do(SubmitAmountCommand aCommand, SubmitAmountCommand::Result* outResult = nullptr) override;
		bool Do(SubmitOptionCommand aCommand, SubmitOptionCommand::Result* outResult = nullptr) override;

		bool Do(PlayCommand aCommand, PlayCommand::Result* outResult = nullptr) override;
		bool Do(MoveCardCommand aCommand, MoveCardCommand::Result* outResult = nullptr) override;
		bool Do(CreateCardCommand aCommand, CreateCardCommand::Result* outResult = nullptr) override;
		bool Do(RemoveCardCommand aCommand, RemoveCardCommand::Result* outResult = nullptr) override;
		bool Do(CreateLocationCommand aCommand, CreateLocationCommand::Result* outResult = nullptr) override;
		bool Do(RemoveLocationCommand aCommand, RemoveLocationCommand::Result* outResult = nullptr) override;
		bool Do(CreatePlayerCommand aCommand, CreatePlayerCommand::Result* outResult = nullptr) override;
		bool Do(RemovePlayerCommand aCommand, RemovePlayerCommand::Result* outResult = nullptr) override;
		bool Do(StartCommand aCommand, StartCommand::Result* outResult = nullptr) override;
		bool Do(EndTurnCommand aCommand, EndTurnCommand::Result* outResult = nullptr) override;
		bool Do(HealPlayerCommand aCommand, HealPlayerCommand::Result* outResult = nullptr) override;
		bool Do(DamagePlayerCommand aCommand, DamagePlayerCommand::Result* outResult = nullptr) override;
		bool Do(AddPlayerResourcesCommand aCommand, AddPlayerResourcesCommand::Result* outResult = nullptr) override;
		bool Do(RemovePlayerResourcesCommand aCommand, RemovePlayerResourcesCommand::Result* outResult = nullptr) override;
		bool Do(SetNameCommand aCommand, SetNameCommand::Result* outResult = nullptr) override;

		bool Do(SetSeedCommand aCommand, SetSeedCommand::Result* outResult = nullptr) override;
		bool Do(GenerateUniformRandomIntegerCommand aCommand, GenerateUniformRandomIntegerCommand::Result* outResult = nullptr) override;

		std::mt19937 myRNG;

		CardID myNextCardID;
		LocationID myNextLocationID;
		PlayerID myNextPlayerID;

		CommonUtilities::EventManager myEventManager;

		TurnIndex myTurnIndex;

		std::map<PlayerID, std::unique_ptr<Player>> myPlayers;
		std::map<LocationID, std::unique_ptr<Location>> myLocations;
		std::map<CardID, std::unique_ptr<Card>> myCards;

		std::queue<Task*> myTaskQueue;
		std::vector<Command> myCommandStack;
		CardFactory myCardFactory;
	};
}