#pragma once
#include "Walnut/Application.h"
#include "Walnut/Layer.h"
#include "Walnut/Networking/Client.h"
#include "glm/glm.hpp"


namespace StarsOfRedemption
{
	class ClientLayer final : public Walnut::Layer
	{
	public:
		ClientLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUIRender() override;

		void OnUpdate(float deltaTime) override;

	private:
		void OnDataReceived(const Walnut::Buffer& buffer);
		void OnServerConnected();
		void OnServerDisconnected();

	private:
		glm::vec2 m_PlayerPosition{ 100, 100 };
		glm::vec2 m_PlayerVelocity{ 0, 0 };

		Walnut::Client m_Client;
		std::string m_ServerAddress;
	};
}
