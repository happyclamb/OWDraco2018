#ifndef __DEBUGLIGHTPATTERNER_H__
#define __DEBUGLIGHTPATTERNER_H__

#include <Arduino.h>

#include "ProjectDefs.h"

#include "SingletonManager.h"
class SingletonManager;

class DebugLightPatterner {
	public:
		DebugLightPatterner(SingletonManager* _singleMan);

		void drawPattern();

	private:
		SingletonManager* singleMan = NULL;

		void testPatternRGBAll(long blinkSpeed);
		void testPatternStationCount(long blinkSpeed);
		void testPatternStationChannelCount(long blinkSpeed);
		void testPatternFullCount(long blinkSpeed);
};

#endif // __DEBUGLIGHTPATTERNER_H__
