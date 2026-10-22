#pragma once
#include <Implementations/CardGame/Card.h>

namespace shared
{
	class DamageCard : public cardgame::Card
	{
	public:
		DamageCard();
		virtual ~DamageCard();

		virtual cardgame::ResourceCount GetPlayCost() const override;
		virtual bool CanBePlayed() const override;
		virtual const char* GetName() const override;
		virtual const char* GetDescription() const override;

	protected:
		virtual void OnInit() override;
		virtual bool OnEvent(CommonUtilities::StringHash aEvent, const std::any& aData) override;
	};
}