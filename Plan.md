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
