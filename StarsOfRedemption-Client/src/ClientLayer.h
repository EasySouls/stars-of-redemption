#pragma once

#include "Renderer/Renderer.h"
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
		void OnRender() override;

		void OnUpdate(float deltaTime) override;

	private:
		void OnDataReceived(const Walnut::Buffer& buffer);
		void OnServerConnected();
		void OnServerDisconnected();

	private:
		Renderer m_Renderer;

		glm::vec2 m_PlayerPosition{ 100, 100 };
		glm::vec2 m_PlayerVelocity{ 0, 0 };

		Walnut::Client m_Client;
		uint32_t m_ClientId = 0;

		std::string m_ServerAddress;

		struct PlayerData
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
		};

		std::mutex m_PlayerDataMutex;
		std::map<uint32_t, PlayerData> m_PlayerData;
	};
}
