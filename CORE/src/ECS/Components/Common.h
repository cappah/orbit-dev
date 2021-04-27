#pragma once

namespace Orbit::ECS {
	struct Common {
		FORCEINLINE Common() = default;
		FORCEINLINE ~Common() = default;
		FORCEINLINE Common(const Common&) = default;
		FORCEINLINE Common(std::string name) : Name(name) { }

		std::string Name;
		bool IsActive = true;
	};
}

