#ifndef __PROJECTDEFS_H__
#define __PROJECTDEFS_H__

#define INFO
//#define DEBUG
//#define TIMING

// MODIFY: C:\Users\clamb\Documents\Arduino\libraries\Tlc5940
// #define NUM_TLCS    5
// Also changed:
//  #define TLC_PWM_PERIOD    8075
// #define TLC_GSCLK_PERIOD    4

#ifdef INFO
 #define info_print(x)     Serial.print(x)
 #define info_println(x)   Serial.println(x)
#else
 #define info_print(x)
 #define info_println(x)
#endif

#ifdef DEBUG
 #define debug_print(x)     Serial.print(x)
 #define debug_println(x)   Serial.println(x)
#else
 #define debug_print(x)
 #define debug_println(x)
#endif

#ifdef TIMING
 #define timing_print(x)     Serial.print(x)
 #define timing_println(x)   Serial.println(x)
#else
 #define timing_print(x)
 #define timing_println(x)
 #endif

#define PATTERN_CHANGE_DELAY 250
#define COLOR_STEPS_IN_WHEEL 255

#define LEFT_DIAL_A0_PIN    0
#define RIGHT_DIAL_A1_PIN   1
#define TOGGLE_SWITCH_PIN   48

#endif // __PROJECTDEFS_H__
