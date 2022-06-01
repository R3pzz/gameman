#pragma once
#include <config.hpp>

namespace render {
	class rhi {
	public:
		V_VIRTUAL void create( ) V_VIRTUAL_POST;

		V_INLINE static rhi* instance( );
	};
}

#include "rhi.inl"