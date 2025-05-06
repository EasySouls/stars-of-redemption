#pragma once

#include "Vulkan.h"
#include "glm/glm.hpp"

#include <filesystem>

namespace StarsOfRedemption
{
	struct Vertex
	{
		glm::vec2 position;
		glm::vec3 color;
	};

	class Renderer
	{
	public:
		void Init();
		void Shutdown();

		void Render();

	private:
		void InitPipeline();

		static VkShaderModule LoadShader(const std::filesystem::path& path, VkShaderStageFlagBits stage);

	private:
		VkFormat m_ColorAttachmentsFormat = VK_FORMAT_B8G8R8A8_UNORM;
		VkPipelineLayout m_PipelineLayout = nullptr;
		VkPipeline m_GraphicsPipeline = nullptr;
	};
}


