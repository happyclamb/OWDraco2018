#include "InputManager.h"

#include "ProjectDefs.h"

#include "SingletonManager.h"

InputManager::InputManager(SingletonManager* _singleMan) :
	singleMan(_singleMan)
{
	// Setup the addressing pins
	pinMode(TOGGLE_SWITCH_PIN, INPUT_PULLUP);

	singleMan->setInputMan(this);
}

void InputManager::updateValues() {

	// HIGH == off, LOW == on
	toggleUp = false;
	if(digitalRead(TOGGLE_SWITCH_PIN) == LOW)
		toggleUp = true;
	
	// Analog inputs (value 0->1024)
	leftDial = analogRead(LEFT_DIAL_A0_PIN);
	rightDial = analogRead(RIGHT_DIAL_A1_PIN);
}
