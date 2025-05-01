#include "ClientLayer.h"
#include "Walnut/Input/Input.h"
#include "Walnut/ImGui/ImGuiTheme.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

namespace StarsOfRedemption
{
	static void DrawRect(const glm::vec2 position, const glm::vec2 size, const uint32_t color)
	{
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		const ImVec2 min = ImGui::GetWindowPos() + ImVec2(position.x, position.y);
		const ImVec2 max = min + ImVec2(size.x, size.y);

		drawList->AddRectFilled(min, max, color);
	}

	void ClientLayer::OnAttach()
	{
		m_Client.SetDataReceivedCallback([this](const Walnut::Buffer buffer) { OnDataReceived(buffer); });
		m_Client.SetServerConnectedCallback([this]() { OnServerConnected(); });
		m_Client.SetServerDisconnectedCallback([this]() { OnServerDisconnected(); });
	}

	void ClientLayer::OnDetach()
	{

	}

	void ClientLayer::OnUpdate(const float deltaTime)
	{
		if (m_Client.GetConnectionStatus() != Walnut::Client::ConnectionStatus::Connected)
			return;

		glm::vec2 dir{ 0.0f, 0.0f };

		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W))
			dir.y = 1.0f;
		else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S))
			dir.y = -1.0f;

		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A))
			dir.x = -1.0f;
		else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D))
			dir.x = 1.0f;

		if (glm::length(dir) > 0.0f)
		{
			dir = glm::normalize(dir);
			constexpr float speed = 40.0f;
			m_PlayerVelocity = dir * speed;
		}

		m_PlayerVelocity = glm::mix(m_PlayerVelocity, glm::vec2(0.0f), 5.0f * deltaTime);

		m_PlayerPosition += m_PlayerVelocity * deltaTime;
		std::cout << "Player Position: (" << m_PlayerPosition.x << ", " << m_PlayerPosition.y << ")\n";
	}

	void ClientLayer::OnUIRender()
	{

		if (m_Client.GetConnectionStatus() == Walnut::Client::ConnectionStatus::Connected)
		{
			ImGui::Begin("Client");
			ImGui::Text("Connected to server");
			ImGui::End();

			DrawRect(m_PlayerPosition, { 50.0f, 50.0f }, 0xffffff);
		}
		else
		{
			ImGui::Begin("Connect to server");

			ImGui::InputText("Server Address", &m_ServerAddress);
			if (ImGui::Button("Connect"))
			{
				m_Client.ConnectToServer(m_ServerAddress);
			}

			if (m_Client.GetConnectionStatus() == Walnut::Client::ConnectionStatus::Connecting)
			{
				ImGui::TextColored(ImColor(Walnut::UI::Colors::Theme::textDarker), "Connecting to server...");
			}
			else if (m_Client.GetConnectionStatus() == Walnut::Client::ConnectionStatus::FailedToConnect)
			{
				ImGui::TextColored(ImColor(Walnut::UI::Colors::Theme::error),"Failed to connect to server");
			}
			else if (m_Client.GetConnectionStatus() == Walnut::Client::ConnectionStatus::Disconnected)
			{
				ImGui::TextColored(ImColor(Walnut::UI::Colors::Theme::error), "Disconnected");
			}

			ImGui::End();
		}
	}

	void ClientLayer::OnDataReceived(const Walnut::Buffer& buffer)
	{
		// Handle incoming data from the server
		// This is where you would parse the data and update the game state accordingly
		std::cout << "Data received from server: " << buffer << '\n';
	}

	void ClientLayer::OnServerConnected()
	{
		std::cout << "Connected to server\n";
	}

	void ClientLayer::OnServerDisconnected()
	{
		std::cout << "Disconnected from server\n";
		m_PlayerPosition = { 100, 100 };
		m_PlayerVelocity = { 0, 0 };
	}
}
