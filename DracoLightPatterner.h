#ifndef __DRACOLIGHTPATTERNER_H__
#define __DRACOLIGHTPATTERNER_H__

#include <Arduino.h>

#include "ProjectDefs.h"

#include "SingletonManager.h"
class SingletonManager;

class DracoLightPatterner {
	public:
		DracoLightPatterner(SingletonManager* _singleMan);

		void drawPattern();

	private:
		SingletonManager* singleMan = NULL;

		// Need to hold channel assignments for
		//	 hubs(4); tail(1); tube_tail (4); tube_middle(5); tube_head(5);  head(1);
		TLC_CHANNEL_TYPE hubs_array[4];
		TLC_CHANNEL_TYPE tail_array[1];
		TLC_CHANNEL_TYPE tube_tail_array[4];
		TLC_CHANNEL_TYPE tube_middle_array[5];
		TLC_CHANNEL_TYPE tube_head_array[5];
		TLC_CHANNEL_TYPE head_array[1];

		unsigned long choosePattern(int currPattern);
		unsigned long pattern_general_colors();
		unsigned long pattern_solid_fade();
		unsigned long pattern_circus();

		void colorWheelSnippetFade(TLC_CHANNEL_TYPE colorArray[], int arraySize, int patternSpeed,
				int brightnessSpeed, byte wheelBegin, byte wheelEnd, bool wrap=false);
};

#endif // __DRACOLIGHTPATTERNER_H__
