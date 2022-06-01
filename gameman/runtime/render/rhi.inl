#include "rhi/vulkan/rhi_vulkan_impl.hpp"

namespace render {
	V_INLINE rhi* rhi::instance( ) {
		switch ( k_render_api ) {
			case e_render_api::vulkan: return impl::rhi_vulkan_impl::instance( );
			default: throw std::runtime_error{ "unknown rendering api!" };
		}
	}
}