#pragma once
#include <config.hpp>

namespace ext::math {
	template < typename _type >
	V_INLINE _type clamp( _type val, _type min, _type max ) V_NOEXCEPT;

	template < typename _type >
	V_INLINE _type clamp( _type val, _type max ) V_NOEXCEPT;
}

#include "clamp.inl"