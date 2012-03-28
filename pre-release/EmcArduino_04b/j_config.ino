// Adjust these to reflect EMC's limits in 1/1000ths of an inch.
#define xMinLimit -14000 //-500000
#define yMinLimit -14000 //-250000
#define zMinLimit -14000
#define aMinLimit -10000
#define xMaxLimit 140000
#define yMaxLimit 140000
#define zMaxLimit 140000
#define aMaxLimit 100000
#define xHomeSwitch 2000
#define yHomeSwitch 2000
#define zHomeSwitch 2000
#define aHomeSwitch 2000
#define stepsPerInchX 3200
#define stepsPerInchY 3200
#define stepsPerInchZ 3200
#define stepsPerInchA 3200  // 200 steps per revolution with 1/16th microstepping = 3200 (aka: 200 * 16 = 3200 microsteps per rev)


// Pin config section
#define pwrSwitch -1 // For toggling power button within the EMC application window.

#define microSteppingPins 3 // Number of microstepping pins for each channel. (Set to 0 to disable.)
#define minStepTime 25 // in MICRO seconds.
#define chanXms1 45
#define chanXms2 47
#define chanXms3 49
#define chanYms1 44
#define chanYms2 46
#define chanYms3 48
#define chanZms1 31
#define chanZms2 29
#define chanZms3 27
#define chanAms1 30
#define chanAms2 28
#define chanAms3 26

#define xDirPin 43
#define yDirPin 42
#define zDirPin 33
#define aDirPin 32

#define xEnablePin 38
#define yEnablePin 39
#define zEnablePin 37
#define aEnablePin 36

#define xStepPin 41
#define yStepPin 40
#define zStepPin 35
#define aStepPin 34

#define xSleepPin 41
#define ySleepPin 41
#define zSleepPin 41
#define aSleepPin 41

#define xResetPin 51
#define yResetPin 50
#define zResetPin 25
#define aResetPin 24

  // pin config for the "Official Aruino Motor Shield"

#define loadX A0 // not used. Yet?
#define loadY A1 // not used. Yet?
#define pwmX 3
#define pwmY 11
#define xPowerOutput 150 // Aright Scotty. How fast you wanna go. Too much = noisy,fuzzy (but interesting) drawings
#define yPowerOutput 150 //  and a hot driver circuit,  Too little = weak, wobbly or no movement at all. Max.value=255
#define xOverdrivePwr 255 // Reffer to the l_variables tab. (Section: Servo options)
#define yOverdrivePwr 255
#define dirX 12
#define dirY 13
#define enableX 9
#define enableY 8
#define zSolenoid -1

  // limit switches
#define xHomePin 7
#define yHomePin 6
#define xHomePos -11200
#define yHomePos -3250

  // connection keep alive. (in milliseconds)
# define idleTime 1000

  // lcd config
#define lcdColumns 8
#define lcdRows 2
#define lcdRS -1
#define lcdEN -1
#define lcdD4 -1
#define lcdD5 -1
#define lcdD6 -1
#define lcdD7 -1

  // ATX psu control
#define atxPowerCtrl -1     // Usually the PSU's green wire. (We bring this LOW to turn PSU on.)
#define atxPowerState -1     // Usually the PSU's gray wire. (Use a diode, ie. Arduino --|<|-- PSU, else lights blink when PSU sleeps.)
#define atxPowerLed -1       // Just an output led pin for visual feedback.
#define atxPowerTimeout 1000 // How long to wait before shutting down once EMC has been closed. (milliSeconds) 

//Libraries to include are as follows:
#include <LiquidCrystal.h>     // Standard issue arduino library, most likely you already have this one.
#include <digitalWriteFast.h>  // Found at http://code.google.com/p/digitalwritefast/downloads/list get the safe one.

