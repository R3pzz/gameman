#pragma once
#include <config.hpp>
#include <render/rhi.hpp>
#include "device/device.hpp"
#include "instance/instance.hpp"
#include "shader/shader.hpp"
#include "specs/specs.hpp"
#include "swapchain/swapchain.hpp"

/**
 * this is a basic design of vulkan context and renderer api.
 * there's a whole bunch of stuff to implement for easier
 * renderer operations and other useful stuff.
 * 
 * NOTE: This needs to be rewritten when we get into
 * more advanced stuff.
*/

namespace render::impl {
	class rhi_vulkan_impl
		: public render::rhi {
		V_INLINE rhi_vulkan_impl( const rhi_vulkan_impl& ) = delete;
		V_INLINE rhi_vulkan_impl& operator=( const rhi_vulkan_impl& ) = delete;

	private:
		instance		m_instance{};
		
		device			m_device{};

		swapchain		m_swapchain{};

	public:
		V_INLINE rhi_vulkan_impl( ) = default;
		V_INLINE ~rhi_vulkan_impl( );

		V_VIRTUAL void create( ) V_OVERRIDE;
		V_VIRTUAL void shutdown( ) V_OVERRIDE;

		static rhi* instance( );
	};
}

#include "rhi_vulkan_impl.inl"