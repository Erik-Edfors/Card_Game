#include "Game.h"
#include <ranges>
#include "Task.h"

namespace cardgame
{
	Game::Game() :
		myNextCardID(INVALID_CARD_ID + 1),
		myNextLocationID(INVALID_LOCATION_ID + 1),
		myNextPlayerID(INVALID_PLAYER_ID + 1),
		myTurnIndex(0)
	{
		myRNG.seed(0);
	}
	Game::~Game()
	{
	}

	bool Game::Initialize(const CardFactory& aCardFactory)
	{
		myCardFactory = aCardFactory;

		//for (size_t i = 0; i < aPlayerCount; i++)
		//{
		//	const PlayerID playerID = CreatePlayerID();
		//	const LocationID handID = CreateLocationID();
		//	const LocationID deckID = CreateLocationID();
		//	const LocationID playID = CreateLocationID();
		//	const LocationID discardID = CreateLocationID();
		//	const TurnIndex turnIndex = static_cast<TurnIndex>(i);

		//	myLocations[handID] = std::make_unique<Location>(handID);
		//	myLocations[deckID] = std::make_unique<Location>(deckID);
		//	myLocations[playID] = std::make_unique<Location>(playID);
		//	myLocations[discardID] = std::make_unique<Location>(discardID);
		//	myPlayers[playerID] = std::make_unique<Player>(playerID, handID, deckID, playID, discardID, turnIndex);
		//}

		return true;
	}
	bool Game::Shutdown()
	{
		bool succeeded = true;

		return succeeded;
	}

	bool Game::PushCommand(const Command& aCommand, Command::Result* outResult)
	{
		bool result = false;

		switch (aCommand.Type)
		{
		case COMMAND_SUBMIT_PLAYERS: result = Do(aCommand.Command.SubmitPlayers, outResult ? &outResult->SubmitPlayers : nullptr); break;
		case COMMAND_SUBMIT_CARDS: result = Do(aCommand.Command.SubmitCards, outResult ? &outResult->SubmitCards : nullptr); break;
		case COMMAND_SUBMIT_LOCATIONS: result = Do(aCommand.Command.SubmitLocations, outResult ? &outResult->SubmitLocations : nullptr); break;
		case COMMAND_SUBMIT_AMOUNT: result = Do(aCommand.Command.SubmitAmount, outResult ? &outResult->SubmitAmount : nullptr); break;
		case COMMAND_SUBMIT_OPTION: result = Do(aCommand.Command.SubmitOption, outResult ? &outResult->SubmitOption : nullptr); break;

		case COMMAND_PLAY: result = Do(aCommand.Command.Play, outResult ? &outResult->Play : nullptr); break;
		case COMMAND_MOVE_CARD: result = Do(aCommand.Command.MoveCard, outResult ? &outResult->MoveCard : nullptr); break;
		case COMMAND_CREATE_CARD: result = Do(aCommand.Command.CreateCard, outResult ? &outResult->CreateCard : nullptr); break;
		case COMMAND_REMOVE_CARD: result = Do(aCommand.Command.RemoveCard, outResult ? &outResult->RemoveCard : nullptr); break;
		case COMMAND_CREATE_LOCATION: result = Do(aCommand.Command.CreateLocation, outResult ? &outResult->CreateLocation : nullptr); break;
		case COMMAND_REMOVE_LOCATION: result = Do(aCommand.Command.RemoveLocation, outResult ? &outResult->RemoveLocation : nullptr); break;
		case COMMAND_CREATE_PLAYER: result = Do(aCommand.Command.CreatePlayer, outResult ? &outResult->CreatePlayer : nullptr); break;
		case COMMAND_REMOVE_PLAYER: result = Do(aCommand.Command.RemovePlayer, outResult ? &outResult->RemovePlayer : nullptr); break;
		case COMMAND_END_TURN: result = Do(aCommand.Command.EndTurn, outResult ? &outResult->EndTurn : nullptr); break;
		case COMMAND_START: result = Do(aCommand.Command.Start, outResult ? &outResult->Start : nullptr); break;
		case COMMAND_HEAL_PLAYER: result = Do(aCommand.Command.HealPlayer, outResult ? &outResult->HealPlayer : nullptr); break;
		case COMMAND_DAMAGE_PLAYER: result = Do(aCommand.Command.DamagePlayer, outResult ? &outResult->DamagePlayer : nullptr); break;
		case COMMAND_ADD_PLAYER_RESOURCES: result = Do(aCommand.Command.AddPlayerResources, outResult ? &outResult->AddPlayerResources : nullptr); break;
		case COMMAND_REMOVE_PLAYER_RESOURCES: result = Do(aCommand.Command.RemovePlayerResources, outResult ? &outResult->RemovePlayerResources : nullptr); break;
		case COMMAND_SET_NAME: result = Do(aCommand.Command.SetName, outResult ? &outResult->SetName : nullptr); break;
		case COMMAND_SET_SEED: result = Do(aCommand.Command.SetSeed, outResult ? &outResult->SetSeed : nullptr); break;

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
		for (const auto& entry : myPlayers)
		{
			const cardgame::Player* player = entry.second.get();
			if (player->GetTurnIndex() == GetTurnIndex())
			{
				return player;
			}
		}
		return nullptr;
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
		return CommonUtilities::TStringHash<std::mt19937>(&myRNG, 1).Hash ^ CommonUtilities::TStringHash<Command>(myCommandStack.data(), myCommandStack.size()).Hash;
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

			if (not anyAction)
			{
				anyAction |= RemoveDeadPlayers();
			}

			if (loopCount > 1024)
			{
				//TODO: Log probably infinite loop!
				if (not RemoveDeadPlayers()) // Continue looping for as long as players are dying.
				{
					break;
				}
			}
		}

		for (const std::unique_ptr<Card>& card : std::views::values(myCards))
		{
			card->ResetLocks();
		}
	}

	bool Game::RemoveDeadPlayers()
	{
		bool anyAction = false;

		std::vector<Player*> killedPlayers;
		for (const std::unique_ptr<Player>& player : std::views::values(myPlayers))
		{
			if (player->GetHealth() <= 0)
			{
				killedPlayers.push_back(player.get());
			}
		}

		while (not killedPlayers.empty())
		{
			size_t lowestHealthIndex = 0;
			for (size_t i = 0; i < killedPlayers.size(); i++)
			{
				Player* player = killedPlayers[i];
				if (player->GetHealth() < lowestHealthIndex)
				{
					lowestHealthIndex = i;
				}
			}

			Player* lowestHealthPlayer = killedPlayers[lowestHealthIndex];

			if (lowestHealthIndex + 1 != killedPlayers.size())
			{
				killedPlayers[lowestHealthIndex] = killedPlayers.back();
			}
			killedPlayers.pop_back();

			const RemovePlayerCommand removePlayer = {
				.Player = lowestHealthPlayer->GetID()
			};

			anyAction |= Do(removePlayer);
		}

		return anyAction;
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

	bool Game::RetractTask(Task* aTask)
	{
		std::queue<Task*> newQueue;
		size_t numRemoved = 0;

		while (not myTaskQueue.empty())
		{
			Task* task = myTaskQueue.front();
			if (task != aTask)
			{
				newQueue.push(task);
			}
			else
			{
				++numRemoved;
			}
		}

		myTaskQueue = std::move(newQueue);

		return numRemoved != 0;
	}

	bool Game::Do(SubmitPlayersCommand aCommand, SubmitPlayersCommand::Result* outResult)
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
		if (task->GetAuthority() != aCommand.Authority)
		{
			return false;
		}

		if (task->Submit(aCommand.Players, aCommand.Count))
		{
			EventTaskFinished taskSubmitted = {
				.Type = task->GetType()
			};
			myEventManager.Push(EVENT_TASK_FINISHED, taskSubmitted);

			myTaskQueue.pop();

			if (outResult != nullptr)
			{
				*outResult = SubmitPlayersCommand::Result{

				};
			}

			return true;
		}

		return false;
	}

	bool Game::Do(SubmitCardsCommand aCommand, SubmitCardsCommand::Result* outResult)
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
		if (task->GetAuthority() != aCommand.Authority)
		{
			return false;
		}

		if (task->Submit(aCommand.Cards, aCommand.Count))
		{
			EventTaskFinished taskSubmitted = {
				.Type = task->GetType()
			};
			myEventManager.Push(EVENT_TASK_FINISHED, taskSubmitted);

			myTaskQueue.pop();

			if (outResult != nullptr)
			{
				*outResult = SubmitCardsCommand::Result{

				};
			}
			return true;
		}

		return false;
	}

	bool Game::Do(SubmitLocationsCommand aCommand, SubmitLocationsCommand::Result* outResult)
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
		if (task->GetAuthority() != aCommand.Authority)
		{
			return false;
		}

		if (task->Submit(aCommand.Locations, aCommand.Count))
		{
			EventTaskFinished taskSubmitted = {
				.Type = task->GetType()
			};
			myEventManager.Push(EVENT_TASK_FINISHED, taskSubmitted);

			myTaskQueue.pop();

			if (outResult != nullptr)
			{
				*outResult = SubmitLocationsCommand::Result{

				};
			}
			return true;
		}

		return false;
	}

	bool Game::Do(SubmitAmountCommand aCommand, SubmitAmountCommand::Result* outResult)
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
		if (task->GetAuthority() != aCommand.Authority)
		{
			return false;
		}

		if (task->Submit(aCommand.Amount))
		{
			EventTaskFinished taskSubmitted = {
				.Type = task->GetType()
			};
			myEventManager.Push(EVENT_TASK_FINISHED, taskSubmitted);

			myTaskQueue.pop();

			if (outResult != nullptr)
			{
				*outResult = SubmitAmountCommand::Result{

				};
			}
			return true;
		}

		return false;
	}

	bool Game::Do(SubmitOptionCommand aCommand, SubmitOptionCommand::Result* outResult)
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
		if (task->GetAuthority() != aCommand.Authority)
		{
			return false;
		}

		if (task->Submit(aCommand.Index))
		{
			EventTaskFinished taskSubmitted = {
				.Type = task->GetType()
			};
			myEventManager.Push(EVENT_TASK_FINISHED, taskSubmitted);

			myTaskQueue.pop();

			if (outResult != nullptr)
			{
				*outResult = SubmitOptionCommand::Result{

				};
			}
			return true;
		}

		return false;
	}

	bool Game::Do(PlayCommand aCommand, PlayCommand::Result* outResult)
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
				.Cost = cost,
				.Payed = payed
			}
		);

		if (outResult != nullptr)
		{
			*outResult = PlayCommand::Result{
				.NewLocation = move.Location
			};
		}

		return true;
	}
	bool Game::Do(MoveCardCommand aCommand, MoveCardCommand::Result* outResult)
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
				.From = from->GetID(),
				.To = to->GetID()
			}
		);

		if (outResult != nullptr)
		{
			*outResult = MoveCardCommand::Result{

			};
		}

		return true;
	}
	bool Game::Do(CreateCardCommand aCommand, CreateCardCommand::Result* outResult)
	{
		const CardID id = CreateCardID();
		Player* const owner = FindPlayer(aCommand.Owner);
		Location* const location = FindLocation(aCommand.Location);

		if (owner == nullptr || location == nullptr)
		{
			return false;
		}

		{
			std::unique_ptr<Card> card = myCardFactory.CreateCard(aCommand.FactoryID);
			if (card == nullptr)
			{
				return false;
			}

			location->AddCard(id);
			card->Init(this, id, owner->GetID(), location->GetID());

			myCards[id] = std::move(card);
		}

		myEventManager.Push(
			EVENT_CARD_CREATED,
			EventCardCreated{
				.Card = id,
				.Location = location->GetID()
			}
		);

		if (outResult != nullptr)
		{
			*outResult = CreateCardCommand::Result{
				.Card = id
			};
		}

		return true;
	}
	bool Game::Do(RemoveCardCommand aCommand, RemoveCardCommand::Result* outResult)
	{
		auto it = myCards.find(aCommand.Card);

		if (it == myCards.end())
		{
			return false;
		}

		it->second->OnDestroyed();

		myCards.erase(it);

		myEventManager.Push(
			EVENT_CARD_REMOVED,
			EventCardRemoved{
				.Card = aCommand.Card
			}
		);

		if (outResult != nullptr)
		{
			*outResult = RemoveCardCommand::Result{

			};
		}

		return true;
	}
	bool Game::Do(CreateLocationCommand aCommand, CreateLocationCommand::Result* outResult)
	{
		aCommand;

		const LocationID id = CreateLocationID();
		myLocations[id] = std::make_unique<Location>(id);

		myEventManager.Push(
			EVENT_LOCATION_CREATED,
			EventLocationCreated{
				.Location = id
			}
		);

		if (outResult != nullptr)
		{
			*outResult = CreateLocationCommand::Result{
				.Location = id
			};
		}

		return false;
	}
	bool Game::Do(RemoveLocationCommand aCommand, RemoveLocationCommand::Result* outResult)
	{
		auto it = myLocations.find(aCommand.Location);

		if (it == myLocations.end())
		{
			return false;
		}

		size_t numRemovedCards = 0;

		if (aCommand.RemoveCards)
		{
			std::vector<CardID> cards;
			for (auto& entry : myCards)
			{
				CardID id = entry.first;
				std::unique_ptr<Card>& card = entry.second;

				if (card->GetLocation() == aCommand.Location)
				{
					cards.push_back(id);
				}
			}

			numRemovedCards = cards.size();

			for (CardID card : cards)
			{
				RemoveCardCommand command = {
					.Card = card
				};
				Do(command);
			}
		}
		else
		{
			for (auto& entry : myCards)
			{
				std::unique_ptr<Card>& card = entry.second;
				if (card->GetLocation() == aCommand.Location)
				{
					// Location contains cards.
					return false;
				}
			}
		}

		myLocations.erase(it);

		myEventManager.Push(
			EVENT_LOCATION_REMOVED,
			EventLocationRemoved{
				.Location = aCommand.Location
			}
		);

		if (outResult != nullptr)
		{
			*outResult = RemoveLocationCommand::Result{
				.NumRemovedCards = numRemovedCards
			};
		}

		return true;
	}
	bool Game::Do(CreatePlayerCommand aCommand, CreatePlayerCommand::Result* outResult)
	{
		aCommand;

		const PlayerID id = CreatePlayerID();

		CreateLocationCommand createLocationCommand = {
		};
		CreateLocationCommand::Result createLocationResult;

		Do(createLocationCommand, &createLocationResult);
		const LocationID hand = createLocationResult.Location;
		Do(createLocationCommand, &createLocationResult);
		const LocationID deck = createLocationResult.Location;
		Do(createLocationCommand, &createLocationResult);
		const LocationID play = createLocationResult.Location;
		Do(createLocationCommand, &createLocationResult);
		const LocationID discard = createLocationResult.Location;

		TurnIndex turnIndex = static_cast<TurnIndex>(GetNumPlayers());

		myPlayers[id] = std::make_unique<Player>(id, hand, deck, play, discard, turnIndex);

		myEventManager.Push(
			EVENT_PLAYER_CREATED,
			EventPlayerCreated{
				.Player = id
			}
		);

		if (outResult != nullptr)
		{
			*outResult = CreatePlayerCommand::Result{
				.Player = id
			};
		}

		return true;
	}
	bool Game::Do(RemovePlayerCommand aCommand, RemovePlayerCommand::Result* outResult)
	{
		auto it = myPlayers.find(aCommand.Player);

		if (it == myPlayers.end())
		{
			return false;
		}

		Player* player = it->second.get();

		size_t numRemovedCards = 0;

		{
			RemoveLocationCommand command = {
				.Location = player->GetHandLocation(),
				.RemoveCards = true
			};
			RemoveLocationCommand::Result result = {};
			Do(command, &result);
			numRemovedCards += result.NumRemovedCards;
		}
		{
			RemoveLocationCommand command = {
				.Location = player->GetDeckLocation(),
				.RemoveCards = true
			};
			RemoveLocationCommand::Result result = {};
			Do(command, &result);
			numRemovedCards += result.NumRemovedCards;
		}
		{
			RemoveLocationCommand command = {
				.Location = player->GetPlayLocation(),
				.RemoveCards = true
			};
			RemoveLocationCommand::Result result = {};
			Do(command, &result);
			numRemovedCards += result.NumRemovedCards;
		}
		{
			RemoveLocationCommand command = {
				.Location = player->GetDiscardLocation(),
				.RemoveCards = true
			};
			RemoveLocationCommand::Result result = {};
			Do(command, &result);
			numRemovedCards += result.NumRemovedCards;
		}

		const TurnIndex turnIndex = player->GetTurnIndex();
		myPlayers.erase(it);

		for (auto& entry : myPlayers)
		{
			Player* p = entry.second.get();
			const TurnIndex index = p->GetTurnIndex();
			if (index > turnIndex)
			{
				p->SetTurnIndex(index - 1);
			}
		}

		myEventManager.Push(
			EVENT_PLAYER_REMOVED,
			EventPlayerRemoved{
				.Player = aCommand.Player
			}
		);


		if (turnIndex == GetTurnIndex())
		{
			myEventManager.Push(
				EVENT_TURN_END,
				EventTurnEnd{
					.Owner = aCommand.Player
				}
			);

			const Player* turnPlayer = GetTurnPlayer();
			if (turnPlayer != nullptr)
			{
				myEventManager.Push(
					EVENT_TURN_BEGIN,
					EventTurnBegin{
						.Owner = GetTurnPlayer()->GetID()
					}
				);
			}
		}

		// Makes sure myTurnIndex is not invalid.
		if (GetNumPlayers() != 0)
		{
			myTurnIndex %= static_cast<TurnIndex>(GetNumPlayers());
		}
		else
		{
			myTurnIndex = 0;
		}

		if (outResult != nullptr)
		{
			*outResult = RemovePlayerCommand::Result{
				.NumRemovedCards = numRemovedCards
			};
		}

		return true;
	}
	bool Game::Do(StartCommand aCommand, StartCommand::Result* outResult)
	{
		aCommand;

		if (GetNumPlayers() == 0)
		{
			return false;
		}

		myTurnIndex = 0;

		const cardgame::Player* turnPlayer = GetTurnPlayer();

		if (turnPlayer == nullptr)
		{
			return false;
		}

		myEventManager.Push(
			EVENT_TURN_BEGIN,
			EventTurnBegin{
				.Owner = GetTurnPlayer()->GetID()
			}
		);

		if (outResult != nullptr)
		{
			*outResult = StartCommand::Result{

			};
		}

		return true;
	}
	bool Game::Do(EndTurnCommand aCommand, EndTurnCommand::Result* outResult)
	{
		aCommand;

		if (GetNumPlayers() == 0)
		{
			return false;
		}

		myEventManager.Push(
			EVENT_TURN_END,
			EventTurnEnd{
				.Owner = GetTurnPlayer()->GetID()
			}
		);

		myTurnIndex = (myTurnIndex + 1) % static_cast<TurnIndex>(GetNumPlayers());

		myEventManager.Push(
			EVENT_TURN_BEGIN,
			EventTurnBegin{
				.Owner = GetTurnPlayer()->GetID()
			}
		);

		if (outResult != nullptr)
		{
			*outResult = EndTurnCommand::Result{

			};
		}

		return true;
	}
	bool Game::Do(HealPlayerCommand aCommand, HealPlayerCommand::Result* outResult)
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

		if (outResult != nullptr)
		{
			*outResult = HealPlayerCommand::Result{

			};
		}

		return true;
	}
	bool Game::Do(DamagePlayerCommand aCommand, DamagePlayerCommand::Result* outResult)
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

		if (outResult != nullptr)
		{
			*outResult = DamagePlayerCommand::Result{
			};
		}

		return true;
	}
	bool Game::Do(AddPlayerResourcesCommand aCommand, AddPlayerResourcesCommand::Result* outResult)
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

		if (outResult != nullptr)
		{
			*outResult = AddPlayerResourcesCommand::Result{

			};
		}

		return true;
	}
	bool Game::Do(RemovePlayerResourcesCommand aCommand, RemovePlayerResourcesCommand::Result* outResult)
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

		if (outResult != nullptr)
		{
			*outResult = RemovePlayerResourcesCommand::Result{

			};
		}

		return true;
	}
	bool Game::Do(SetNameCommand aCommand, SetNameCommand::Result* outResult)
	{
		Player* player = FindPlayer(aCommand.Player);
		if (player == nullptr)
		{
			return false;
		}

		player->SetName(aCommand.Name);

		myEventManager.Push(
			EVENT_PLAYER_NAME_CHANGED,
			EventPlayerNameChanged{
				.Player = aCommand.Player
			}
		);

		if (outResult != nullptr)
		{
			*outResult = SetNameCommand::Result{

			};
		}

		return true;
	}
	bool Game::Do(SetSeedCommand aCommand, SetSeedCommand::Result* outResult)
	{
		myRNG.seed(aCommand.Seed);

		if (outResult != nullptr)
		{
			*outResult = SetSeedCommand::Result{

			};
		}

		return true;
	}
	bool Game::Do(GenerateUniformRandomIntegerCommand aCommand, GenerateUniformRandomIntegerCommand::Result* outResult)
	{
		int number = std::uniform_int_distribution<int>(aCommand.Min, aCommand.Max)(myRNG);

		if (outResult != nullptr)
		{
			*outResult = GenerateUniformRandomIntegerCommand::Result{
				.Number = number
			};
		}

		return true;
	}
}