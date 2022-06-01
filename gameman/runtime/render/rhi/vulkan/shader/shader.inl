namespace render::impl {
	V_INLINE auto& shader::add_desc( const layout_item_desc_t& desc ) {
		m_descs.push_back( desc );

		return *this;
	}

	V_INLINE auto& shader::remove_desc( const layout_item_desc_t& desc ) {
		auto pos = std::find( m_descs.begin( ), m_descs.end( ), desc );

		if ( pos == m_descs.end( ) )
			return *this;
		else
			m_descs.erase( pos );

		return *this;
	}

	V_INLINE auto& shader::adopt_device( const device* dev ) V_NOEXCEPT {
		m_device = dev;

		return *this;
	}

	V_INLINE auto& shader::set_type( const VkShaderStageFlagBits bits ) V_NOEXCEPT {
		m_type = static_cast< int >( bits );

		return *this;
	}

	V_INLINE const auto& shader::name( ) const V_NOEXCEPT { return m_name; }
	
	V_INLINE const auto& shader::type( ) const V_NOEXCEPT { return m_type; }
}