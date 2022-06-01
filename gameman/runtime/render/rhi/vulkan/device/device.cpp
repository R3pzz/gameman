#include "device.hpp"

namespace render::impl {
	enum struct e_gpu_fail : std::uint32_t {
		api_ver = 0u,
		type,
		families,
		extensions,
		success
	};

	device& device::connect_gpu( const gpu_req_t& req, const VkSurfaceKHR surf ) {
		m_surface = surf;

		auto is_suitable = [ ] ( const VkPhysicalDevice dev,
			const gpu_req_t& req, const auto surf,
			std::vector< const char* >& exts, queue_desc_t& qdesc
		) -> e_gpu_fail {
			std::printf( "is_suitable: started!\n" );

			auto properties = VkPhysicalDeviceProperties{};

			if ( dev == VK_NULL_HANDLE )
				throw std::runtime_error{ "is_suitable: dev was null handle" };

			vkGetPhysicalDeviceProperties( dev, &properties );
			
			std::printf( "is_suitable: device properties obtained!\n" );

			if ( properties.apiVersion < req.m_min_api_ver )
				return e_gpu_fail::api_ver;

			std::printf( "is_suitable: api check passed!\n" );
			
			if ( !req.m_allow_any_type )
				if ( properties.deviceType != -req.m_type )
					return e_gpu_fail::type;

			std::printf( "is_suitable: properties part passed!\n" );

			// --- queue family criteria --- //

			auto family_count = std::uint32_t{};
			auto family_properties = std::vector< VkQueueFamilyProperties >{};
			
			vkGetPhysicalDeviceQueueFamilyProperties( dev,
				&family_count, nullptr
			);

			family_properties.resize( family_count );

			vkGetPhysicalDeviceQueueFamilyProperties( dev,
				&family_count, family_properties.data( )
			);

			queue_desc_t desc{};

			for ( std::size_t it{};
				it < family_properties.size( ); it++
			) {
				auto& prop = family_properties.at( it );

				if ( prop.queueFlags & VK_QUEUE_GRAPHICS_BIT )
					desc.m_graphics_queue = it;
				
				if ( prop.queueFlags & VK_QUEUE_COMPUTE_BIT )
					desc.m_compute_queue = it;

				if ( prop.queueFlags & VK_QUEUE_TRANSFER_BIT )
					desc.m_transfer_queue = it;

				auto present = VkBool32{};

				vkGetPhysicalDeviceSurfaceSupportKHR( dev, it, surf, &present );

				if ( present )
					desc.m_present_queue = it;
			}

			if ( req.m_require_graphics )
				if ( desc.m_graphics_queue == -1 )
					return e_gpu_fail::families;
			
			if ( req.m_require_present )
				if ( desc.m_present_queue == -1 )
					return e_gpu_fail::families;
			
			if ( req.m_require_transfer )
				if ( desc.m_transfer_queue == -1 )
					return e_gpu_fail::families;

			if ( req.m_require_compute )
				if ( desc.m_compute_queue == -1 )
					return e_gpu_fail::families;

			std::printf( "is_suitable: families part passed!\n" );

			// --- extension criteria --- //

			auto ext_count = std::uint32_t{};
			auto ext_properties = std::vector< VkExtensionProperties >{};

			vkEnumerateDeviceExtensionProperties( dev,
				nullptr, &ext_count, nullptr
			);

			ext_properties.resize( ext_count );

			std::printf( "is_suitable: ext_properties size: %lo\n", ext_properties.size( ) );

			vkEnumerateDeviceExtensionProperties( dev,
				nullptr, &ext_count, ext_properties.data( )
			);

			auto ext_names = std::vector< const char* >{};

			for ( auto& name : req.m_required_exts ) {
				auto flag = false;

				for ( auto& prop : ext_properties ) {
					std::printf( "is_suitable: extension count: %o, name: %s\n", ext_count, prop.extensionName );

					if ( strcmp( prop.extensionName, name ) == 0 )
						flag = true;
				}

				if ( !flag )
					return e_gpu_fail::extensions;
			}

			exts = req.m_required_exts;

			std::printf( "is_suitable: extensions part passed!\n" );

			qdesc = desc;

			return e_gpu_fail::success;
		};

		auto gpu_count = std::uint32_t{};
		auto gpu_avail = std::vector< VkPhysicalDevice >{};

		vkEnumeratePhysicalDevices( m_instance,
			&gpu_count, nullptr
		);

		if ( !gpu_count )
			throw std::runtime_error{ "no devices are present at the machine!" };

		gpu_avail.resize( gpu_count );
		
		std::printf( "gpu_avail size before enumeration: %lo\n", gpu_avail.size( ) );

		vkEnumeratePhysicalDevices( m_instance,
			&gpu_count, gpu_avail.data( )
		);

		std::printf( "gpu_avail size after enumeration: %lo\n", gpu_avail.size( ) );

		for ( std::size_t it{}; it < gpu_count; it++ ) {
			std::printf( "started gpu is_suitable for loop\n" );

			auto& gpu = gpu_avail.at( it );

			if ( !gpu )
				continue;

			auto desc = queue_desc_t{};
			auto res = is_suitable( gpu, req, surf, m_extensions, desc );

			std::printf( "it: %lo, is_suitable: %o\n", it, static_cast< std::uint32_t >( res ) );

			if ( res == e_gpu_fail::success ) {
				m_gpu = gpu;

				m_families.emplace( e_family::graphics, desc.m_graphics_queue );
				m_families.emplace( e_family::present, desc.m_present_queue );
				m_families.emplace( e_family::transfer, desc.m_transfer_queue );
				m_families.emplace( e_family::compute, desc.m_compute_queue );

				vkGetPhysicalDeviceFeatures( gpu, &m_features );

				return *this;
			}
		}

		throw std::runtime_error{ "unable to find suitable gpu!" };
		
		return *this;
	}

	device& device::build_device( ) {
		if ( !m_gpu )
			throw std::runtime_error{ "please, run connect_gpu before build_device!" };

		auto priority = 1.f;
		auto queue_infos = std::vector< VkDeviceQueueCreateInfo >{};

		for ( auto& pair : m_families ) {
			// discard queue creation if we don't need one
			if ( pair.second == -1 )
				continue;

			auto queue_info = VkDeviceQueueCreateInfo{};
			queue_info.pQueuePriorities = &priority;
			queue_info.queueCount = 1u;
			queue_info.queueFamilyIndex = pair.second;
			queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

			queue_infos.push_back( queue_info );
		}

		auto features = VkPhysicalDeviceFeatures{};

		features.depthClamp = k_support_depth_clamp;

		for ( auto& name : m_extensions )
			std::printf( "m_extensions has: %s\n", name );

		VkDeviceCreateInfo dev_info{};
		dev_info.enabledExtensionCount = m_extensions.size( );
		dev_info.ppEnabledExtensionNames = m_extensions.data( );
		dev_info.pEnabledFeatures = &features;
		dev_info.queueCreateInfoCount = queue_infos.size( );
		dev_info.pQueueCreateInfos = queue_infos.data( );
		dev_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		if ( auto res = vkCreateDevice( m_gpu,
			&dev_info, nullptr, &m_device
		); res != VK_SUCCESS )
			throw std::runtime_error{ std::to_string( res ) };

		for ( auto& pair : m_families ) {
			auto queue = VkQueue{};

			vkGetDeviceQueue( m_device, pair.second, 0u, &queue );

			m_queues.emplace( pair.first, queue );
		}

		return *this;
	}
}