#define useAccelStepper true

// Requires the AccelStepper Library 1.30 or higher.
// Available at http://www.open.com.au/mikem/arduino/AccelStepper/AccelStepper-1.30.zip

#if useAccelStepper
  #define numAxises 3 // How many axises do you have?

  #define enablePins true // Some drivers have dedicated 'enable' pins.
                          //If you don't need any, set to false and skip the enablePins section.

  #define pinInversion true // If NONE are needed, set to false and skip the pinInversion section.

  #define stepperX true // enable to bind this stepper to axis0 (X axis).
  #define stepperY true // enable to bind this stepper to axis1 (Y axis).
  #define stepperZ true // enable to bind this stepper to axis2 (Z axis).
  #define stepperA false // enable to bind this stepper to axis3 (A axis).
  #define stepperB false // enable to bind this stepper to axis4 (B axis).
  #define stepperC false // enable to bind this stepper to axis5 (C axis).
  #define stepperU false // enable to bind this stepper to axis6 (U axis).
  #define stepperV false // enable to bind this stepper to axis7 (V axis).
  #define stepperW false // enable to bind this stepper to axis8 (W axis).
  
  // Set your steps per unit of measure, inch/mm. (labels just say Inch, but it really should say 'Unit')
  #define stepsPerInchX 200
  #define stepsPerInchY 200
  #define stepsPerInchZ 200
  #define stepsPerInchA 200
  #define stepsPerInchB 200
  #define stepsPerInchC 200
  #define stepsPerInchU 200
  #define stepsPerInchV 200
  #define stepsPerInchW 200

  #include <AccelStepper.h> // load the library.
  
  // Define some steppers and the pins they will use.
  // Read http://www.open.com.au/mikem/arduino/AccelStepper/
  // for documentation on how to setup different kinds of steppers.
    #if stepperX
      AccelStepper stprX(1, 41, 43); //type, stepPin, dirPin
    #endif
    #if stepperY
      AccelStepper stprY(1, 40, 42); 
    #endif
    #if stepperZ
      AccelStepper stprZ(1, 35, 33);
    #endif
    #if stepperA
      AccelStepper stprA(1, 34, 32);
    #endif
    #if stepperB
      AccelStepper stprB(1, 99, 99);
    #endif
    #if stepperC
      AccelStepper stprC(1, 99, 99);
    #endif
    #if stepperU
      AccelStepper stprU(1, 99, 99);
    #endif
    #if stepperV
      AccelStepper stprV(1, 99, 99);
    #endif
    #if stepperW
      AccelStepper stprW(1, 99, 99);
    #endif

///Doing some math here. Just skip past it.
  #if stepperX
    float stpsPrInX=10000.0/stepsPerInchX;
  #endif
  #if stepperY
    float stpsPrInY=10000.0/stepsPerInchY;
  #endif
  #if stepperZ
    float stpsPrInZ=10000.0/stepsPerInchZ;
  #endif
  #if stepperA
    float stpsPrInA=10000.0/stepsPerInchA;
  #endif
  #if stepperB
    float stpsPrInB=10000.0/stepsPerInchB;
  #endif
  #if stepperC
    float stpsPrInC=10000.0/stepsPerInchC;
  #endif
  #if stepperU
    float stpsPrInU=10000.0/stepsPerInchU;
  #endif
  #if stepperV
    float stpsPrInV=10000.0/stepsPerInchV;
  #endif
  #if stepperW
    float stpsPrInW=10000.0/stepsPerInchW;
  #endif


 
  void stepperSetup(){
    for(int i=2;i<6;i++){
      pinMode(i,OUTPUT);
      digitalWrite(i,LOW);
    }

///// pinInversion section /////
  #if pinInversion
 // example: stepper#.setPinsInverted(step, direction, enable);
    #if stepperX
      stprX.setPinsInverted(false, false, true);
    #endif
    #if stepperY
      stprY.setPinsInverted(false, false, true);
    #endif
    #if stepperZ
      stprZ.setPinsInverted(false, false, true);
    #endif
    #if stepperA
      stprA.setPinsInverted(false, false, true);
    #endif
    #if stepperB
      stprB.setPinsInverted(false, false, false);
    #endif
    #if stepperC
      stprC.setPinsInverted(false, false, false);
    #endif
    #if stepperU
      stprU.setPinsInverted(false, false, false);
    #endif
    #if stepperV
      stprV.setPinsInverted(false, false, false);
    #endif
    #if stepperW
      stprW.setPinsInverted(false, false, false);
    #endif
  #endif
///// enablePins section /////
  #if enablePins
  // example: stepper#.setEnablePin(q); change 'q' to the enable pin for this axis.
    #if stepperX
      stprX.setEnablePin(2);
    #endif
    #if stepperY
      stprY.setEnablePin(3);
    #endif
    #if stepperZ
      stprZ.setEnablePin(5);
    #endif
    #if stepperA
      stprA.setEnablePin(4);
    #endif
    #if stepperB
      stprB.setEnablePin(0);
    #endif
    #if stepperC
      stprC.setEnablePin(0);
    #endif
    #if stepperU
      stprU.setEnablePin(0);
    #endif
    #if stepperV
      stprV.setEnablePin(0);
    #endif
    #if stepperW
      stprW.setEnablePin(0);
    #endif
  #endif
  
  #if stepperX
    stprX.setMaxSpeed(3.1333*stepsPerInchX);
    stprX.setAcceleration(10.0*stepsPerInchX);
  #endif
  #if stepperY
    stprY.setMaxSpeed(3.1333*stepsPerInchY);
    stprY.setAcceleration(10.0*stepsPerInchY);
  #endif
  #if stepperZ
    stprZ.setMaxSpeed(3.1333*stepsPerInchZ);
    stprZ.setAcceleration(10.0*stepsPerInchZ);
  #endif
  #if stepperA
    stprA.setMaxSpeed(3.1333*stepsPerInchA);
    stprA.setAcceleration(10.0*stpsPrInA);
  #endif
  #if stepperB
    stprB.setMaxSpeed(3.1333*stepsPerInchB);
    stprB.setAcceleration(10.0*stepsPerInchB);
  #endif
  #if stepperC
    stprC.setMaxSpeed(3.1333*stepsPerInchC);
    stprC.setAcceleration(10.0*stepsPerInchC);
  #endif
  #if stepperU
    stprU.setMaxSpeed(3.1333*stpsPrInU);
    stprU.setAcceleration(10.0*stepsPerInchU);
  #endif
  #if stepperV
    stprV.setMaxSpeed(3.1333*stpsPrInV);
    stprV.setAcceleration(10.0*stepsPerInchV);
  #endif
  #if stepperW
    stprW.setMaxSpeed(3.1333*stpsPrInW);
    stprW.setAcceleration(10.0*stepsPerInchW);
  #endif
  
  #define enableSlpRstMicroStepCode true
  #if enableSlpRstMicroStepCode
    //Sparkfun's Quadstepper Specific settings.
    // (Ie: header pins used to directly plug a quadstepper into Mega2560 pins 22~35, 40~53)
    //set SLP/RST pins
    for(int i=22;i<26;i++){
      pinMode(i,OUTPUT);
      digitalWrite(i,HIGH);
      pinMode(i+28,OUTPUT);
      digitalWrite(i+28,HIGH);
    }
    
    //set MS1,MS2,MS3 pins (set stepping mode)
    for(int i=26;i<32;i++){
      pinMode(i,OUTPUT);
      digitalWrite(i,LOW);
      pinMode(i+18,OUTPUT);
      digitalWrite(i+18,LOW);
    }
    //end of////////TEMPORARY CODE
  #endif
  }

  void runSteppers(){
    if(canMove){
      #if stepperX
        stprX.run();
      #endif
      #if stepperY
        stprY.run();
      #endif
      #if stepperZ
        stprZ.run();
      #endif
      #if stepperA
        stprA.run();
      #endif
      #if stepperB
        stprB.run();
      #endif
      #if stepperC
        stprC.run();
      #endif
      #if stepperU
        stprU.run();
      #endif
      #if stepperV
        stprV.run();
      #endif
      #if stepperW
        stprW.run();
      #endif
    }
  }

#endif

