#pragma once
#include <config.hpp>

namespace render::impl {
	struct specs_t {
		std::string m_app_name{},
								m_engine_name{};

		int 				m_app_ver{},
								m_engine_ver{};
		
		std::vector<
			std::string
		> 					m_layers{},
								m_extensions{};
	};
}