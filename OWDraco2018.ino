#include <Arduino.h>

#include "ProjectDefs.h"

#include "SingletonManager.h"
#include "InputManager.h"
#include "LightManager.h"

SingletonManager *singleMan = NULL;

void setup() {
//	Serial.begin(115200);
	Serial.begin(9600);

	// Create the holder for global objects
	singleMan = new SingletonManager();

	InputManager *inputMan = new InputManager(singleMan);
	delay(50); // Wait for inputs to init before continuing

	// Initilize the LightManager
	LightManager *lightMan = new LightManager(singleMan);
	delay(50); // Wait for light to init before continuing

	info_println("Info Logging enabled");
	debug_println("Debug Logging enabled");
	timing_println("Timing Logging enabled");
}

/*
https://www.google.ca/search?q=breadboard-arduino-tlc5940.png&rlz=1C1CHBF_enCA732CA732&source=lnms&tbm=isch&sa=X&ved=0ahUKEwiwgvrlxfjSAhUE42MKHUD9D74Q_AUICCgB&biw=1191&bih=602

C:\Users\clamb\Documents\Arduino\libraries

SinWave
http://forum.arduino.cc/index.php?topic=223906.0
https://www.sparkfun.com/tutorials/329
*/

void loop() {

	singleMan->inputMan()->updateValues();
	singleMan->lightMan()->updateLights();

	delay(30);
}
