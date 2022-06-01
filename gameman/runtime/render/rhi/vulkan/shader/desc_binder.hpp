#pragma once
#include <config.hpp>

namespace render::impl {
	struct layout_item_desc_t {
		std::size_t		m_size{};

		std::uint32_t m_layout_pos{};

		std::string		m_name{};

		VkFormat			m_format{};
	};

	auto bind_input( const std::vector< layout_item_desc_t >& descs,
		std::vector< VkVertexInputAttributeDescription >& out_attr,
		std::vector< VkVertexInputBindingDescription >& out_binding
	);
}