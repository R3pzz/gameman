#pragma once
#include <config.hpp>

namespace render::impl {
	struct swapchain_req_t {
		int 	m_present_mode{},
					m_present_mode_fb{};
		
		int 	m_format{},
					m_format_fb{};

		int 	m_color_space{},
					m_color_space_fb{};

		glm::uvec2 m_extent{};

		bool 	m_use_max_extent{},
					m_can_be_recreated{};

		bool 	m_clipped{};

		int		m_composite_alpha_bits{},
					m_swapchain_create_bits{},
					m_pretransform_bits{},
					m_image_user_flags{},
					m_format_feature_flags{};
	};

	class device;

	class swapchain {
		V_INLINE swapchain( const swapchain& ) = delete;
		V_INLINE swapchain& operator=( const swapchain& ) = delete;
	
	private:
		const device*	m_device{};

		VkSwapchainKHR m_swapchain{};

		int 				m_image_count{},
								m_image_format{},
								m_color_space{},
								m_present_mode{};

		glm::uvec2 	m_extent{};

		bool 				m_use_max_extent{},
								m_can_be_recreated{};

	public:
		V_INLINE swapchain( ) = default;
		V_INLINE swapchain( const device* dev )
			: m_device{ dev } {}

		swapchain& create( const swapchain_req_t& req );
		void shutdown( const std::vector< VkImageView >& views );

		V_INLINE swapchain& adopt_device( const device* dev ) V_NOEXCEPT;

		V_INLINE const auto& swapchain_handle( ) const V_NOEXCEPT;

		V_INLINE const auto& image_count( ) const V_NOEXCEPT;
		V_INLINE const auto& image_format( ) const V_NOEXCEPT;
		V_INLINE const auto& present_mode( ) const V_NOEXCEPT;

		V_INLINE const auto& extent( ) const V_NOEXCEPT;

		V_INLINE const auto uses_max_extent( ) const V_NOEXCEPT;
		V_INLINE const auto can_be_recreated( ) const V_NOEXCEPT;

		auto images( ) const;
		auto views( ) const;
	};
}

#include "swapchain.inl"