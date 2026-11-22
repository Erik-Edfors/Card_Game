#pragma once
#include <Implementations/CardGame/Card.h>
#include "../Tasks/SacrificeTask.h"

namespace shared
{
	class SacrificeCard : public cardgame::Card
	{
	public:
		SacrificeCard();
		virtual ~SacrificeCard();

		virtual cardgame::ResourceCount GetPlayCost() const override;
		virtual bool CanBePlayed() const override;
		virtual const char* GetName() const override;
		virtual const char* GetDescription() const override;

	protected:
		virtual void OnInit() override;
		virtual bool OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData) override;
		virtual void OnDestroyed() override;

	private:
		cardgame::CardID GetRandomCard(cardgame::LocationID aLocation);

		SacrificeTask mySacrificeTask;
	};
}