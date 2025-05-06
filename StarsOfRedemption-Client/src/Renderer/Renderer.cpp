#include "Renderer.h"

#include "Walnut/Application.h"

#include <stdexcept>
#include <array>
#include <vector>
#include <fstream>

namespace StarsOfRedemption
{
	void Renderer::Init()
	{
		InitPipeline();
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
		VkShaderModule vertShader = LoadShader("assets/shaders/basic.vert.spirv", VK_SHADER_STAGE_VERTEX_BIT);
		VkShaderModule fragShader = LoadShader("assets/shaders/basic.frag.spirv", VK_SHADER_STAGE_FRAGMENT_BIT);

		VkDevice device = GetVulkanBackendData()->Device;
		//Walnut::Application::GetMainWindowData()->Surface;

		constexpr VkPipelineLayoutCreateInfo pipelineLayoutInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
		VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout));

		// Define the vertex input binding description
		VkVertexInputBindingDescription bindingDescription{
			.binding = 0,
			.stride = sizeof(Vertex),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX };

		// Define the vertex input attribute descriptions
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = { {
			{.location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT, .offset = offsetof(Vertex, position)},        // position
			{.location = 1, .binding = 0, .format = VK_FORMAT_R32G32B32_SFLOAT, .offset = offsetof(Vertex, color)}         // color
		} };

		// Create the vertex input state
		VkPipelineVertexInputStateCreateInfo vertexInput{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
														  .vertexBindingDescriptionCount = 1,
														  .pVertexBindingDescriptions = &bindingDescription,
														  .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
														  .pVertexAttributeDescriptions = attributeDescriptions.data() };

		// Specify we will use triangle lists to draw geometry.
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE };

		// Specify rasterization state.
		VkPipelineRasterizationStateCreateInfo raster{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.depthBiasEnable = VK_FALSE,
			.lineWidth = 1.0f };

		// Specify that these states will be dynamic, i.e. not part of pipeline state object.
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
			VK_DYNAMIC_STATE_CULL_MODE,
			VK_DYNAMIC_STATE_FRONT_FACE,
			VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY
		};

		// Our attachment will write to all color channels, but no blending is enabled.
		VkPipelineColorBlendAttachmentState blendAttachment{
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT };

		VkPipelineColorBlendStateCreateInfo blend{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.attachmentCount = 1,
			.pAttachments = &blendAttachment };

		// We will have one viewport and scissor box.
		VkPipelineViewportStateCreateInfo viewport{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = 1,
			.scissorCount = 1 };

		// Disable all depth testing.
		VkPipelineDepthStencilStateCreateInfo depthStencil{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.depthCompareOp = VK_COMPARE_OP_ALWAYS };

		// No multisampling.
		VkPipelineMultisampleStateCreateInfo multisample{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT };

		VkPipelineDynamicStateCreateInfo dynamic_state_info{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
			.pDynamicStates = dynamicStates.data() };

		// Load our SPIR-V shaders.
		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = { {
			{.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			 .stage = VK_SHADER_STAGE_VERTEX_BIT,
			 .module = vertShader,
			 .pName = "main"},        // Vertex shader stage
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
				.module = fragShader,
				.pName = "main"}        // Fragment shader stage
		} };

		// Pipeline rendering info (for dynamic rendering).
		VkPipelineRenderingCreateInfo pipelineRenderingInfo{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
			.colorAttachmentCount = 1,
			.pColorAttachmentFormats = &m_ColorAttachmentsFormat };

		// Create the graphics pipeline.
		VkGraphicsPipelineCreateInfo pipe{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &pipelineRenderingInfo,
			.stageCount = static_cast<uint32_t>(shaderStages.size()),
			.pStages = shaderStages.data(),
			.pVertexInputState = &vertexInput,
			.pInputAssemblyState = &inputAssembly,
			.pViewportState = &viewport,
			.pRasterizationState = &raster,
			.pMultisampleState = &multisample,
			.pDepthStencilState = &depthStencil,
			.pColorBlendState = &blend,
			.pDynamicState = &dynamic_state_info,
			.layout = m_PipelineLayout,        // We need to specify the pipeline layout description up front as well.
			.renderPass = VK_NULL_HANDLE,                 // Since we are using dynamic rendering this will set as null
			.subpass = 0,
		};

		VK_CHECK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipe, nullptr, &m_GraphicsPipeline));

		// Pipeline is baked, we can delete the shader modules now.
		vkDestroyShaderModule(device, shaderStages[0].module, nullptr);
		vkDestroyShaderModule(device, shaderStages[1].module, nullptr);
	}

	VkShaderModule Renderer::LoadShader(const std::filesystem::path& path, VkShaderStageFlagBits stage)
	{
		std::ifstream stream(path, std::ios::ate | std::ios::binary);

		if (!stream.is_open())
		{
			throw std::runtime_error("Failed to open shader file: " + path.string());
		}

		stream.seekg(std::ios_base::end);
		const std::streamsize fileSize = stream.tellg();
		stream.seekg(std::ios_base::beg);

		std::vector<char> buffer(fileSize);

		stream.read(buffer.data(), fileSize);

		stream.close();

		const VkShaderModuleCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = buffer.size(),
			.pCode = reinterpret_cast<uint32_t*>(buffer.data())
		};

		const VkDevice device = GetVulkanBackendData()->Device;
		VkShaderModule shaderModule = nullptr;
		VK_CHECK(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule));

		return shaderModule;
	}
}
