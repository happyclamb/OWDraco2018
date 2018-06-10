#ifndef __SINGLETONMANAGER_H__
#define __SINGLETONMANAGER_H__

#include "InputManager.h"
class InputManager;

#include "LightManager.h"
class LightManager;

class SingletonManager
{
	public:
		SingletonManager() {}

		InputManager* inputMan() { return inputManager; }
		void setInputMan(InputManager* _inputMan) { inputManager = _inputMan; }

		LightManager* lightMan() { return lightManager; }
		void setLightMan(LightManager* _lightMan) { lightManager = _lightMan; }

	private:
		InputManager* inputManager = NULL;
		LightManager* lightManager = NULL;
};

#endif // __SINGLETONMANAGER_H__
