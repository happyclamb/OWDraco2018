#include "LightManager.h"

#include <Arduino.h>

#include "Tlc5940.h"
#include "ProjectDefs.h"
#include "SingletonManager.h"

#include "DebugLightPatterner.h"


LightManager::LightManager(SingletonManager* _singleMan):
	singleMan(_singleMan) {

	Tlc.init(4095);

	for(int i=0;i<NUM_TLCS;i++) {
		for(int j=0;j<5;j++) {
			channelArray[(i*5)+j] = (i*16)+(j*3);

			debug_print((i*5)+j);
			debug_print(": ");
			debug_println(channelArray[(i*5)+j]);
		}
  }

	singleMan->setLightMan(this);

	debugPatterner = new DebugLightPatterner(_singleMan);
	dracoPatterner = new DracoLightPatterner(_singleMan);
}


void LightManager::updateLights() {

	if(singleMan->inputMan()->isToggleUp())
		debugPatterner->drawPattern();
	else
		dracoPatterner->drawPattern();

	Tlc.update();
}

void LightManager::setColorToChannel(TLC_CHANNEL_TYPE channelToSet, byte red, byte green, byte blue, int brightness /*-1*/) {
	if(brightness < 0)
		brightness = map(singleMan->inputMan()->getLeftDial(), 0, 1023, 4095, 0);
	Tlc.set(channelToSet+0, map(red  , 0, 255, 4095, brightness));
	Tlc.set(channelToSet+1, map(green, 0, 255, 4095, brightness));
	Tlc.set(channelToSet+2, map(blue , 0, 255, 4095, brightness));
}

void LightManager::setColorToChannelFromWheelPosition(TLC_CHANNEL_TYPE channelToSet, byte wheelPos, int brightness /*-1*/) {
	byte r=0;
	byte g=0;
	byte b=0;
	this->colorFromWheelPosition(wheelPos, &r, &g, &b);
	this->setColorToChannel(channelToSet, r, g, b, brightness);
}

void LightManager::colorFromWheelPosition(byte wheelPos, byte *r, byte *g, byte *b) {
	if (wheelPos < 85) {
		// 0-->85
		*r = wheelPos * 3;
		*g = 255 - wheelPos * 3;
		*b = 0;
	} else if (wheelPos < 170) {
		// 85-->170
		wheelPos -= 85;
		*r = 255 - wheelPos * 3;
		*g = 0;
		*b = wheelPos * 3;
	} else {
		// 170-->255
		wheelPos -= 170;
		*r = 0;
		*g = wheelPos * 3;
		*b = 255 - wheelPos * 3;
	}
}
