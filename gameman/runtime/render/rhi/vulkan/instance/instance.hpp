#pragma once
#include <config.hpp>

namespace render::impl {
	struct instance_req_t {
		std::string 	m_engine_name{},
									m_app_name{};

		std::uint32_t m_engine_ver{},
									m_app_ver{}; 

		std::uint32_t m_min_api_ver{};
	};

	class instance {
		V_INLINE instance( const instance& ) = delete;
		V_INLINE instance& operator=( const instance& ) = delete;

	private:
		VkInstance 			m_handle{};

		std::vector<
			const char* > m_exts{},
										m_layers{};

		bool 						m_status{};

	public:
		V_INLINE constexpr instance( ) = default;
		V_INLINE ~instance( );

		V_INLINE instance( instance&& ) = default;
		V_INLINE instance& operator=( instance&& ) = default;

		V_INLINE auto& request_layer( const std::string_view name );

		instance& create( const instance_req_t& req );
		instance& destroy( );

		V_INLINE bool result( ) V_NOEXCEPT const;
		V_INLINE VkInstance get( ) V_NOEXCEPT const;
	};
}

#include "instance.inl"