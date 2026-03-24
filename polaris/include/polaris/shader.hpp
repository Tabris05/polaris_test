#pragma once

#include <polaris/pod_types.hpp>

namespace pl {
	class Shader {
		public:
			Shader(const ShaderCreateInfo& ci);
			Shader(Shader&& src);
			Shader& operator=(Shader&& src);
			~Shader();

			Shader(const Shader& src) = delete;
			Shader& operator=(const Shader& src) = delete;

		private:
			alignas(8) byte reserved[24];

	};
}