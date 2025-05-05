#include "ServerLayer.h"

#include "Walnut/Core/Buffer.h"

namespace StarsOfRedemption
{
	void ServerLayer::OnAttach()
	{
		m_Server.SetDataReceivedCallback([this](const Walnut::ClientInfo& clientInfo, const Walnut::Buffer buffer) { OnDataReceived(clientInfo, buffer); });
		m_Server.SetClientConnectedCallback([this](const Walnut::ClientInfo& clientInfo) { OnClientConnected(clientInfo); });
		m_Server.SetClientDisconnectedCallback([this](const Walnut::ClientInfo& clientInfo) { OnClientDisconnected(clientInfo); });
	}

	void ServerLayer::OnDetach()
	{
		Layer::OnDetach();
	}

	void ServerLayer::OnUpdate(float ts)
	{
		Layer::OnUpdate(ts);
	}

	void ServerLayer::OnUIRender()
	{
		Layer::OnUIRender();
	}

	void ServerLayer::OnConsoleMessage(std::string_view message)
	{
		throw std::logic_error("Not implemented");
	}

	void ServerLayer::OnClientConnected(const Walnut::ClientInfo& clientInfo)
	{
		throw std::logic_error("Not implemented");
	}

	void ServerLayer::OnClientDisconnected(const Walnut::ClientInfo& clientInfo)
	{
		throw std::logic_error("Not implemented");
	}

	void ServerLayer::OnDataReceived(const Walnut::ClientInfo& clientInfo, const Walnut::Buffer buffer)
	{
		throw std::logic_error("Not implemented");
	}
}
