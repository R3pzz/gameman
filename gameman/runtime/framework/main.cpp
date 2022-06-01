#include <framework/window.hpp>
#include <vulkan_api/vulkan_api.hpp>

int main( int argc, char** argv ) {
	auto& window = framework::window::instance( );
	
	framework::window_props_t props{};
	props.m_floating = false;
	props.m_size = { 800, 600 };
	props.m_title = "gameman engine";

	bool state{};

	window.set_props( props ).open( &state );

	if ( !state )
		return 1;
	
	auto& ctx = render::implcontext::instance( );

	ctx.create( );
	
	while ( !window.should_close( ) ) {
		window.update( );
	}

	ctx.shutdown( );
	window.close( nullptr );

	return 0;
}