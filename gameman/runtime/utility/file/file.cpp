#include "file.hpp"

namespace utility {
	file_reader& file_reader::read( const std::filesystem::path path ) {
		auto stream = std::ifstream{ path, std::ios::ate };

		if ( !stream.is_open( ) )
			throw std::runtime_error{ "file_reader: unable to open stream!" };

		auto size = stream.tellg( );
		
		m_buffer.resize( size );

		stream.seekg( 0u );
		stream.read( m_buffer.data( ), size );
		stream.close( );

		return *this;
	}
}