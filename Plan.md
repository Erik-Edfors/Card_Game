# User Experience

## Joining a game
The following is a description of the wished user experience of joining a game.

The user launches the game, entering the main menu. Inside there are 4 options. Host Game, Join Game, Settings and Exit.

In this example the user clicks Join Game and enters the server select screen. In the server select screen, the user is greeted by a list of saved servers and the option to add a new server. The status of the server should be listed underneath the server name, and description. f.ex 'In Lobby', 'In Game', 'Not Reponding'.

Clicking join on a server with status 'In Lobby' either connects the user to the server, or opens a password screen followed before connecting. Then the lobby screen is opened.

In the lobby, the user can enter a name, and maybe color. Other players do the same, and now it is easy to distinguish players apart.

When the game starts, the user is moved into the game screen. The bottom half of the screen is entirely dedicated to showing the user his hand and board of play.

The upper half of the screen is dedicated to showing the other players boards.

## Hosting a game
The following is a description of the wished user experience of hosting a game.

The user launches the server, entering the setup screen.

The server loads the previous name, description and password from a saved file. The user can change these freely.

The user then presses host, and is taken to the server lobby screen.

The user launches the game and goes through the steps of **Joining a game** until they enter server select. Here there is the server under local servers.

The user joins their own server, along with their friends who want to join the game.

The user goes back to the server and presses 'Start Game'.

The game starts, and the user can play the same as any other game.

If a player missbehaves or the host has a power trip, they can at any moment enter the server and kick or ban a player. Banning a player saves their IP address in a blacklist. Only the server has access to the address of players. This information is NEVER shared between clients.

# Game Rules

## Starting
* Cards are registered in a registry, which is passed to the CardFactory for creating cards during the game.
* Players are initialized.
* Each Client is assigned a PlayerID.
* Clients send their deck list to the server, which populates their decks with the given cards.
* Server draws each player 5 randomized cards from their decks.

## Turn Order
Every player has a TurnIndex, which the order of is randomised once at the start of the game.

During a turn, only the player whose turn it is should be able to influence the servers game stateobject. (May change during development)

At the start of a turn, the player whose turn it is, draws 1 card.

## Playing cards
By hovering over a card in their hand, and left clicking the mouse, a player can play a card after paying any associated costs.

Playing a card means moving it to the play area, face up. Then any effects dictated by the card happens, followed after by any effect that triggers from the card entering play.

## Life
Some cards can cause a change of life to a player. When a player reaches 0 or lower life. They lose the game.

## Cards
Cards can have a cost, which must be payed to play the card.

Cards can have triggers.
 
Triggers call one or more effects.

All triggers are locked when triggered until all effects and triggers are played out. This means that a card cannot have a trigger that is in some remove way triggered again by its own effect in an infinite loop.

<!-- [Old Plan] -->
<!-- # Plan

## Player & Hand
The player and hand will have a manager that creates, moves, plays and sends commands to server.

It will be able to undo actions if the server rejects them.

## Cards
Cards are a collection of data which describes effects and triggers for those effects along with effects that are constant.

A Card can be played from the hand zone.

## Card Zones
A card can exist in different zones during gameplay.
* Deck
    + Cannot be seen by any player.
    + All cards start in the deck.
    + Players draw cards from the deck.
    + Costs
        - Expensive triggers.
        - Expensive play.
* Hand
    + Can only be seen by owned player.
    + Only cards in hand can be played normaly.
    + Costs
        - Expensive triggers.
        - Normal play.
* Play
    + Can be seen by all players.
    + Costs
        - Normal triggers.
* Graveyard
    + Can be seen by all players.
    + Costs
        - Expensive triggers.
        - Expensive play.
* Void
    + Can be seen by all players.
    + Costs
        - Extortionate play.
        - Extortionate triggers.

## Triggers
* Health loss
* Health gain
* Beginning of turn.
* End of turn. 
* Card enters play.
* Card leaves play.

## Conditions
A card trigger can have conditions to make it more customizable.
* Play.
    + when played by a player.
* More than.
    + for triggers with associated amounts.
* Less than.
    + for trigger with associated amounts.
* Player.
    + Only triggers caused by the player.
* Opponent.
    + Only triggers caused by an opponent.
* Other.
    + Only other cards.
* Permanents.
    + Only Permanent.
* In Play.
    + Only when the card is in play.
* In Deck.
    + Only when the card is in the deck.
* In Graveyard.
    + Only when the card is in the graveyard.
* In Hand.
    + Only when the card is in the hand.

## Effects
Effects are an action a card takes upon a specified trigger and conditions. An effect can be any one time action.

* Win The Game.
    + Wins the game.
* Lose The Game.
    + Loses the game.
* Destroy.
    + Sends a card in Play to the Graveyard.
* Annihilate.
    + Sends a card to Void.
* Damage.
    + Damages a target.
* Heal.
    + Heals a target.
* Draw.
    + Draws a number of cards.
* Discard.
    + Discards a card from Hand.
    + Player chooses.
* Discard Random.
    + Distards a card from Hand.
    + Randomly choosen.
* Buff.
    + To be determined.
* Debuff.
    + To be determined.

## Gameplay
### Game Start
* Two (maybe more players) draw a hand of 5 cards.
* All players health is set to 10.
    + May be changed later.
* A coin toss determines who starts.
    + Some small benefit to the player going second might be given.

### Phases
#### Begin
* Refresh resources.
    + Resources to be determined.
* Draw a card.
    + Only if not first.
* Beginning of Turn trigger.

#### Main
* Play Cards.

#### End
* End of Turn trigger.


## Coding
In order to implement all features alot of preperatory work is needed.

### Assets
All games need assets.

#### AssetManager
The Assetmanager should be initialized with a directory in which it will look for assets. It will log invalid assets or duplicate names.

When it has initialized, the user should be able to load an asset by giving its name and type. It should log any inconsistensies like wrong asset type or missing assets.

When loading an asset for the first time, the assetmanager should load said asset using a user defined function. It should then store that asset in memory until such a moment that the user no longer has use of the asset.

How the assetmanager should determine if an asset is needed or not can be decided on later. For now, it can load all assets when initialized and store them in memory.

If an asset is updated during runtime, the AssetManager should NOT update the asset without explicit order to do so. This is to prevent an asset updating during gameplay causing unintended issues.

#### Game Elements
Elements in the game, such as the player, hand, input manager and such can either be implemented through the component system, or through object oriented implementation inside specialized scenes. Where to add certain elements can be confusing.

Cards will almost certainly be implemented through the component system as they have graphics, can move, cause effects and other things that are best implemented with components.

Zones are probably best if they are game objects as well, as they have positions and will need to be accessed by the cards in order to produce certain effects.

Players are an odd one. They will need to manage zones and resources. They do not need to manage client networking or player actions (commands) but they are closely connected as every action a client can make in a match is made through the player.

Client manages connection and communications as well as actions/commands. A user makes an action, the Client creates a command, sends it to the server for verification and plays the command. It should also undo commands when asked by the server.

Commands are EVERYTHING a user can do to interact with the game except for CLIENT ONLY actions, such as menus. Commands should be applied to a scene so that it is easy to access by a client in a known location. How commands interact with game objects is to be determined. But most likely networked objects will have IDs that identify them, probably given by the server. -->