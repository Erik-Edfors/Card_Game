#pragma once
#include <string>
#include "../RefObject.hpp"
#include "../GraphicsDefs.h"

namespace graphics
{
	class GraphicsObject : public RefObject
	{
	public:
		GraphicsObject(GraphicsID aID);
		virtual ~GraphicsObject() = default;

		virtual void SetName(const char* aName) = 0;
		const std::string& GetName() const;

		GraphicsID GetID() const;
	protected:
		void SetLocalName(const char* aName);

	private:
		GraphicsID myID;
		std::string myName;
	};
}