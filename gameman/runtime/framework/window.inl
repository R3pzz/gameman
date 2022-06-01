namespace framework {
	V_INLINE window::~window( ) { close( nullptr ); }

	V_INLINE window& window::update( ) {
		glfwPollEvents( );

		return *this;
	}
	
	V_INLINE bool window::should_close( ) const { return glfwWindowShouldClose( m_window ); }

	V_INLINE window& window::set_resize_callback( GLFWwindowsizefun cb ) {
		glfwSetWindowSizeCallback( m_window, cb );

		return *this;
	}

	V_INLINE window& window::set_close_callback( GLFWwindowclosefun cb ) {
		glfwSetWindowCloseCallback( m_window, cb );

		return *this;
	}

	V_INLINE window& window::set_key_callback( GLFWkeyfun cb ) {
		glfwSetKeyCallback( m_window, cb );

		return *this;
	}

	V_INLINE window& window::set_char_callback( GLFWcharfun cb ) {
		glfwSetCharCallback( m_window, cb );

		return *this;
	}

	V_INLINE window& window::set_mouse_callback( GLFWmousebuttonfun cb ) {
		glfwSetMouseButtonCallback( m_window, cb );

		return *this;
	}

	V_INLINE window& window::set_scroll_callback( GLFWscrollfun cb ) {
		glfwSetScrollCallback( m_window, cb );

		return *this;
	}

	V_INLINE window& window::set_mouse_move_callback( GLFWcursorposfun cb ) {
		glfwSetCursorPosCallback( m_window, cb );

		return *this;
	}

	V_INLINE window& window::set_props( const window_props_t& from ) V_NOEXCEPT {
		m_props = from;
	
		return *this;
	}

	V_INLINE const window_props_t& window::props( ) V_NOEXCEPT { return m_props; }

	V_INLINE window& window::instance( ) { return *g_window; }
}