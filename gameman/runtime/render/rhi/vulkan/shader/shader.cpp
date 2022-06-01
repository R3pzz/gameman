#include "shader.hpp"
#include "../device/device.hpp"
#include <utility/file/file.hpp>

namespace render::impl {
	auto& shader::create( const std::filesystem::path path ) {
		auto code = utility::file_reader{}.read( path ).buffer( );

		VkShaderModuleCreateInfo module_info{};
		module_info.codeSize = code.size( );
		module_info.pCode = reinterpret_cast< std::uint32_t* >( code.data( ) );
		module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		if ( vkCreateShaderModule( m_device->device_handle( ),
			&module_info, nullptr, &m_shader
		) != VK_SUCCESS )
			throw std::runtime_error{ "unable to create shader module!" };

		VkPipelineShaderStageCreateInfo stage_info{};
		stage_info.module = m_shader;
		stage_info.pName = m_name.c_str( );
		stage_info.stage = static_cast< VkShaderStageFlagBits >( m_type );
		stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

		return *this;
	}

	auto& shader::refresh( ) { return create( m_working_dir ); }

	void shader::shutdown( ) { vkDestroyShaderModule( m_device->device_handle( ), m_shader, nullptr ); }
	
	auto& shader::use( const VkShaderStageFlagBits type ) {

		return *this;
	}
}