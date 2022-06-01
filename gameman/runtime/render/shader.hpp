#pragma once
#include <config.hpp>

namespace render {
	class shader {
	public:
		// create a shader with all presets
		V_VIRTUAL void create( const std::filesystem::path path ) V_VIRTUAL_POST;

		// refresh shader code if it changed
		V_VIRTUAL void refresh( ) V_VIRTUAL_POST;

		// unallocate shader module and stuff
		V_VIRTUAL void shutdown( ) V_VIRTUAL_POST;

		// use in a pipeline
		V_VIRTUAL void use( const VkShaderStageFlagBits type ) V_VIRTUAL_POST;
	};
}