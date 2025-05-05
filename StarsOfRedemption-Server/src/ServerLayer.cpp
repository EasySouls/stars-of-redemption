#include "ServerLayer.h"

#include "ServerPacket.h"
#include "Walnut/Core/Buffer.h"
#include "Walnut/Serialization/BufferStream.h"

namespace StarsOfRedemption
{
	static Walnut::Buffer s_MessageBuffer;

	void ServerLayer::OnAttach()
	{
		s_MessageBuffer.Allocate(10 * 1024 * 1024);

		m_Console.SetMessageSendCallback([this](const std::string_view message) { OnConsoleMessage(message); });

		m_Server.SetDataReceivedCallback([this](const Walnut::ClientInfo& clientInfo, const Walnut::Buffer buffer) { OnDataReceived(clientInfo, buffer); });
		m_Server.SetClientConnectedCallback([this](const Walnut::ClientInfo& clientInfo) { OnClientConnected(clientInfo); });
		m_Server.SetClientDisconnectedCallback([this](const Walnut::ClientInfo& clientInfo) { OnClientDisconnected(clientInfo); });

		m_Server.Start();
	}

	void ServerLayer::OnDetach()
	{
		m_Server.Stop();
	}

	void ServerLayer::OnUpdate(const float ts)
	{
		Walnut::BufferStreamWriter writer(s_MessageBuffer);
		writer.WriteRaw(PacketType::ClientUpdate);

		m_PlayerDataMutex.lock();
		{
			writer.WriteMap(m_PlayerData);
		}
		m_PlayerDataMutex.unlock();

		m_Server.SendBufferToAllClients(writer.GetBuffer());
	}

	void ServerLayer::OnUIRender()
	{
		Layer::OnUIRender();
	}

	void ServerLayer::OnConsoleMessage(const std::string_view message)
	{
		if (message.starts_with("/"))
		{
			std::cout << "Console command: " << message << "\n";
		}
	}

	void ServerLayer::OnClientConnected(const Walnut::ClientInfo& clientInfo)
	{
		m_Console.AddMessage("Client connected: " + std::to_string(clientInfo.ID));

		Walnut::BufferStreamWriter writer(s_MessageBuffer);
		writer.WriteRaw(PacketType::ClientConnect);
		writer.WriteRaw(clientInfo.ID);

		m_Server.SendBufferToClient(clientInfo.ID, writer.GetBuffer());
	}

	void ServerLayer::OnClientDisconnected(const Walnut::ClientInfo& clientInfo)
	{
		m_Console.AddMessage("Client disconnected: " + std::to_string(clientInfo.ID));

		Walnut::BufferStreamWriter writer(s_MessageBuffer);
		writer.WriteRaw(PacketType::ClientDisconnect);
		writer.WriteRaw(clientInfo.ID);
	}

	void ServerLayer::OnDataReceived(const Walnut::ClientInfo& clientInfo, const Walnut::Buffer buffer)
	{
		Walnut::BufferStreamReader reader(buffer);

		PacketType packetType;
		reader.ReadRaw(packetType);
		switch (packetType)
		{
		case PacketType::None:
		case PacketType::ClientConnectionRequest:
		case PacketType::ConnectionStatus:
		case PacketType::ClientList:
		case PacketType::ClientConnect:
			break;
		case PacketType::ClientUpdate:
			{

			m_PlayerDataMutex.lock();
			{
				auto& [Position, Velocity] = m_PlayerData[clientInfo.ID];
				reader.ReadRaw(Position);
				reader.ReadRaw(Velocity);
				m_Console.AddMessage("{}, {} - {}, {}", Position.x, Position.y, Velocity.x, Velocity.y);
			}
			m_PlayerDataMutex.unlock();

			break;
			}
		case PacketType::Message:
		{
			std::string message;
			reader.ReadRaw(message);
			m_Console.AddMessage("Client " + std::to_string(clientInfo.ID) + ": " + message);
			Walnut::BufferStreamWriter writer(s_MessageBuffer);
			writer.WriteRaw(PacketType::Message);
			writer.WriteRaw(clientInfo.ID);
			writer.WriteRaw(message);
			m_Server.SendBufferToAllClients(writer.GetBuffer());
			break;
		}
		case PacketType::ClientDisconnect:
		case PacketType::ClientUpdateResponse:
		case PacketType::MessageHistory:
		case PacketType::ServerShutdown:
		case PacketType::ClientKick:
			break;
		}
	}
}
