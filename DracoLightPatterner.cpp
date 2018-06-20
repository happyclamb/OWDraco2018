#include "DracoLightPatterner.h"

#include <Arduino.h>

#include "ProjectDefs.h"
#include "SingletonManager.h"

DracoLightPatterner::DracoLightPatterner(SingletonManager* _singleMan):
	singleMan(_singleMan) {

	head_array[0]         = singleMan->lightMan()->channelArray[0];
	hubs_array[0]         = singleMan->lightMan()->channelArray[1];
	tube_head_array[0]    = singleMan->lightMan()->channelArray[2];
	tube_head_array[1]    = singleMan->lightMan()->channelArray[3];
	tube_head_array[2]    = singleMan->lightMan()->channelArray[4];
	tube_head_array[3]    = singleMan->lightMan()->channelArray[5];
	tube_head_array[4]    = singleMan->lightMan()->channelArray[6];
	hubs_array[1]         = singleMan->lightMan()->channelArray[7];
	tube_middle_array[0]  = singleMan->lightMan()->channelArray[8];
	tube_middle_array[1]  = singleMan->lightMan()->channelArray[9];
	tube_middle_array[2]  = singleMan->lightMan()->channelArray[10];
	tube_middle_array[3]  = singleMan->lightMan()->channelArray[11];
	hubs_array[2]         = singleMan->lightMan()->channelArray[12];
	tube_tail_array[0]    = singleMan->lightMan()->channelArray[13];
	tube_tail_array[1]    = singleMan->lightMan()->channelArray[14];
	tube_tail_array[2]    = singleMan->lightMan()->channelArray[15];
	tube_tail_array[3]    = singleMan->lightMan()->channelArray[16];
	hubs_array[3]         = singleMan->lightMan()->channelArray[17];
	tail_array[0]         = singleMan->lightMan()->channelArray[18];
}

void DracoLightPatterner::drawPattern() {
	static unsigned long nextPatternSwitch = 0;
	static int currPattern = -1;

	if(nextPatternSwitch <= millis()) {
		currPattern++;
		if(currPattern >= 8)
			currPattern = 0;

		nextPatternSwitch = millis() + choosePattern(currPattern);
	}	else {
		choosePattern(currPattern);
	}
}

unsigned long DracoLightPatterner::choosePattern(int currPattern)
{
	switch (currPattern) {
		case 0:   return pattern_general_colors();	// 5 min
		case 1:   return pattern_rainbow(60, 100, false);  // 1 min
		case 2:   return pattern_solid_fade(false);	// 3 min
		case 3:   return pattern_rainbow(0, 255, true); // 1 min
		case 4:   return pattern_general_colors(); // 5 min
		case 5:   return pattern_rainbow(60, 100, false);  // 1 min
		case 6:   return pattern_solid_fade(true);	// 3 min
		case 7:   return pattern_circus(); // 15 sec
	}
}

unsigned long DracoLightPatterner::pattern_rainbow(byte minWheel, byte maxWheel, bool wrap) {
	static int currSpeed = 1;
	static unsigned long nextSwitch = 0;

	if(nextSwitch <= millis()) {
		currSpeed = random(5,15);
		nextSwitch = millis() + 1000*10;
	}

	colorWheelSnippetFade(singleMan->lightMan()->channelArray, 19, 5, 5*currSpeed, minWheel, maxWheel, true, wrap);

	return 60000; // 60 sec
}


unsigned long DracoLightPatterner::pattern_general_colors() {

	static unsigned long nextSpeedSwitch = 0;
	static int currSpeed = 1;

	if(nextSpeedSwitch <= millis()) {
		currSpeed = random(8,15);
		nextSpeedSwitch = millis() + 1000*15;
	}
	/*
	                      8, 9, 10, 11, 12, 13, 14
	36*(currSpeed-8) ==>  0,36, 72,108,144,180,216
	36*(currSpeed-7) ==> 36,72,108,144,180,216,252
	*/
	colorWheelSnippetFade(hubs_array,        4,  4*currSpeed, 5,   36*(currSpeed-8), 36*(currSpeed-7), false);
	colorWheelSnippetFade(tail_array,        1,  6*currSpeed, 0,    0, 255, true, true);
	colorWheelSnippetFade(head_array,        1,  6*currSpeed, 0,    0, 255, true, true);
	colorWheelSnippetFade(tube_tail_array,   4,  3*currSpeed, 13, 50, 110);
	colorWheelSnippetFade(tube_middle_array, 4,  4*currSpeed, 11, 50, 110);
	colorWheelSnippetFade(tube_head_array,   5,  5*currSpeed,  9, 50, 110);

	return 300000; // 5 min
}

unsigned long DracoLightPatterner::pattern_solid_fade(bool useOffset) {
	static unsigned long nextSwitch = 0;
	static byte currWheelChoice = 1;
	static int currSpeed = 1;

	if(nextSwitch <= millis()) {
		currWheelChoice = random(255);
		currSpeed = random(5,15);

		nextSwitch = millis() + 1000*10;
	}

	byte offsetWheelChoice = currWheelChoice;
	if(useOffset)
		offsetWheelChoice = currWheelChoice > 127 ? currWheelChoice - 128 : currWheelChoice + 128;

	colorWheelSnippetFade(hubs_array,        4,   1, 2*currSpeed,  offsetWheelChoice, offsetWheelChoice);
	colorWheelSnippetFade(tail_array,        1,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);
	colorWheelSnippetFade(head_array,        1,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);
	colorWheelSnippetFade(tube_tail_array,   4,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);
	colorWheelSnippetFade(tube_middle_array, 4,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);
	colorWheelSnippetFade(tube_head_array,   5,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);

	return 180000; // 3 min
}

unsigned long DracoLightPatterner::pattern_circus() {
	unsigned long currTime = millis();

	static unsigned long nextSwitch = 0;
	static int currBright = 1;
	if(nextSwitch <= millis()) {
		currBright--;
		if(currBright < 0)
			currBright = 4;

		nextSwitch = millis() + 1000*6;
	}

	int brightnessSpeed = currBright*5;
	int brightness = 0;
	if(brightnessSpeed > 0) {
		int totalBrightSteps = 314;
		int brightnessAngleIndex = (currTime%(totalBrightSteps*brightnessSpeed))/brightnessSpeed;
		float cosBright = cos(brightnessAngleIndex/50.0);
		brightness = (cosBright + 1.0)*1300.0; // never turn it all the way off
	}

	for(int i=0; i<19; i++) {
		singleMan->lightMan()->setColorToChannelFromWheelPosition(singleMan->lightMan()->channelArray[i], random(255), brightness);
	}
	delay(30);

	return 15000; // 15 sec
}

void DracoLightPatterner::colorWheelSnippetFade(TLC_CHANNEL_TYPE colorArray[], int arraySize,
		int patternSpeed, int brightnessSpeed,
		byte wheelBegin, byte wheelEnd,
		bool spreadPattern /*true*/, bool wrap /*false*/) {

	unsigned long currTime = millis();

	/*	brightnessFade 40%->100%   4095 == off  1024 == 25%  2048 == 50%   0 == 100%     */
	/*   fade from 512->4095  (NOTE: to get less steps multiply brightness before use)  */
	int brightness = 0;
	if(brightnessSpeed > 0) {
			/*		cos(rad)		Calculates the cos of an angle (in radians). The result will be between -1 and 1.
			cos(0) == 1
			cos(3.14) == -1
			cos(6.28) == 1
			*/

		int totalBrightSteps = 314;
		int brightnessAngleIndex = (currTime%(totalBrightSteps*brightnessSpeed))/brightnessSpeed;
		float cosBright = cos(brightnessAngleIndex/50.0);
		brightness = (cosBright + 1.0)*1300.0; // never turn it all the way off
	}

	int stepRange = wheelEnd-wheelBegin;
	int patternIndex = 0;
	int totalSteps = 0;
	if(stepRange > 0) {
		long colorTimeBetweenSteps = patternSpeed;
		totalSteps = stepRange;
		if(wrap == false)
			totalSteps = stepRange*2;

		patternIndex = (currTime%(totalSteps*colorTimeBetweenSteps))/colorTimeBetweenSteps;
	}

	for(int i=0; i<arraySize; i++) {

		byte wheelStepPosition = 0;
		if(stepRange > 0) {
			int offset = spreadPattern ? (totalSteps / arraySize) : 0;
			int currIndex = patternIndex + offset*i;
			if(currIndex > totalSteps)
				currIndex -= totalSteps;

			wheelStepPosition = currIndex<=stepRange ? currIndex : (stepRange - (currIndex-stepRange));
		}

		singleMan->lightMan()->setColorToChannelFromWheelPosition(colorArray[i], wheelBegin+wheelStepPosition, brightness);
	}
}
