#pragma once
#include <config.hpp>

namespace utility {
	class file_reader {
	private:
		std::vector< char > m_buffer{};

	public:
		V_INLINE file_reader( ) = default;
		V_INLINE ~file_reader( ) = default;

		V_INLINE file_reader( const file_reader& ) = default;
		V_INLINE file_reader& operator=( const file_reader& ) = default;

		V_INLINE file_reader( file_reader&& ) = default;
		V_INLINE file_reader& operator=( file_reader&& ) = default;

		file_reader& read( const std::filesystem::path path );

		V_INLINE const auto& buffer( ) const V_NOEXCEPT;
	};
}

#include "file.inl"