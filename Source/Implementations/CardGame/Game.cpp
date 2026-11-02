#include "Game.h"
#include <ranges>
#include "Task.h"

namespace cardgame
{
	Game::Game() :
		myNextCardID(INVALID_CARD_ID + 1),
		myNextLocationID(INVALID_LOCATION_ID + 1),
		myNextPlayerID(INVALID_PLAYER_ID + 1),
		myTurnPlayer(nullptr),
		myTurnIndex(0)
	{
	}
	Game::~Game()
	{
	}

	bool Game::Initialize(size_t aPlayerCount, const CardFactory& aCardFactory)
	{
		myCardFactory = aCardFactory;

		for (size_t i = 0; i < aPlayerCount; i++)
		{
			const PlayerID playerID = CreatePlayerID();
			const LocationID handID = CreateLocationID();
			const LocationID deckID = CreateLocationID();
			const LocationID playID = CreateLocationID();
			const LocationID discardID = CreateLocationID();
			const TurnIndex turnIndex = static_cast<TurnIndex>(i);

			myLocations[handID] = std::make_unique<Location>(handID);
			myLocations[deckID] = std::make_unique<Location>(deckID);
			myLocations[playID] = std::make_unique<Location>(playID);
			myLocations[discardID] = std::make_unique<Location>(discardID);
			myPlayers[playerID] = std::make_unique<Player>(playerID, handID, deckID, playID, discardID, turnIndex);
		}

		return true;
	}
	bool Game::Shutdown()
	{
		bool succeeded = true;

		return succeeded;
	}

	bool Game::PushCommand(const Command& aCommand)
	{
		bool result = false;

		switch (aCommand.Type)
		{
		case COMMAND_SUBMIT_PLAYERS: result = Do(aCommand.Command.SubmitPlayers); break;
		case COMMAND_SUBMIT_CARDS: result = Do(aCommand.Command.SubmitCards); break;
		case COMMAND_SUBMIT_LOCATIONS: result = Do(aCommand.Command.SubmitLocations); break;
		case COMMAND_SUBMIT_AMOUNT: result = Do(aCommand.Command.SubmitAmount); break;
		case COMMAND_SUBMIT_OPTION: result = Do(aCommand.Command.SubmitOption); break;

		case COMMAND_PLAY: result = Do(aCommand.Command.Play); break;
		case COMMAND_MOVE_CARD: result = Do(aCommand.Command.MoveCard); break;
		case COMMAND_CREATE_CARD: result = Do(aCommand.Command.CreateCard); break;
		case COMMAND_END_TURN: result = Do(aCommand.Command.EndTurn); break;
		case COMMAND_START: result = Do(aCommand.Command.Start); break;
		case COMMAND_HEAL_PLAYER: result = Do(aCommand.Command.HealPlayer); break;
		case COMMAND_DAMAGE_PLAYER: result = Do(aCommand.Command.DamagePlayer); break;
		case COMMAND_ADD_PLAYER_RESOURCES: result = Do(aCommand.Command.AddPlayerResources); break;
		case COMMAND_REMOVE_PLAYER_RESOURCES: result = Do(aCommand.Command.RemovePlayerResources); break;
		case COMMAND_SET_NAME: result = Do(aCommand.Command.SetName); break;

		default:
			//TODO: Log invalid command type.
			return false;
		}

		if (result)
		{
			myCommandStack.push_back(aCommand);
			Update();
		}

		return result;
	}

	bool Game::HasTasks() const noexcept
	{
		return not myTaskQueue.empty();
	}

	const Task* Game::GetTask() const noexcept
	{
		if (not HasTasks())
		{
			return nullptr;
		}
		return myTaskQueue.front();
	}

	const Player* Game::GetPlayer(PlayerID aID) const noexcept
	{
		auto it = myPlayers.find(aID);
		if (it == myPlayers.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	const Card* Game::GetCard(CardID aID) const noexcept
	{
		auto it = myCards.find(aID);
		if (it == myCards.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	const Location* Game::GetLocation(LocationID aID) const noexcept
	{
		auto it = myLocations.find(aID);
		if (it == myLocations.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	size_t Game::GetNumPlayers() const noexcept
	{
		return myPlayers.size();
	}

	size_t Game::GetPlayers(const Player** outPlayers, size_t aNumPlayers) const noexcept
	{
		auto it = myPlayers.begin();
		for (size_t i = 0; i < aNumPlayers; i++)
		{
			if (it == myPlayers.end())
			{
				return i;
			}

			const Player* player = it->second.get();

			outPlayers[i] = player;

			it = std::next(it);
		}

		return aNumPlayers;
	}

	size_t Game::GetNumLocations() const noexcept
	{
		return myLocations.size();
	}

	size_t Game::GetLocations(const Location** outLocations, size_t aNumLocations) const noexcept
	{
		auto it = myLocations.begin();
		for (size_t i = 0; i < aNumLocations; i++)
		{
			if (it == myLocations.end())
			{
				return i;
			}

			const Location* location = it->second.get();

			outLocations[i] = location;

			it = std::next(it);
		}

		return aNumLocations;
	}

	TurnIndex Game::GetTurnIndex() const noexcept
	{
		return myTurnIndex;
	}

	const Player* Game::GetTurnPlayer() const noexcept
	{
		return myTurnPlayer;
	}

	size_t Game::GetNumCommands() const noexcept
	{
		return myCommandStack.size();
	}

	size_t Game::GetCommands(Command* outCommands, size_t aStart, size_t aNumCommands) const noexcept
	{
		if (aStart >= myCommandStack.size())
		{
			return 0;
		}

		const size_t copyAmount = std::min(aNumCommands, aStart - myCommandStack.size());

		const auto begin = myCommandStack.begin() + aStart;
		const auto end = begin + copyAmount;
		std::copy(begin, end, outCommands);
		return copyAmount;
	}

	CardFactory& Game::GetCardFactory() noexcept
	{
		return myCardFactory;
	}

	CommonUtilities::EventPusher& Game::GetEventPusher() noexcept
	{
		return myEventManager;
	}

	unsigned int Game::GetGamestateHash() const noexcept
	{
		return CommonUtilities::TStringHash<Command>(myCommandStack.data(), myCommandStack.size());
	}

	Player* Game::FindPlayer(PlayerID aID) noexcept
	{
		auto it = myPlayers.find(aID);
		if (it == myPlayers.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	Card* Game::FindCard(CardID aID) noexcept
	{
		auto it = myCards.find(aID);
		if (it == myCards.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	Location* Game::FindLocation(LocationID aID) noexcept
	{
		auto it = myLocations.find(aID);
		if (it == myLocations.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	void Game::Update()
	{
		bool anyAction = true;
		size_t loopCount = 0;
		std::vector<CardID> cards;

		while (anyAction)
		{
			myEventManager.DispatchEvents();

			{
				cards.clear();
				auto keys = std::views::keys(myCards);
				cards.assign(keys.begin(), keys.end());
			}

			anyAction = false;
			for (CardID id : cards)
			{
				auto cardIt = myCards.find(id);
				if (cardIt == myCards.end())
				{
					continue;
				}

				Card& card = *cardIt->second;

				anyAction |= card.Update();
			}
			++loopCount;

			if (loopCount > 1024)
			{
				//TODO: Log probably infinite loop!
				break;
			}
		}

		for (const std::unique_ptr<Card>& card : std::views::values(myCards))
		{
			card->ResetLocks();
		}
	}

	CardID Game::CreateCardID() noexcept
	{
		return myNextCardID++;
	}
	LocationID Game::CreateLocationID() noexcept
	{
		return myNextLocationID++;
	}
	PlayerID Game::CreatePlayerID() noexcept
	{
		return myNextPlayerID++;
	}

	bool Game::PushTask(Task* aTask)
	{
		myTaskQueue.push(aTask);

		return true;
	}

	bool Game::Do(SubmitPlayersCommand aCommand)
	{
		if (myTaskQueue.empty())
		{
			return false;
		}
		if (myTaskQueue.front()->GetType() != TASK_TARGET_PLAYERS)
		{
			return false;
		}

		TargetPlayersTask* task = static_cast<TargetPlayersTask*>(myTaskQueue.front());

		if (task->Submit(aCommand.Players, aCommand.Count))
		{
			myTaskQueue.pop();

			EventTaskSubmitted taskSubmitted = {
			};
			myEventManager.Push(EVENT_TASK_SUBMITTED, taskSubmitted);

			return true;
		}

		return false;
	}

	bool Game::Do(SubmitCardsCommand aCommand)
	{
		if (myTaskQueue.empty())
		{
			return false;
		}
		if (myTaskQueue.front()->GetType() != TASK_TARGET_CARDS)
		{
			return false;
		}
		
		TargetCardsTask* task = static_cast<TargetCardsTask*>(myTaskQueue.front());

		if (task->Submit(aCommand.Cards, aCommand.Count))
		{
			myTaskQueue.pop();
			return true;
		}

		return false;
	}

	bool Game::Do(SubmitLocationsCommand aCommand)
	{
		if (myTaskQueue.empty())
		{
			return false;
		}
		if (myTaskQueue.front()->GetType() != TASK_TARGET_LOCATIONS)
		{
			return false;
		}

		TargetLocationsTask* task = static_cast<TargetLocationsTask*>(myTaskQueue.front());

		if (task->Submit(aCommand.Locations, aCommand.Count))
		{
			myTaskQueue.pop();
			return true;
		}

		return false;
	}

	bool Game::Do(SubmitAmountCommand aCommand)
	{
		if (myTaskQueue.empty())
		{
			return false;
		}
		if (myTaskQueue.front()->GetType() != TASK_AMOUNT)
		{
			return false;
		}

		AmountTask* task = static_cast<AmountTask*>(myTaskQueue.front());

		if (task->Submit(aCommand.Amount))
		{
			myTaskQueue.pop();
			return true;
		}

		return false;
	}

	bool Game::Do(SubmitOptionCommand aCommand)
	{
		if (myTaskQueue.empty())
		{
			return false;
		}
		if (myTaskQueue.front()->GetType() != TASK_OPTION)
		{
			return false;
		}

		OptionTask* task = static_cast<OptionTask*>(myTaskQueue.front());

		if (task->Submit(aCommand.Index))
		{
			myTaskQueue.pop();
			return true;
		}

		return false;
	}

	bool Game::Do(PlayCommand aCommand)
	{
		Card* card;
		{
			auto it = myCards.find(aCommand.Card);
			if (it == myCards.end())
			{
				return false;
			}
			card = it->second.get();
		}

		if (not card->CanBePlayed())
		{
			// Card cannot be played currently.
			return false;
		}

		Player* player;
		{
			auto it = myPlayers.find(aCommand.Player);
			if (it == myPlayers.end())
			{
				return false;
			}
			player = it->second.get();
		}

		const ResourceCount cost = card->GetPlayCost();
		if (not aCommand.IgnoreCost && cost > player->GetResourceCount())
		{
			// Player cannot pay the cost to play the card.
			return false;
		}

		ResourceCount payed = 0;

		if (not aCommand.IgnoreCost)
		{
			payed = cost;
			const RemovePlayerResourcesCommand rmPlResources = {
				.Source = card->GetID(),
				.Target = player->GetID(),
				.Amount = payed
			};
			if (not Do(rmPlResources))
			{
				return false;
			}
		}

		const MoveCardCommand move = {
			.Card = card->GetID(),
			.Location = player->GetPlayLocation()
		};
		if (not Do(move))
		{
			return false;
		}

		myEventManager.Push(
			EVENT_CARD_PLAYED,
			EventCardPlayed{
				.Card = aCommand.Card,
				.Owner = aCommand.Player,
				.Cost = cost,
				.Payed = payed
			}
		);

		return true;
	}
	bool Game::Do(MoveCardCommand aCommand)
	{
		Card* card = FindCard(aCommand.Card);
		if (card == nullptr)
		{
			return false;
		}

		Location* const from = FindLocation(card->GetLocation());
		Location* const to = FindLocation(aCommand.Location);

		if (from == nullptr || to == nullptr)
		{
			return false;
		}

		card->SetLocation(to->GetID());
		from->RemoveCard(card->GetID());
		to->AddCard(card->GetID());

		myEventManager.Push(
			EVENT_CARD_MOVED,
			EventCardMoved{
				.Card = aCommand.Card,
				.Owner = card->GetOwner(),
				.From = from->GetID(),
				.To = to->GetID()
			}
		);

		return true;
	}
	bool Game::Do(CreateCardCommand aCommand)
	{
		//TODO: implement.
		aCommand;

		const CardID id = CreateCardID();
		Player* const owner = FindPlayer(aCommand.Owner);
		Location* const location = FindLocation(aCommand.Location);

		if (owner == nullptr || location == nullptr)
		{
			return false;
		}

		{
			std::unique_ptr<Card> card = myCardFactory.CreateCard(aCommand.FactoryID, this, id);
			if (card == nullptr)
			{
				return false;
			}

			card->SetOwner(owner->GetID());
			card->SetLocation(location->GetID());
			location->AddCard(card->GetID());
			myCards[id] = std::move(card);
		}

		myEventManager.Push(
			EVENT_CARD_CREATED,
			EventCardCreated{
				.Card = id,
				.Owner = owner->GetID(),
				.Location = location->GetID()
			}
		);

		return true;
	}
	bool Game::Do(StartCommand aCommand)
	{
		aCommand;

		myTurnIndex = 0;
		myTurnPlayer = nullptr;
		for (const std::unique_ptr<Player>& player : std::views::values(myPlayers))
		{
			if (GetTurnIndex() == player->GetTurnIndex())
			{
				myTurnPlayer = player.get();
				break;
			}
		}

		myEventManager.Push(
			EVENT_TURN_BEGIN,
			EventTurnBegin{
				.Owner = GetTurnPlayer()->GetID()
			}
		);

		return true;
	}
	bool Game::Do(EndTurnCommand aCommand)
	{
		aCommand;

		myEventManager.Push(
			EVENT_TURN_END,
			EventTurnEnd{
				.Owner = GetTurnPlayer()->GetID()
			}
		);

		myTurnIndex = (myTurnIndex + 1) % static_cast<TurnIndex>(GetNumPlayers());

		myTurnPlayer = nullptr;
		for (const std::unique_ptr<Player>& player : std::views::values(myPlayers))
		{
			if (GetTurnIndex() == player->GetTurnIndex())
			{
				myTurnPlayer = player.get();
				break;
			}
		}

		myEventManager.Push(
			EVENT_TURN_BEGIN,
			EventTurnBegin{
				.Owner = GetTurnPlayer()->GetID()
			}
		);

		return true;
	}
	bool Game::Do(HealPlayerCommand aCommand)
	{
		if (aCommand.Amount < 0)
		{
			return false;
		}

		Player* target = FindPlayer(aCommand.Target);
		if (target == nullptr)
		{
			return false;
		}

		target->SetHealth(target->GetHealth() + aCommand.Amount);

		myEventManager.Push(
			EVENT_PLAYER_HEALED,
			EventPlayerDamaged{
				.Source = aCommand.Source,
				.Target = aCommand.Target,
				.Amount = aCommand.Amount
			}
		);

		myEventManager.Push(
			EVENT_PLAYER_HEALTH_CHANGED,
			EventPlayerHealthChanged{
				.Source = aCommand.Source,
				.Target = aCommand.Target,
				.Amount = aCommand.Amount
			}
		);

		return true;
	}
	bool Game::Do(DamagePlayerCommand aCommand)
	{
		if (aCommand.Amount < 0)
		{
			return false;
		}

		Player* target = FindPlayer(aCommand.Target);
		if (target == nullptr)
		{
			return false;
		}

		target->SetHealth(target->GetHealth() - aCommand.Amount);

		myEventManager.Push(
			EVENT_PLAYER_DAMAGED,
			EventPlayerDamaged{
				.Source = aCommand.Source,
				.Target = aCommand.Target,
				.Amount = aCommand.Amount
			}
		);

		myEventManager.Push(
			EVENT_PLAYER_HEALTH_CHANGED,
			EventPlayerHealthChanged{
				.Source = aCommand.Source,
				.Target = aCommand.Target,
				.Amount = -aCommand.Amount
			}
		);

		return true;
	}
	bool Game::Do(AddPlayerResourcesCommand aCommand)
	{
		if (aCommand.Amount < 0)
		{
			return false;
		}

		Player* target = FindPlayer(aCommand.Target);
		if (target == nullptr)
		{
			return false;
		}

		target->SetResourceCount(target->GetResourceCount() + aCommand.Amount);

		myEventManager.Push(
			EVENT_PLAYER_GAINED_RESOURCES,
			EventPlayerGainedResources{
				.Source = aCommand.Source,
				.Target = aCommand.Target,
				.Amount = aCommand.Amount
			}
		);

		myEventManager.Push(
			EVENT_PLAYER_RESOURCES_CHANGED,
			EventPlayerResourcesChanged{
				.Source = aCommand.Source,
				.Target = aCommand.Target,
				.Amount = aCommand.Amount
			}
		);

		return true;
	}
	bool Game::Do(RemovePlayerResourcesCommand aCommand)
	{
		if (aCommand.Amount < 0)
		{
			return false;
		}

		Player* target = FindPlayer(aCommand.Target);
		if (target == nullptr)
		{
			return false;
		}

		target->SetResourceCount(target->GetResourceCount() - aCommand.Amount);

		myEventManager.Push(
			EVENT_PLAYER_LOST_RESOURCES,
			EventPlayerLostResources{
				.Source = aCommand.Source,
				.Target = aCommand.Target,
				.Amount = aCommand.Amount
			}
		);

		myEventManager.Push(
			EVENT_PLAYER_RESOURCES_CHANGED,
			EventPlayerResourcesChanged{
				.Source = aCommand.Source,
				.Target = aCommand.Target,
				.Amount = -aCommand.Amount
			}
		);

		return true;
	}
	bool Game::Do(SetNameCommand aCommand)
	{
		Player* player = FindPlayer(aCommand.Player);
		if (player == nullptr)
		{
			return false;
		}

		player->SetName(aCommand.Name);

		return true;
	}
}