#include "ClientLayer.h"

#include "imgui.h"

namespace StarsOfRedemption
{
	ClientLayer::ClientLayer()
	= default;

	void ClientLayer::OnUIRender()
	{
		ImGui::Begin("Client Layer");
		ImGui::Text("Hello from Client Layer!");
		ImGui::End();
	}
	void ClientLayer::OnUpdate(float deltaTime)
	{}
}
