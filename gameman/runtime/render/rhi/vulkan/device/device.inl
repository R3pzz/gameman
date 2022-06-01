namespace render::impl {
	template < typename _utype = std::underlying_type_t< e_device_type > >
	V_INLINE _utype operator-( const e_device_type rhs ) { return static_cast< _utype >( rhs ); }

	V_INLINE const auto& device::gpu_handle( ) const V_NOEXCEPT { return m_gpu; }

	V_INLINE const auto& device::device_handle( ) const V_NOEXCEPT { return m_device; }

	V_INLINE const auto& device::queue( const e_family family ) const { return m_queues.at( family ); }

	V_INLINE const auto& device::fam_index( const e_family family ) const { return m_families.at( family ); }

	V_INLINE const auto& device::features( ) const V_NOEXCEPT { return m_features; }

	V_INLINE const auto& device::extensions( ) const V_NOEXCEPT { return m_extensions; }

	V_INLINE const auto& device::layers( ) const V_NOEXCEPT { return m_layers; }
	
	V_INLINE const auto& device::surface( ) const V_NOEXCEPT { return m_surface; }

	V_INLINE device& device::adopt_instance( const VkInstance inst ) {
		m_instance = inst;
	
		return *this;
	}

	V_INLINE void device::shutdown( ) { vkDestroyDevice( m_device, nullptr ); }

	V_INLINE device::~device( ) { shutdown( ); }
}