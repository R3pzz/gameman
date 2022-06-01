#pragma once
#include <config.hpp>
#include "desc_binder.hpp"

namespace render::impl {
	struct scissor_t {
		glm::uvec2 m_offset{},
								m_extent{};
	};

	struct viewport_t {
		glm::vec2 m_offset{},
							m_extent{},
							m_depth{};
	};

	class device;

	class shader {
	private:
		const device* 				m_device{};
		VkShaderModule				m_shader{};
		
		std::filesystem::path m_working_dir{};

		std::string 					m_name{};
		int 									m_type{};

		std::vector< layout_item_desc_t > m_descs{};
		std::vector< viewport_t > m_viewports{};
		std::vector< scissor_t > 	m_scissors{};

		bool									m_depth_clamp{},
													m_rasterizer_discard{},
													m_culling{};

		VkPrimitiveTopology		m_topology{};
		VkPolygonMode					m_poly_mode{};

		float									m_line_width{};

		std::uint32_t					m_sample_count{};
		
	public:
		V_INLINE shader( ) = default;
		V_INLINE shader( const device* dev )
			: m_device{ dev } {}

		auto& create( const std::filesystem::path path );
		auto& refresh( );
		void shutdown( );

		auto& use( const VkShaderStageFlagBits type );

		V_INLINE auto& add_desc( const layout_item_desc_t& desc );
		V_INLINE auto& remove_desc( const layout_item_desc_t& desc );

		V_INLINE auto& set_topology( const VkPrimitiveTopology type );

		V_INLINE auto& add_scissor( const scissor_t& sc );
		V_INLINE auto& add_viewport( const viewport_t& wp );

		V_INLINE auto& remove_scissor( const scissor_t& sc );
		V_INLINE auto& remove_viewport( const viewport_t& wp );

		V_INLINE auto& set_type( const VkShaderStageFlagBits bits ) V_NOEXCEPT;

		V_INLINE auto& adopt_device( const device* dev ) V_NOEXCEPT;

		V_INLINE const auto& name( ) const V_NOEXCEPT;
		V_INLINE const auto& type( ) const V_NOEXCEPT;
	};
}

#include "shader.inl"