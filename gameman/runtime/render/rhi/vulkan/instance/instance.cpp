#include "instance.hpp"

namespace render::impl {
	instance& instance::create( const instance_req_t& req ) {
		VkApplicationInfo app_info{};
		app_info.apiVersion = req.m_min_api_ver;
		app_info.applicationVersion = req.m_app_ver;
		app_info.engineVersion = req.m_engine_ver;
		app_info.pApplicationName = req.m_app_name.c_str( );
		app_info.pEngineName = req.m_engine_name.c_str( );
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

		auto ext_count = std::uint32_t{};
		auto ext_names = glfwGetRequiredInstanceExtensions( &ext_count );

		auto layer_count = std::uint32_t{};
    auto layer_props = std::vector< VkLayerProperties >{};

		{
    	vkEnumerateInstanceLayerProperties( &layer_count, nullptr );

			layer_props.reserve( layer_count );

    	vkEnumerateInstanceLayerProperties( &layer_count, layer_props.data( ) );

			for ( auto& layer : m_layers ) {
				auto found = bool{};

				for ( auto& prop : layer_props )
					if ( !strcmp( layer, prop.layerName ) )
						found = true;

				if ( !found ) {
					auto name = std::string{};

					name += "instance -> create: layer ";
					name += layer;
					name += " not found!";

					throw std::runtime_error{ name };
				}
			}
		}

		VkInstanceCreateInfo instance_info{};
		instance_info.enabledExtensionCount = ext_count;
		instance_info.ppEnabledExtensionNames = ext_names;
		instance_info.enabledLayerCount = m_layers.size( );
		instance_info.ppEnabledLayerNames = m_layers.data( );
		instance_info.pApplicationInfo = &app_info;
		instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		
		if ( vkCreateInstance(
			&instance_info, nullptr, &m_handle 
		) != VK_SUCCESS )
			throw std::runtime_error{ "unable to create vulkan instance" };

		return *this;
	}

	instance& instance::destroy( ) {
		vkDestroyInstance( m_handle, nullptr );

		return *this;
	}
}