#pragma once
#include <config.hpp>

namespace engine { struct event_t; }

namespace framework {
	struct window_props_t {
		std::string m_title{};

		glm::ivec2 	m_size{};
		
		float				m_opacity{};
		
		bool 				m_floating{};
	};

	class window {
		V_INLINE window( const window& ) = delete;
		V_INLINE window& operator=( const window& ) = delete;

	private:
		window_props_t 	m_props{};
		bool						m_update{};
		GLFWwindow* 		m_window{};

	public:
		V_INLINE constexpr window( ) = default;
		V_INLINE ~window( );

		V_INLINE window( const window_props_t& props )
			: m_props{ props } {}

		V_INLINE window( window&& ) = default;
		V_INLINE window& operator=( window&& ) = default;

		window& open( bool* status );
		window& close( bool* status );

		V_INLINE window& update( );
		V_INLINE bool should_close( ) const;

		V_INLINE window& set_resize_callback( GLFWwindowsizefun cb );
		V_INLINE window& set_close_callback( GLFWwindowclosefun cb );

		// dispatch an action
		V_INLINE window& set_key_callback( GLFWkeyfun cb );
		// do not dispatch, just pressed and released a key
		V_INLINE window& set_char_callback( GLFWcharfun cb );

		V_INLINE window& set_mouse_callback( GLFWmousebuttonfun cb );
		V_INLINE window& set_scroll_callback( GLFWscrollfun cb );
		V_INLINE window& set_mouse_move_callback( GLFWcursorposfun cb );

		V_INLINE const window_props_t& props( ) V_NOEXCEPT;
		V_INLINE window& set_props( const window_props_t& from ) V_NOEXCEPT;

		// this will return a g_window declared down below.
		V_INLINE static window& instance( );

		V_INLINE operator GLFWwindow* ( ) { return m_window; }
	};

	V_INLINE const auto g_window = std::make_unique< window >( );
}

#include "window.inl"