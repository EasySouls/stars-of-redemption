#include "Renderer.h"

#include <stdexcept>

namespace StarsOfRedemption
{
	void Renderer::Init()
	{
		throw std::logic_error("Not implemented");
	}

	void Renderer::Shutdown()
	{
		throw std::logic_error("Not implemented");
	}

	void Renderer::Render()
	{
		throw std::logic_error("Not implemented");
	}

	void Renderer::InitPipeline()
	{
		constexpr VkPipelineLayoutCreateInfo pipelineLayoutInfo{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
		VK_CHECK(vkCreatePipelineLayout(sd, &pipelineLayoutInfo, nullptr, &m_PipelineLayout));
	}
}
