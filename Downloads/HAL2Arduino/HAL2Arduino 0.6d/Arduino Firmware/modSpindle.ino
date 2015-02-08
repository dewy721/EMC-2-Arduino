#define useSpindle false // load spindle control code.

#if useSpindle
  // We'll need some info to calc with.
  #define spindleMinRpm 0
  #define spindleMaxRpm 3260
  
  #define spindleDriveType pwm //pwm,bool,vfd
      //if pwm: set min/max values.
      #define spindleMinPwm 50 // Set this just high enough to creep reliably.
      #define spindleMaxPwm 255
      
      //if boolean: Just use the spindleEnablePin for simple on/off control. 
      #define spindleStartupTime 1000 //millisec
      
      //if vfd: (variable frequency drive) set min/max freq.
      // Note: vfd uses tone() function, this will effect pwm pins 3 and 11 on smaller sub-Mega boards.
      // Arduino DUE does not officially support tone() yet.
      #define spindleMinFreq 1
      #define spindleMaxFreq 20000

  // Pins: By default ALL usable UNO pins are assigned, just as an example.
  const byte spindleEnablePin=2;
  const byte spindleBrakePin=3;
  const byte spindleDirPin=4;
  const byte spindleThrottlePin=5; // pwm or vfd signal.
  const byte spindleTachPin=0; // Also used as index pin.
  
  // AVR or Due interrupts? (Since they're handled differently.)
  #define ArduinoDueInterrupts false
  
  // Some variables.
  boolean spindleBrake=1;
  boolean spindleSetDir=0;
  boolean spindleEnabled=0;
  long spindleSetRpm=0;
  boolean sendSpindleIndexSignal=0;
  volatile long rawTachData=0;
  unsigned long tachTime=millis();
  void updateSpindle(){
    if(spindleEnablePin>0){ // If there is a spindle setup for this axis...
      digitalWrite(spindleBrakePin,spindleBrake);
      digitalWrite(spindleDirPin,spindleSetDir);
      updateSpindleSpeed(spindleSetRpm);
      if(!spindleBrake){ // And finally if the brake is NOT on, spindle is allowed move.
        digitalWrite(spindleEnablePin,spindleEnabled);
      }
    }
  }
  
  byte oldTonePin=-1;
  void updateSpindleSpeed(long rpm){
    #if spindleDriveType == pwm
      byte output=constrain(map(rpm,spindleMinRpm,spindleMaxRpm,spindleMinPwm,spindleMaxPwm),spindleMinPwm,spindleMaxPwm);
      analogWrite(spindleThrottlePin,output);
    #endif
    #if spindleDriveType == boolean
      // Do nothing since it was already handled by the spindleEnablePin.
    #endif
    #if spindleDriveType == vfd
      long outputFreq=constrain(map(rpm,spindleMinRpm,spindleMaxRpm,spindleMinFreq,spindleMaxFreq),spindleMinFreq,spindleMaxFreq);
      noTone(spindleThrottlePin); // First we have to cancel the old tone.
      tone(spindleThrottlePin,outputFreq); // set new tone.
    #endif
  }

  void getSpindleSpeed(){
    if(spindleTachPin>0){
      unsigned long time=millis();
      unsigned long timeLapsed=time - tachTime;
      if(timeLapsed >= 1000){
        unsigned long rps=rawTachData;
        rawTachData=0; // Reset the raw data for next reading.
        tachTime=time; // Set next time to re-read data.
        #if useDueNativePort
          SerialUSB.print("135 0 "); // 135  motion.spindle-revs
          SerialUSB.print(rps); // current rps (revolutions per second)
          SerialUSB.print(" ");
          SerialUSB.print(135+rps); // Calculate the checksum.
          SerialUSB.println(";");
        #else
          Serial.print("135 0 "); // 135  motion.spindle-revs
          Serial.print(rps); // current rps (revolutions per second)
          Serial.print(" ");
          Serial.print(135+rps); // Calculate the checksum.
          Serial.println(";");
        #endif
      }
    }
  }
  
  void sendSpindleIndexPulse(byte axis){ // Low speed signaling ie: <600rpm
    if(sendSpindleIndexSignal)
#endif
