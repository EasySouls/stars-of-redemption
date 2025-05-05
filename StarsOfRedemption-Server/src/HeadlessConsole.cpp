#include "HeadlessConsole.h"

HeadlessConsole::HeadlessConsole(std::string_view title)
	: m_Title(title)
{
	m_InputThread = std::thread([this]() { InputThreadFunc(); });
}

HeadlessConsole::~HeadlessConsole()
{
	m_InputThreadRunning = false;
	if (m_InputThread.joinable())
		m_InputThread.join();
}

void HeadlessConsole::ClearLog()
{
	m_MessageHistory.clear();
}

void HeadlessConsole::SetMessageSendCallback(const MessageSendCallback& callback)
{
	m_MessageSendCallback = callback;
}

void HeadlessConsole::InputThreadFunc()
{
	m_InputThreadRunning = true;
	while (m_InputThreadRunning)
	{
		std::string line;
		std::getline(std::cin, line);
		m_MessageSendCallback(line);
	}

}