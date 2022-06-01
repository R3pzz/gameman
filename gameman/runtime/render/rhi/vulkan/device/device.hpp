#pragma once
#include <config.hpp>
#include "../instance/instance.hpp"

namespace render::impl {
	enum struct e_family : std::uint32_t {
		present,
		graphics,
		compute,
		transfer
	};

	enum struct e_device_type : std::uint32_t {
		other = 0u,
		integrated,
		discrete,
		abstract,
		cpu
	};

	template < typename _utype = std::underlying_type_t< e_device_type > >
	V_INLINE _utype operator-( const e_device_type rhs );

	struct gpu_req_t {
		// require validation layers
		bool 	m_require_vl{},
					m_require_present{},
					m_require_graphics{},
					m_require_compute{},
					m_require_transfer{};
		
		e_device_type m_type{};

		bool 					m_allow_any_type{};

		std::uint32_t m_min_api_ver{},
									m_min_mem_size{},
									m_desired_mem_size{};

		std::vector<
			const char* > m_required_exts{},
										m_desired_exts{};
	};

	struct queue_desc_t {
		V_INLINE queue_desc_t( )
			: m_graphics_queue{ -1 },
				m_present_queue{ -1 },
				m_transfer_queue{ -1 },
				m_compute_queue{ -1 } {}

		int m_graphics_queue{},
				m_present_queue{},
				m_transfer_queue{},
				m_compute_queue{};
	};

	class device {
		V_INLINE device( const device& ) = delete;
		V_INLINE device& operator=( const device& ) = delete;

	private:
		VkInstance								m_instance{};

		VkPhysicalDevice 					m_gpu{};

		VkDevice 									m_device{};

		VkPhysicalDeviceFeatures 	m_features{};

		VkSurfaceKHR							m_surface{};

		std::vector<
			const char* > 					m_extensions{},
															m_layers{};

		std::unordered_map<
			e_family, int > 				m_families{};

		std::unordered_map<
			e_family, VkQueue > 		m_queues{};

	public:
		V_INLINE device( ) = default;
		V_INLINE ~device( );

		V_INLINE device( const VkInstance inst )
			: m_instance{ inst } {}

		V_INLINE device( const instance& inst )
			: m_instance{ inst.get( ) } {}

		device& connect_gpu( const gpu_req_t& req, const VkSurfaceKHR surf );
		device& build_device( );

		V_INLINE device& adopt_instance( const VkInstance inst );

		V_INLINE const auto& gpu_handle( ) const V_NOEXCEPT;
		V_INLINE const auto& device_handle( ) const V_NOEXCEPT;

		V_INLINE const auto& queue( const e_family family ) const;
		V_INLINE const auto& fam_index( const e_family family ) const;

		V_INLINE const auto& features( ) const V_NOEXCEPT;

		V_INLINE const auto& extensions( ) const V_NOEXCEPT;
		V_INLINE const auto& layers( ) const V_NOEXCEPT;

		V_INLINE const auto& surface( ) const V_NOEXCEPT;

		V_INLINE void shutdown( );
	};
}

#include "device.inl"