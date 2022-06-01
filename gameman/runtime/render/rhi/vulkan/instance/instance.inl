namespace render::impl {
	V_INLINE instance::~instance( ) { vkDestroyInstance( m_handle, nullptr ); }

	V_INLINE auto& instance::request_layer( const std::string_view name ) {
		m_layers.push_back( name.data( ) );

		return *this;
	}

	V_INLINE bool instance::result( ) V_NOEXCEPT const { return m_status; }

	V_INLINE VkInstance instance::get( ) V_NOEXCEPT const { return m_handle; }
}