#include "DebugLightPatterner.h"

#include <Arduino.h>

#include "ProjectDefs.h"
#include "SingletonManager.h"

DebugLightPatterner::DebugLightPatterner(SingletonManager* _singleMan):
	singleMan(_singleMan) {

}

void DebugLightPatterner::drawPattern() {
	long blinkSpeed=500;

	for(int i=0; i < singleMan->lightMan()->getTotalChannels(); i++) {
		singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], 0, 0, 0);
	}

	static int currPattern = 0;
	int newPattern =  map(singleMan->inputMan()->getRightDial(), 0, 1023, 0, 4);
	if(newPattern != currPattern) {
		currPattern = newPattern;
		info_println(newPattern);
	}

	switch(currPattern)
	{
		case 0:
			testPatternRGBAll(blinkSpeed);
			break;
		case 1:
			testPatternStationCount(blinkSpeed);
			break;
		case 2:
			testPatternStationChannelCount(blinkSpeed);
			break;
		case 3:
			testPatternFullCount(blinkSpeed);
			break;
		case 4:
			testPatternAllWhite();
			break;
		}
}

void DebugLightPatterner::testPatternAllWhite() {
	for(int i=0; i<singleMan->lightMan()->getTotalChannels(); i++) {
		singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], 255, 255, 255);
	}
}

void DebugLightPatterner::testPatternRGBAll(long blinkSpeed) {
	long totalColors = 3;
	byte colorIndex = (millis()%(totalColors*blinkSpeed))/blinkSpeed;

	byte red=0, green=0, blue=0;
	if(colorIndex == 0) red = 255;
	else if (colorIndex == 1) green = 255;
	else blue = 255;

	for(int i=0; i<singleMan->lightMan()->getTotalChannels(); i++) {
		singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], red, green, blue);
	}
}

void DebugLightPatterner::testPatternStationCount(long blinkSpeed) {
	byte stationIndex = (millis()%(NUM_TLCS*blinkSpeed))/blinkSpeed;

	byte channelOffset = stationIndex*5;
	for(int i=channelOffset; i<channelOffset+5; i++) {
		if(i == 0)
			singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], 255, 0, 0);
		else
			singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], 255, 255, 255);
	}
}

void DebugLightPatterner::testPatternStationChannelCount(long blinkSpeed) {
	byte stationIndex = (millis()%(5*blinkSpeed))/blinkSpeed;

	for(int i=stationIndex; i<singleMan->lightMan()->getTotalChannels(); i+=5) {
		if(i == 0)
			singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], 255, 0, 0);
		else
			singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[i], 255, 255, 255);
	}
}

void DebugLightPatterner::testPatternFullCount(long blinkSpeed) {
	long fullCount = singleMan->lightMan()->getTotalChannels()*3;
	byte countIndex = (millis() % (fullCount*blinkSpeed)) / blinkSpeed;

	byte channel = countIndex / 3;
	byte rgbOffset = countIndex % 3;

	byte red=0, green=0, blue=0;
	if(rgbOffset == 0) red = 255;
	else if (rgbOffset == 1) green = 255;
	else blue = 255;

	singleMan->lightMan()->setColorToChannel(singleMan->lightMan()->channelArray[channel], red, green, blue);
}
