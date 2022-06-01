#pragma once
#include <includes.hpp>

#ifdef V_NO_VIRTUAL
# define V_VIRTUAL
# define V_VIRTUAL_POST
# define V_DECL_INTERFACE( name )
#else
# define V_VIRTUAL virtual
# define V_VIRTUAL_POST = 0
# define V_DECL_INTERFACE( name ) : public name
#endif

#ifndef V_NO_INLINE
# ifdef V_INLINE_FORCE
#  define V_INLINE __forceinline
# else
#  define V_INLINE inline
# endif
#else
# define V_INLINE
#endif

#ifdef V_USE_OVERRIDE
# define V_OVERRIDE override
#else
# define V_OVERRIDE
#endif

#ifdef V_USE_NOEXCEPT
# define V_NOEXCEPT noexcept
#else
# define V_NOEXCEPT
#endif

#if defined( _WIN32 )
# define V_WIN32
#elif defined( _WIN64 )
# define V_WIN64
#elif defined( __linux__ )
# define V_LINUX
#endif

#ifdef _DEBUG
# include <cassert>
# define V_ASSERT( expr ) assert( expr )
#else
# include <stdexcept>
# define V_ASSERT( expr ) \
	if ( !( expr ) ) \
		throw std::runtime_error{ expr##"failed!" };
#endif

V_INLINE constexpr auto k_support_depth_clamp{ true };

enum struct e_render_api {
	vulkan,
	dx9,
	dx10,
	dx12,
	opengl,
	hammer
};

V_INLINE constexpr auto k_render_api{ e_render_api::vulkan };