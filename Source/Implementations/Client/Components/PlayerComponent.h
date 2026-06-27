#pragma once
#include <Core/Engine/ComponentSystem/Components/Component.h>

namespace client
{
	class HandZoneComponent;
	class FieldZoneComponent;
	class GraveyardZoneComponent;

	class PlayerComponent : public Component
	{
	public:
		void Initialize(HandZoneComponent* aHand, FieldZoneComponent* aField, GraveyardZoneComponent* aGraveyard);

	private:
		ComponentRef<HandZoneComponent> myHand;
		ComponentRef<FieldZoneComponent> myField;
		ComponentRef<GraveyardZoneComponent> myGraveyard;
	};
}