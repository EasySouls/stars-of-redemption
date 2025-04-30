#pragma once
#include "Walnut/Layer.h"


namespace StarsOfRedemption
{
	class ClientLayer final : public Walnut::Layer
	{
	public:
		ClientLayer();
		void OnUIRender() override;
		void OnUpdate(float deltaTime) override;
	};
}
