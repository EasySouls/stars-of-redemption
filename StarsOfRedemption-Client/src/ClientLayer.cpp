#include "ClientLayer.h"
#include "Walnut/Input/Input.h"
#include "imgui.h"
#include "imgui_internal.h"

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

	}

	void ClientLayer::OnDetach()
	{

	}

	void ClientLayer::OnUpdate(const float deltaTime)
	{
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
		DrawRect(m_PlayerPosition, { 50.0f, 50.0f }, 0xffffff);
	}
}
