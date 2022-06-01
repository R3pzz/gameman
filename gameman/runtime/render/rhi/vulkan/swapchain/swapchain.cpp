#include "swapchain.hpp"
#include "../device/device.hpp"
#include <framework/window.hpp>
#include <math/clamp.hpp>

namespace render::impl {
	swapchain& swapchain::create( const swapchain_req_t& req ) {
		if ( !m_device )
			throw std::runtime_error{ "couldn't create swapchain: no device adopted!" };

		// query details

		auto format_count = std::uint32_t{};
		auto formats = std::vector< VkSurfaceFormatKHR >{};

		vkGetPhysicalDeviceSurfaceFormatsKHR( m_device->gpu_handle( ), 
			m_device->surface( ), &format_count, nullptr
		);

		formats.resize( format_count );

		vkGetPhysicalDeviceSurfaceFormatsKHR( m_device->gpu_handle( ), 
			m_device->surface( ), &format_count, formats.data( )
		);

		{
			auto found = false,
						found2 = false;

			for ( auto& format : formats )
				if ( format.format == req.m_format
							&& format.colorSpace == req.m_color_space )
						found = true;
			
			if ( found ) {
				m_image_format = req.m_format;
				m_color_space = req.m_color_space;
			}
			else
				for ( auto& format : formats )
					if ( format.format == req.m_format_fb
								&& format.colorSpace == req.m_color_space_fb )
							found2 = true;
			
			if ( found2 ) {
				m_image_format = req.m_format_fb;
				m_color_space = req.m_color_space_fb;
			}
			else if ( !found )
				throw std::runtime_error{ "swapchain surface format / color space not supported!" };
		}

		std::printf( "format picking stage passed\n" );

		auto mode_count = std::uint32_t{};
		auto modes = std::vector< VkPresentModeKHR >{};

		vkGetPhysicalDeviceSurfacePresentModesKHR( m_device->gpu_handle( ),
			m_device->surface( ), &mode_count, nullptr
		);

		modes.resize( mode_count );

		vkGetPhysicalDeviceSurfacePresentModesKHR( m_device->gpu_handle( ),
			m_device->surface( ), &mode_count, modes.data( )
		);

		{
			auto found = false,
						found2 = false;

			for ( auto& mode : modes )
				if ( mode == req.m_present_mode )
					found = true;
			
			if ( found )
				m_present_mode = req.m_present_mode;
			else
				for ( auto& mode : modes )
					if ( mode == req.m_present_mode )
						found2 = true;
			
			if ( found2 )
				m_present_mode = req.m_present_mode_fb;
			else if ( !found )
				throw std::runtime_error{ "swapchain: present mode not supported!" };
		}

		std::printf( "swapchain mode picking passed!\n" );

		auto size = VkExtent2D{};

		if ( !req.m_use_max_extent )
			size = { req.m_extent.x, req.m_extent.y };
		else {
			int w{}, h{};

			glfwGetFramebufferSize( 
				framework::window::instance( ), &w, &h
			);

			size = { static_cast< std::uint32_t >( w ),
				static_cast< std::uint32_t >( h )
			};
		}

		std::printf( "swapchain size picking passed!\n" );

		VkSurfaceCapabilitiesKHR cpb{};

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_device->gpu_handle( ),
			m_device->surface( ), &cpb
		);

		std::printf( "swapchain capabilities obtained!\n" );

		auto count = math::clamp( cpb.minImageCount + 1u, cpb.minImageCount, cpb.maxImageCount );

		// creation
		VkSwapchainCreateInfoKHR info{};
		info.clipped = req.m_clipped;
		info.compositeAlpha = static_cast< VkCompositeAlphaFlagBitsKHR >( req.m_composite_alpha_bits );
		info.flags = req.m_swapchain_create_bits;
		info.imageArrayLayers = 1u;
		info.imageColorSpace = static_cast< VkColorSpaceKHR >( m_color_space );
		info.imageExtent = size;
		info.imageFormat = static_cast< VkFormat >( m_image_format );
		info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		info.minImageCount = count;
		info.preTransform = static_cast< VkSurfaceTransformFlagBitsKHR >( req.m_pretransform_bits );
		info.presentMode = static_cast< VkPresentModeKHR >( m_present_mode );
		info.surface = m_device->surface( );
		info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

		std::printf( "first info part filled!\n" );
		
		if ( m_device->fam_index( e_family::graphics )
			!= m_device->fam_index( e_family::present )
		) {
			auto indices = std::vector< std::uint32_t >{};

			indices.push_back( m_device->fam_index( e_family::graphics ) );
			indices.push_back( m_device->fam_index( e_family::present ) );

			info.queueFamilyIndexCount = 2u;
			info.pQueueFamilyIndices = indices.data( );
			info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		}
		else
			info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		std::printf( "second info part filled!\n" );

		if ( vkCreateSwapchainKHR( m_device->device_handle( ),
			&info, nullptr, &m_swapchain
		) != VK_SUCCESS )
			throw std::runtime_error{ "unable to create swapchain!" };

		std::printf( "swapchain created!\n" );
		
		return *this;
	}
	
	void swapchain::shutdown( const std::vector< VkImageView >& views ) {
		vkDestroySwapchainKHR( m_device->device_handle( ),
			m_swapchain, nullptr
		);

		for ( auto& view : views )
			vkDestroyImageView( m_device->device_handle( ), view, nullptr );
	}

	auto swapchain::images( ) const {
		auto count = std::uint32_t{};
		auto images = std::vector< VkImage >{};

		vkGetSwapchainImagesKHR( m_device->device_handle( ),
			m_swapchain, &count, nullptr
		);

		images.resize( count );

		vkGetSwapchainImagesKHR( m_device->device_handle( ),
			m_swapchain, &count, images.data( )
		);

		return images;
	}

	auto swapchain::views( ) const {
		auto swapchain_images = images( );
		auto swapchain_views = std::vector< VkImageView >{};

		swapchain_views.resize( swapchain_images.size( ) );

		for ( std::size_t it{};
			it < swapchain_images.size( ); it++
		) {
			VkImageViewCreateInfo info{};
			info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.format = static_cast< VkFormat >( m_image_format );
			info.image = swapchain_images.at( it );
			info.subresourceRange = { 1, 0u, 1u, 0u, 1u };
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

			if ( vkCreateImageView( m_device->device_handle( ),
				&info, nullptr, &swapchain_views.at( it )
			) != VK_SUCCESS )
				throw std::runtime_error{ "unable to create image view!" };
		}

		return swapchain_views;
	}
}