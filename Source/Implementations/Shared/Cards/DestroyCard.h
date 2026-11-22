#pragma once
#include <Implementations/CardGame/Card.h>
#include "../Tasks/TargetPlayCardsTask.h"

namespace shared
{
	class DestroyCard : public cardgame::Card
	{
	public:
		DestroyCard();
		virtual ~DestroyCard();

		virtual cardgame::ResourceCount GetPlayCost() const override;
		virtual bool CanBePlayed() const override;
		virtual const char* GetName() const override;
		virtual const char* GetDescription() const override;

	protected:
		virtual void OnInit() override;
		virtual bool OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData) override;
		virtual void OnDestroyed() override;

	private:
		TargetPlayCardsTask myTargetsTask;
	};
}