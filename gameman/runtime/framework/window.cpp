#include "window.hpp"

namespace framework {
	window& window::open( bool* status ) {
		glfwInit( ) != 0;

		glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

		if ( m_window = glfwCreateWindow(
			m_props.m_size.x, m_props.m_size.y,
			m_props.m_title.data( ), nullptr,
			nullptr
		); !m_window ) {
			*status = false;

			return *this;
		}

		*status = true;

		return *this;
	}

	window& window::close( bool* status ) {
		glfwDestroyWindow( m_window );

		if ( status )
			*status = true;

		return *this;
	}
}