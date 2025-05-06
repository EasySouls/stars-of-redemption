#pragma once

#include "Vulkan.h"

namespace StarsOfRedemption
{
	class Renderer
	{
	public:
		void Init();
		void Shutdown();

		void Render();

	private:
		void InitPipeline();

	private:
		VkPipelineLayout m_PipelineLayout = nullptr;
		VkPipeline m_GraphicsPipeline = nullptr;
	};
}


