# Plan

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

Commands are EVERYTHING a user can do to interact with the game except for CLIENT ONLY actions, such as menus. Commands should be applied to a scene so that it is easy to access by a client in a known location. How commands interact with game objects is to be determined. But most likely networked objects will have IDs that identify them, probably given by the server.