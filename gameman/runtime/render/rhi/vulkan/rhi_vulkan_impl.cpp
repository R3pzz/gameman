#include "rhi_vulkan_impl.hpp"
#include <framework/window.hpp>

namespace render::impl {
	void rhi_vulkan_impl::create( ) {
		{
			instance_req_t req{};
			req.m_app_name = "";
			req.m_app_ver = VK_VERSION_1_0;
			req.m_engine_name = "gameman";
			req.m_engine_ver = VK_VERSION_1_0;
			req.m_min_api_ver = VK_API_VERSION_1_0;

			m_instance.create( req );
		}

		{
			gpu_req_t req{};
			req.m_allow_any_type = true;
			req.m_require_graphics = true;
			req.m_require_present = true;
			req.m_required_exts.push_back( "VK_KHR_swapchain" );

			m_device.adopt_instance( m_instance.get( ) );
			
			auto& window = framework::window::instance( );
			auto surface = VkSurfaceKHR{};

			glfwCreateWindowSurface( m_instance.get( ),
				window, nullptr, &surface
			);

			m_device.connect_gpu( req, surface ).build_device( );
		}

		{
			swapchain_req_t req{};
			req.m_clipped = true;
			req.m_color_space = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
			req.m_color_space_fb = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
			req.m_format = VK_FORMAT_B8G8R8A8_SRGB;
			req.m_format_fb = VK_FORMAT_B8G8R8A8_SRGB;
			req.m_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
			req.m_present_mode_fb = VK_PRESENT_MODE_FIFO_KHR;
			req.m_use_max_extent = true;

			m_swapchain.adopt_device( &m_device ).create( req );
		}
	}

	void rhi_vulkan_impl::shutdown( ) {

	}

	rhi* rhi_vulkan_impl::instance( ) { return s_rhi_vulkan.get( ); }

	V_INLINE static const auto s_rhi_vulkan = std::make_unique< rhi_vulkan_impl >( );
}