#pragma once

#include "vulkan/vulkan.h"

#include <iostream>
#include <string>

namespace vkb
{
	std::string to_string(VkResult result);

}

#define VK_CHECK(x) \
	{ \
		VkResult err = x; \
		if (err) \
		{ \
			fprintf(stderr, "Detected Vulkan error: %d\n", err); \
			throw std::runtime_error("Vulkan error: " + vkb::to_string(err)); \
		} \
	}