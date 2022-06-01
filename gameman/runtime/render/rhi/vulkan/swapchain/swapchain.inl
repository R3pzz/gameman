namespace render::impl {
	V_INLINE swapchain& swapchain::adopt_device( const device* dev ) V_NOEXCEPT {
		m_device = dev;

		return *this;
	}

	V_INLINE const auto& swapchain::swapchain_handle( ) const V_NOEXCEPT { return m_swapchain; }

	V_INLINE const auto& swapchain::image_count( ) const V_NOEXCEPT { return m_image_count; }

	V_INLINE const auto& swapchain::image_format( ) const V_NOEXCEPT { return m_image_format; }

	V_INLINE const auto& swapchain::extent( ) const V_NOEXCEPT { return m_extent; }

	V_INLINE const auto swapchain::uses_max_extent( ) const V_NOEXCEPT { return m_use_max_extent; }

	V_INLINE const auto swapchain::can_be_recreated( ) const V_NOEXCEPT { return m_can_be_recreated; }
}