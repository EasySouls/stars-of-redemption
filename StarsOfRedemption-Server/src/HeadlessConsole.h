#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <functional>
#include <iostream>

#include "spdlog/spdlog.h"

class HeadlessConsole
{
public:
	using MessageSendCallback = std::function<void(std::string_view)>;
public:
	HeadlessConsole(std::string_view title = "Walnut Console");
	~HeadlessConsole();

	void ClearLog();

	template<typename... Args>
	void AddMessage(const std::string_view format, Args&&... args)
	{
		std::string messageString = fmt::vformat(format, fmt::make_format_args(args...));
		std::cout << messageString << std::endl;
		m_MessageHistory.emplace_back(messageString);
	}

	template<typename... Args>
	void AddItalicMessage(const std::string_view format, Args&&... args)
	{
		const std::string messageString = fmt::vformat(format, fmt::make_format_args(args...));
		MessageInfo info = messageString;
		info.Italic = true;
		m_MessageHistory.push_back(info);
		std::cout << messageString << std::endl;
	}

	template<typename... Args>
	void AddTaggedMessage(const std::string_view tag, const std::string_view format, Args&&... args)
	{
		std::string messageString = fmt::vformat(format, fmt::make_format_args(args...));
		m_MessageHistory.emplace_back(std::string(tag), messageString);
		std::cout << '[' << tag << "] " << messageString << std::endl;
	}

	template<typename... Args>
	void AddMessageWithColor(uint32_t color, std::string_view format, Args&&... args)
	{
		std::string messageString = fmt::vformat(format, fmt::make_format_args(args...));
		m_MessageHistory.emplace_back(messageString, color);
		std::cout << messageString << std::endl;
	}

	template<typename... Args>
	void AddItalicMessageWithColor(const uint32_t color, const std::string_view format, Args&&... args)
	{
		const std::string messageString = fmt::vformat(format, fmt::make_format_args(args...));
		MessageInfo info(messageString, color);
		info.Italic = true;
		m_MessageHistory.push_back(info);
		std::cout << messageString << std::endl;
	}

	template<typename... Args>
	void AddTaggedMessageWithColor(uint32_t color, const std::string_view tag, const std::string_view format, Args&&... args)
	{
		std::string messageString = fmt::vformat(format, fmt::make_format_args(args...));
		m_MessageHistory.emplace_back(std::string(tag), messageString, color);
		std::cout << '[' << tag << "] " << messageString << std::endl;
	}

	void OnUIRender() {}

	void SetMessageSendCallback(const MessageSendCallback& callback);
private:
	void InputThreadFunc();
private:
	struct MessageInfo
	{
		std::string Tag;
		std::string Message;
		bool Italic = false;
		uint32_t Color = 0xffffffff;

		MessageInfo(std::string message, const uint32_t color = 0xffffffff)
			: Message(std::move(message)), Color(color)
		{
		}

		MessageInfo(std::string tag, std::string message, const uint32_t color = 0xffffffff)
			: Tag(std::move(tag)), Message(std::move(message)), Color(color)
		{
		}
	};

	std::string m_Title;
	std::vector<MessageInfo> m_MessageHistory;

	std::thread m_InputThread;
	bool m_InputThreadRunning = false;

	MessageSendCallback m_MessageSendCallback;

};