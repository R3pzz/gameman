#include "desc_binder.hpp"

namespace render::impl {
	auto bind_input( const std::vector< layout_item_desc_t >& descs,
		std::vector< VkVertexInputAttributeDescription >& out_attr,
		std::vector< VkVertexInputBindingDescription >& out_binding
	) {
		auto offset = std::uint32_t{};

		for ( std::size_t it{}; it < descs.size( ); it++ ) {
			auto& desc = descs.at( it );

			offset += desc.m_size;

			auto attr = VkVertexInputAttributeDescription{};
			attr.location = desc.m_layout_pos;
			attr.offset = offset;
			attr.format = desc.m_format;
			attr.binding = 0u;

			out_attr.push_back( attr );

			auto binding = VkVertexInputBindingDescription{};
			binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			binding.binding = 0u;
			binding.stride = desc.m_size;

			out_binding.push_back( binding );
		}

		auto input_info = VkPipelineVertexInputStateCreateInfo{};
		input_info.pVertexAttributeDescriptions = out_attr.data( );
		input_info.pVertexBindingDescriptions = out_binding.data( );
		input_info.vertexAttributeDescriptionCount = out_attr.size( );
		input_info.vertexBindingDescriptionCount = out_binding.size( );
		input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		return input_info;
	}
}