#pragma once

#include "Application.h"

#ifdef PLATFORM_WINDOWS
	int main(int argc, char* argv) {
		Orbit::Application::Run();
		return EXIT_SUCCESS;
	}
#endif

