#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include <Arduino.h>
#include "ProjectDefs.h"

#include "SingletonManager.h"
class SingletonManager;

class InputManager
{
	public:
		InputManager(SingletonManager* _singleMan);

		void updateValues();

		bool isToggleUp() { return this->toggleUp; }
		int getLeftDial() { return this->leftDial; }
		int getRightDial() { return this->rightDial; }

	private:
		SingletonManager* singleMan = NULL;

		bool toggleUp = false;
		int leftDial = 0;
		int rightDial = 0;
};

#endif // __INPUTMANAGER_H__
