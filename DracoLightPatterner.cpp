#include "DracoLightPatterner.h"

#include <Arduino.h>

#include "ProjectDefs.h"
#include "SingletonManager.h"

DracoLightPatterner::DracoLightPatterner(SingletonManager* _singleMan):
	singleMan(_singleMan) {

	hubs_array[0]         = singleMan->lightMan()->channelArray[0];
	hubs_array[1]         = singleMan->lightMan()->channelArray[1];
	hubs_array[2]         = singleMan->lightMan()->channelArray[2];
	hubs_array[3]         = singleMan->lightMan()->channelArray[3];
	tail_array[0]         = singleMan->lightMan()->channelArray[4];

	head_array[0]         = singleMan->lightMan()->channelArray[5];
	tube_tail_array[0]    = singleMan->lightMan()->channelArray[6];
	tube_tail_array[1]    = singleMan->lightMan()->channelArray[7];
	tube_tail_array[2]    = singleMan->lightMan()->channelArray[8];
	tube_tail_array[3]    = singleMan->lightMan()->channelArray[9];

	tube_middle_array[0]  = singleMan->lightMan()->channelArray[10];
	tube_middle_array[1]  = singleMan->lightMan()->channelArray[11];
	tube_middle_array[2]  = singleMan->lightMan()->channelArray[12];
	tube_middle_array[3]  = singleMan->lightMan()->channelArray[13];
	tube_middle_array[4]  = singleMan->lightMan()->channelArray[14];

	tube_head_array[0]    = singleMan->lightMan()->channelArray[15];
	tube_head_array[1]    = singleMan->lightMan()->channelArray[16];
	tube_head_array[2]    = singleMan->lightMan()->channelArray[17];
	tube_head_array[3]    = singleMan->lightMan()->channelArray[18];
	tube_head_array[4]    = singleMan->lightMan()->channelArray[19];
}

void DracoLightPatterner::drawPattern() {
	static unsigned long nextPatternSwitch = 0;
	static int currPattern = -1;

	if(nextPatternSwitch <= millis()) {
		currPattern++;
		if(currPattern >= 5)
			currPattern = 0;

		nextPatternSwitch = millis() + choosePattern(currPattern);
	}	else {
		choosePattern(currPattern);
	}
}

unsigned long DracoLightPatterner::choosePattern(int currPattern)
{
	unsigned long nextLength = 0;

	if(currPattern == 0)
		nextLength = pattern_general_colors();
	else if(currPattern == 1)
		nextLength = pattern_solid_fade();
	else if(currPattern == 2)
		nextLength = pattern_general_colors();
	else if(currPattern == 3)
		nextLength = pattern_solid_fade();
	else if(currPattern == 4)
		nextLength = pattern_circus();

	return nextLength;
}

unsigned long DracoLightPatterner::pattern_general_colors() {

	static unsigned long nextSpeedSwitch = 0;
	static int currSpeed = 1;

	if(nextSpeedSwitch <= millis()) {
		currSpeed = random(8,15);
		nextSpeedSwitch = millis() + 1000*15;
	}

	colorWheelSnippetFade(hubs_array,        4,  4*currSpeed, 5,   60, 100);
	colorWheelSnippetFade(tail_array,        1,  6*currSpeed, 0,    0, 255, true);
	colorWheelSnippetFade(head_array,        1,  6*currSpeed, 0,    0, 255, true);
	colorWheelSnippetFade(tube_tail_array,   4,  2*currSpeed, 10, 125, 235);
	colorWheelSnippetFade(tube_middle_array, 5, 10*currSpeed, 20,   0,  40);
	colorWheelSnippetFade(tube_head_array,   5, 10*currSpeed, 20,   0,  40);

	return 600000; // 10 min
}

unsigned long DracoLightPatterner::pattern_solid_fade() {
	static unsigned long nextSwitch = 0;
	static byte currWheelChoice = 1;
	static int currSpeed = 1;

	if(nextSwitch <= millis()) {
		currWheelChoice = random(255);
		currSpeed = random(5,15);

		nextSwitch = millis() + 1000*10;
	}

	colorWheelSnippetFade(hubs_array,        4,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);
	colorWheelSnippetFade(tail_array,        1,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);
	colorWheelSnippetFade(head_array,        1,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);
	colorWheelSnippetFade(tube_tail_array,   4,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);
	colorWheelSnippetFade(tube_middle_array, 5,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);
	colorWheelSnippetFade(tube_head_array,   5,   1, 2*currSpeed,  currWheelChoice, currWheelChoice);

	return 240000; // 4 min
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

	for(int i=0; i<25; i++) {
		singleMan->lightMan()->setColorToChannelFromWheelPosition(singleMan->lightMan()->channelArray[i], random(255), brightness);
	}
	delay(30);

	return 120000; // 2 min
}

void DracoLightPatterner::colorWheelSnippetFade(TLC_CHANNEL_TYPE colorArray[], int arraySize,
		int patternSpeed, int brightnessSpeed,
		byte wheelBegin, byte wheelEnd, bool wrap /*false*/) {

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
			int offset = totalSteps / arraySize;
			int currIndex = patternIndex + offset*i;
			if(currIndex > totalSteps)
				currIndex -= totalSteps;

			wheelStepPosition = currIndex<=stepRange ? currIndex : (stepRange - (currIndex-stepRange));
		}

		singleMan->lightMan()->setColorToChannelFromWheelPosition(colorArray[i], wheelBegin+wheelStepPosition, brightness);
	}
}
