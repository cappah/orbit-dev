#pragma once

/* 
	client application! this will be include in APP!
*/

#include "PlatformWindow.h"
#include "PlatformInputs.h"
#include "Scene/Scene.h"

namespace Orbit {

	class AppConfig { };

	namespace Application {
		void OB_API Run();

		Scene* GetScene();

		PlatformInputs* Inputs();

		PlatformWindow* Window();

		EventDispatcher* Dispatcher();

		/* client application config 
			not really naccessary right now
		*/
		extern AppConfig Create();
	}
}
