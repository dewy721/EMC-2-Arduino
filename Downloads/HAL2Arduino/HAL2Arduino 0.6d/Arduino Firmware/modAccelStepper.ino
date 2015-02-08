#define useAccelStepper true

// Requires the AccelStepper Library 1.31 or higher.
// Available at http://www.open.com.au/mikem/arduino/AccelStepper

#if useAccelStepper
  #include <AccelStepper.h> // load the library, if needed.

  #define enablePins true // Some drivers have dedicated 'enable' pins.
                          //If you don't need any, set to false and skip the enablePins section.

  #define pinInversion true // If NONE are needed, set to false and skip the pinInversion section.

  #define stepperX true // enable to bind this stepper to axis0 (X axis).
  #define stepperY true // enable to bind this stepper to axis1 (Y axis).
  #define stepperZ true // enable to bind this stepper to axis2 (Z axis).
  #define stepperA true // enable to bind this stepper to axis3 (A axis).
  #define stepperB true // enable to bind this stepper to axis4 (B axis).
  #define stepperC false // enable to bind this stepper to axis5 (C axis).
  #define stepperU false // enable to bind this stepper to axis6 (U axis).
  #define stepperV false // enable to bind this stepper to axis7 (V axis).
  #define stepperW false // enable to bind this stepper to axis8 (W axis).
  
  // Set your steps per unit of measure, Inch/mm/deg.
  #define stepsPerUnitX 2000 //8000
  #define stepsPerUnitY 2000 //8333.333
  #define stepsPerUnitZ 2000 //8000
  #define stepsPerUnitA 45
  #define stepsPerUnitB 45
  #define stepsPerUnitC 0
  #define stepsPerUnitU 0
  #define stepsPerUnitV 0
  #define stepsPerUnitW 0

  // Max speed in units per second. Speed will also be influenced by host.
  #define unitsPerSecondX 1 //0.12
  #define unitsPerSecondY 1 //0.12
  #define unitsPerSecondZ 1 //0.12
  #define unitsPerSecondA 54
  #define unitsPerSecondB 54
  #define unitsPerSecondC 0
  #define unitsPerSecondU 0
  #define unitsPerSecondV 0
  #define unitsPerSecondW 0

  // Max acceleration in units per second.
  #define accelPerSecondX 5 //0.36
  #define accelPerSecondY 5 //0.36
  #define accelPerSecondZ 5 //0.36
  #define accelPerSecondA 300
  #define accelPerSecondB 300
  #define accelPerSecondC 0
  #define accelPerSecondU 0
  #define accelPerSecondV 0
  #define accelPerSecondW 0

  #define leadTime 200 // Velocity lead time compensation in Millisecs.

  #define useVectors false // Beta, On demand clientside prediction. (Bad for rounded trajectories.)

  #if useVectors
    boolean vectorEnable;
    int steppersInMotion;
  #endif

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
      AccelStepper stprB(1, A0, A1);
    #endif
    #if stepperC
      AccelStepper stprC(1, 0, 0);
    #endif
    #if stepperU
      AccelStepper stprU(1, 0, 0);
    #endif
    #if stepperV
      AccelStepper stprV(1, 0, 0);
    #endif
    #if stepperW
      AccelStepper stprW(1, 0, 0);
    #endif

float velocity[9]={unitsPerSecondX,unitsPerSecondY,unitsPerSecondZ,unitsPerSecondA,unitsPerSecondB,unitsPerSecondC,unitsPerSecondU,unitsPerSecondV,unitsPerSecondW};
float unitsPerSecond[9]={unitsPerSecondX,unitsPerSecondY,unitsPerSecondZ,unitsPerSecondA,unitsPerSecondB,unitsPerSecondC,unitsPerSecondU,unitsPerSecondV,unitsPerSecondW};
float stepsPerUnit[9]={stepsPerUnitX,stepsPerUnitY,stepsPerUnitZ,stepsPerUnitA,stepsPerUnitB,stepsPerUnitC,stepsPerUnitU,stepsPerUnitV,stepsPerUnitW};
long currentPos[9];
unsigned long deltaTimeOld[10];
long deltaPosOld[10];
long bestSpeed[9]; 
  void stepperSetup(){
    for(int i=0;i<9;i++){
      bestSpeed[i]=unitsPerSecond[i]*stepsPerUnit[i];
    }
    for(int i=36;i<40;i++){
      pinMode(i,OUTPUT);
      digitalWrite(i,LOW);
    }
    for(int i=0;i<10;i++){
      deltaPosOld[i]=0;
      deltaTimeOld[i]=millis();
    }

///// pinInversion section /////
  #if pinInversion
 // example: stepper#.setPinsInverted(step, direction, enable);
    #if stepperX
      stprX.setPinsInverted(false, true, true);
    #endif
    #if stepperY
      stprY.setPinsInverted(false, true, true);
    #endif
    #if stepperZ
      stprZ.setPinsInverted(false, true, true);
    #endif
    #if stepperA
      stprA.setPinsInverted(false, true, true);
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
      stprX.setEnablePin(10);
    #endif
    #if stepperY
      stprY.setEnablePin(11);
    #endif
    #if stepperZ
      stprZ.setEnablePin(12);
    #endif
    #if stepperA
      stprA.setEnablePin(13);
    #endif
    #if stepperB
      stprB.setEnablePin(35);
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
    stprX.setMaxSpeed(unitsPerSecondX*stepsPerUnitX);
    stprX.setAcceleration(accelPerSecondX*stepsPerUnitX*2);
    stprX.enableOutputs();
    stprX.setMinPulseWidth(5);
  #endif
  #if stepperY
    stprY.setMaxSpeed(unitsPerSecondY*stepsPerUnitY);
    stprY.setAcceleration(accelPerSecondX*stepsPerUnitY*2);
    stprY.enableOutputs();
    stprY.setMinPulseWidth(5);
  #endif
  #if stepperZ
    stprZ.setMaxSpeed(unitsPerSecondZ*stepsPerUnitZ);
    stprZ.setAcceleration(accelPerSecondX*stepsPerUnitZ*2);
    stprZ.enableOutputs();
    stprZ.setMinPulseWidth(5);
  #endif
  #if stepperA
    stprA.setMaxSpeed(unitsPerSecondA*stepsPerUnitA);
    stprA.setAcceleration(accelPerSecondX*stepsPerUnitA*2);
    stprA.enableOutputs();
    stprA.setMinPulseWidth(5);
  #endif
  #if stepperB
    stprB.setMaxSpeed(unitsPerSecondB*stepsPerUnitB);
    stprB.setAcceleration(accelPerSecondX*stepsPerUnitB*2);
    stprB.enableOutputs();
    stprB.setMinPulseWidth(5);
  #endif
  #if stepperC
    stprC.setMaxSpeed(unitsPerSecondC*stepsPerUnitC);
    stprC.setAcceleration(accelPerSecondX*stepsPerUnitC*2);
    stprC.enableOutputs();
    stprC.setMinPulseWidth(1);
  #endif
  #if stepperU
    stprU.setMaxSpeed(unitsPerSecondU*stpsPrInU);
    stprU.setAcceleration(accelPerSecondX*stepsPerUnitU*2);
    stprU.enableOutputs();
    stprU.setMinPulseWidth(1);
  #endif
  #if stepperV
    stprV.setMaxSpeed(unitsPerSecondV*stpsPrInV);
    stprV.setAcceleration(accelPerSecondX*stepsPerUnitV*2);
    stprV.enableOutputs();
    stprV.setMinPulseWidth(1);
  #endif
  #if stepperW
    stprW.setMaxSpeed(unitsPerSecondW*stpsPrInW);
    stprW.setAcceleration(accelPerSecondX*stepsPerUnitW*2);
    stprW.enableOutputs();
    stprW.setMinPulseWidth(1);
  #endif
  
  #define enableSlpRstMicroStepCode false // Depreciated
  #if enableSlpRstMicroStepCode
    //Sparkfun's Quadstepper Specific settings.
    // (Ie: header pins used to directly plug a quadstepper into Mega2560/Due pins 22~35, 40~53)
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

  #define lookback 5 // calc av
  float avgDist[9];
  unsigned long getAvgDist(int i){
    unsigned long tempDist=0;
    for(int i=0;i<lookback;i++){
      tempDist+=(avgDist[i]/100);;
    }
    avgDist[i]=(getDist(currentPos[i],targetPos[i])+tempDist)/lookback;
    return avgDist[i];
  }
  unsigned long velocityTimer[9];
  #define distMult 1
  void runSteppers(){
    if(canMove){
      //for(int i=0;bestSpeed[i]=0;i<9;i++){getAvgDist(i);}
      int i;
      long delta;
      long reqSpeed;
      unsigned long curTime=millis();
      long curDist=0;
      #if useVectors
        steppersInMotion=0;
      #endif
      #if stepperX
        i=0;bestSpeed[i]=0;
        stprX.run();
        currentPos[i]=stprX.currentPosition();
        if(curTime-velocityTimer[i]>leadTime){
          reqSpeed=constrain(map(velocity[i],0,unitsPerSecond[i]*stepsPerUnit[i],getDist(targetPos[i],currentPos[i]),unitsPerSecond[i]*stepsPerUnit[i]),0,unitsPerSecond[i]*stepsPerUnit[i]);
          delta=getDelta(i,targetPos[i]);
          if(reqSpeed>bestSpeed[i]){bestSpeed[i]=reqSpeed;}
          curDist=stprX.distanceToGo()*distMult;
          if(delta>bestSpeed[i]){bestSpeed[i]=delta;}
          if(curDist>avgDist[i]){if(avgDist[i]<100000){avgDist[i]++;}}else{if(avgDist[i]>0){avgDist[i]--;}}
          if(avgDist[i]>bestSpeed[i]){bestSpeed[i]+=(avgDist[i]/100);;}
          stprX.setMaxSpeed(constrain(bestSpeed[i],0,unitsPerSecond[i]*stepsPerUnit[i]));
          velocityTimer[i]=curTime;
        }
        if(stprX.distanceToGo()==0){avgDist[i]=0;} //X.setMaxSpeed(unitsPerSecond[i]*stepsPerUnit[i]);}
      #endif
      #if useVectors
        if(stprX.distanceToGo()>0){steppersInMotion++;}
      #endif
      #if stepperY
        i=1;bestSpeed[i]=0;
        stprY.run();
        currentPos[i]=stprY.currentPosition();
        if(curTime-velocityTimer[i]>leadTime){
          reqSpeed=constrain(map(velocity[i],0,unitsPerSecond[i]*stepsPerUnit[i],getDist(targetPos[i],currentPos[i]),unitsPerSecond[i]*stepsPerUnit[i]),0,unitsPerSecond[i]*stepsPerUnit[i]);
          delta=getDelta(i,targetPos[i]);
          if(reqSpeed>bestSpeed[i]){bestSpeed[i]=reqSpeed;}
          curDist=stprY.distanceToGo()*distMult;
          if(delta>bestSpeed[i]){bestSpeed[i]=delta;}
          if(curDist>avgDist[i]){if(avgDist[i]<100000){avgDist[i]++;}}else{if(avgDist[i]>0){avgDist[i]--;}}
          if(avgDist[i]>bestSpeed[i]){bestSpeed[i]+=(avgDist[i]/100);;}
          stprY.setMaxSpeed(constrain(bestSpeed[i],0,unitsPerSecond[i]*stepsPerUnit[i]));
          velocityTimer[i]=curTime;
        }
        if(stprY.distanceToGo()==0){avgDist[i]=0;} //Y.setMaxSpeed(unitsPerSecond[i]*stepsPerUnit[i]);}
      #endif
      #if useVectors
        if(stprY.distanceToGo()>0){steppersInMotion++;}
      #endif
      #if stepperZ
        i=2;bestSpeed[i]=0;
        stprZ.run();
        currentPos[i]=stprZ.currentPosition();
        if(curTime-velocityTimer[i]>leadTime){
          reqSpeed=constrain(map(velocity[i],0,unitsPerSecond[i]*stepsPerUnit[i],getDist(targetPos[i],currentPos[i]),unitsPerSecond[i]*stepsPerUnit[i]),0,unitsPerSecond[i]*stepsPerUnit[i]);
          delta=getDelta(i,targetPos[i]);
          if(reqSpeed>bestSpeed[i]){bestSpeed[i]=reqSpeed;}
          curDist=stprZ.distanceToGo()*distMult;
          if(delta>bestSpeed[i]){bestSpeed[i]=delta;}
          if(curDist>avgDist[i]){if(avgDist[i]<100000){avgDist[i]++;}}else{if(avgDist[i]>0){avgDist[i]--;}}
          if(avgDist[i]>bestSpeed[i]){bestSpeed[i]+=(avgDist[i]/100);;}
          stprZ.setMaxSpeed(constrain(bestSpeed[i],0,unitsPerSecond[i]*stepsPerUnit[i]));
          velocityTimer[i]=curTime;
        }
        if(stprZ.distanceToGo()==0){avgDist[i]=0;} //Z.setMaxSpeed(unitsPerSecond[i]*stepsPerUnit[i]);}
      #endif
      #if useVectors
        if(stprZ.distanceToGo()>0){steppersInMotion++;}
      #endif
      #if stepperA
        i=3;bestSpeed[i]=0;
        stprA.run();
        currentPos[i]=stprA.currentPosition();
        if(curTime-velocityTimer[i]>leadTime){
          reqSpeed=constrain(map(velocity[i],0,unitsPerSecond[i]*stepsPerUnit[i],getDist(targetPos[i],currentPos[i]),unitsPerSecond[i]*stepsPerUnit[i]),0,unitsPerSecond[i]*stepsPerUnit[i]);
          delta=getDelta(i,targetPos[i]);
          if(reqSpeed>bestSpeed[i]){bestSpeed[i]=reqSpeed;}
          curDist=stprA.distanceToGo()*distMult;
          if(delta>bestSpeed[i]){bestSpeed[i]=delta;}
          if(curDist>avgDist[i]){if(avgDist[i]<100000){avgDist[i]++;}}else{if(avgDist[i]>0){avgDist[i]--;}}
          if(avgDist[i]>bestSpeed[i]){bestSpeed[i]+=(avgDist[i]/100);;}
          stprA.setMaxSpeed(constrain(bestSpeed[i],0,unitsPerSecond[i]*stepsPerUnit[i]));
          velocityTimer[i]=curTime;
        }
        if(stprA.distanceToGo()==0){avgDist[i]=0;} //A.setMaxSpeed(unitsPerSecond[i]*stepsPerUnit[i]);}
      #endif
      #if useVectors
        if(stprA.distanceToGo()>0){steppersInMotion++;}
      #endif
      #if stepperB
        i=4;bestSpeed[i]=0;
        stprB.run();
        currentPos[i]=stprB.currentPosition();
        if(curTime-velocityTimer[i]>leadTime){
          reqSpeed=constrain(map(velocity[i],0,unitsPerSecond[i]*stepsPerUnit[i],getDist(targetPos[i],currentPos[i]),unitsPerSecond[i]*stepsPerUnit[i]),0,unitsPerSecond[i]*stepsPerUnit[i]);
          delta=getDelta(i,targetPos[i]);
          if(reqSpeed>bestSpeed[i]){bestSpeed[i]=reqSpeed;}
          curDist=stprB.distanceToGo()*distMult;
          if(delta>bestSpeed[i]){bestSpeed[i]=delta;}
          if(curDist>avgDist[i]){if(avgDist[i]<100000){avgDist[i]++;}}else{if(avgDist[i]>0){avgDist[i]--;}}
          if(avgDist[i]>bestSpeed[i]){bestSpeed[i]+=(avgDist[i]/100);;}
          stprB.setMaxSpeed(constrain(bestSpeed[i],0,unitsPerSecond[i]*stepsPerUnit[i]));
          velocityTimer[i]=curTime;
        }
        if(stprB.distanceToGo()==0){avgDist[i]=0;} //B.setMaxSpeed(unitsPerSecond[i]*stepsPerUnit[i]);}
      #endif
      #if useVectors
        if(stprB.distanceToGo()>0){steppersInMotion++;}
      #endif
      #if stepperC
        i=5;bestSpeed[i]=0;
        stprC.run();
        currentPos[i]=stprC.currentPosition();
        if(curTime-velocityTimer[i]>leadTime){
          reqSpeed=constrain(map(velocity[i],0,unitsPerSecond[i]*stepsPerUnit[i],getDist(targetPos[i],currentPos[i]),unitsPerSecond[i]*stepsPerUnit[i]),0,unitsPerSecond[i]*stepsPerUnit[i]);
          delta=getDelta(i,targetPos[i]);
          if(reqSpeed>bestSpeed[i]){bestSpeed[i]=reqSpeed;}
          curDist=stprC.distanceToGo()*distMult;
          if(delta>bestSpeed[i]){bestSpeed[i]=delta;}
          if(curDist>avgDist[i]){if(avgDist[i]<100000){avgDist[i]++;}}else{if(avgDist[i]>0){avgDist[i]--;}}
          if(avgDist[i]>bestSpeed[i]){bestSpeed[i]+=(avgDist[i]/100);;}
          stprC.setMaxSpeed(constrain(bestSpeed[i],0,unitsPerSecond[i]*stepsPerUnit[i]));
          velocityTimer[i]=curTime;
        }
        if(stprC.distanceToGo()==0){avgDist[i]=0;} //C.setMaxSpeed(unitsPerSecond[i]*stepsPerUnit[i]);}
        #if useVectors
          if(stprC.distanceToGo()>0){steppersInMotion++;}
        #endif
      #endif
      #if stepperU
        i=6;bestSpeed[i]=0;
        stprU.run();
        currentPos[i]=stprU.currentPosition();
        if(curTime-velocityTimer[i]>leadTime){
          reqSpeed=constrain(map(velocity[i],0,unitsPerSecond[i]*stepsPerUnit[i],getDist(targetPos[i],currentPos[i]),unitsPerSecond[i]*stepsPerUnit[i]),0,unitsPerSecond[i]*stepsPerUnit[i]);
          delta=getDelta(i,targetPos[i]);
          if(reqSpeed>bestSpeed[i]){bestSpeed[i]=reqSpeed;}
          curDist=stprU.distanceToGo()*distMult;
          if(delta>bestSpeed[i]){bestSpeed[i]=delta;}
          if(curDist>avgDist[i]){if(avgDist[i]<100000){avgDist[i]++;}}else{if(avgDist[i]>0){avgDist[i]--;}}
          if(avgDist[i]>bestSpeed[i]){bestSpeed[i]+=(avgDist[i]/100);;}
          stprU.setMaxSpeed(constrain(bestSpeed[i],0,unitsPerSecond[i]*stepsPerUnit[i]));
          velocityTimer[i]=curTime;
        }
        if(stprU.distanceToGo()==0){avgDist[i]=0;} //U.setMaxSpeed(unitsPerSecond[i]*stepsPerUnit[i]);}
        #if useVectors
          if(stprU.distanceToGo()>0){steppersInMotion++;}
        #endif
      #endif
      #if stepperV
        i=7;bestSpeed[i]=0;
        stprV.run();
        currentPos[i]=stprV.currentPosition();
        if(curTime-velocityTimer[i]>leadTime){
          reqSpeed=constrain(map(velocity[i],0,unitsPerSecond[i]*stepsPerUnit[i],getDist(targetPos[i],currentPos[i]),unitsPerSecond[i]*stepsPerUnit[i]),0,unitsPerSecond[i]*stepsPerUnit[i]);
          delta=getDelta(i,targetPos[i]);
          if(reqSpeed>bestSpeed[i]){bestSpeed[i]=reqSpeed;}
          curDist=stprV.distanceToGo()*distMult;
          if(delta>bestSpeed[i]){bestSpeed[i]=delta;}
          if(curDist>avgDist[i]){if(avgDist[i]<100000){avgDist[i]++;}}else{if(avgDist[i]>0){avgDist[i]--;}}
          if(avgDist[i]>bestSpeed[i]){bestSpeed[i]+=(avgDist[i]/100);;}
          stprV.setMaxSpeed(constrain(bestSpeed[i],0,unitsPerSecond[i]*stepsPerUnit[i]));
          velocityTimer[i]=curTime;
        }
        if(stprV.distanceToGo()==0){avgDist[i]=0;} //V.setMaxSpeed(unitsPerSecond[i]*stepsPerUnit[i]);}
        #if useVectors
          if(stprV.distanceToGo()>0){steppersInMotion++;}
        #endif
      #endif
      #if stepperW
        i=8;bestSpeed[i]=0;
        stprW.run();
        currentPos[8]=stprW.currentPosition();
        if(curTime-velocityTimer[i]>leadTime){
          reqSpeed=constrain(map(velocity[i],0,unitsPerSecond[i]*stepsPerUnit[i],getDist(targetPos[i],currentPos[i]),unitsPerSecond[i]*stepsPerUnit[i]),0,unitsPerSecond[i]*stepsPerUnit[i]);
          delta=getDelta(i,targetPos[i]);
          if(reqSpeed>bestSpeed[i]){bestSpeed[i]=reqSpeed;}
          curDist=stprW.distanceToGo()*distMult;
          if(delta>bestSpeed[i]){bestSpeed[i]=delta;}
          if(curDist>avgDist[i]){if(avgDist[i]<100000){avgDist[i]++;}}else{if(avgDist[i]>0){avgDist[i]--;}}
          if(avgDist[i]>bestSpeed[i]){bestSpeed[i]+=(avgDist[i]/100);;}
          stprW.setMaxSpeed(constrain(bestSpeed[i],0,unitsPerSecond[i]*stepsPerUnit[i]));
          velocityTimer[i]=curTime;
        }
        if(stprW.distanceToGo()==0){avgDist[i]=0;} //W.setMaxSpeed(unitsPerSecond[i]*stepsPerUnit[i]);}
        #if useVectors
          if(stprW.distanceToGo()>0){steppersInMotion++;}
        #endif
      #endif
      #if useVectors
       if(steppersInMotion>1){
         vectorEnable=true;
       }else{
         vectorEnable=false;
       }
      #endif
    }
  }

    float deltaOld[9];
    
    unsigned long getDelta(int axis,long pos)
    {
      unsigned long time=millis();
      unsigned long deltaTime=time-deltaTimeOld[axis];
      long delta;
      if(pos<deltaPosOld[axis]){delta=deltaPosOld[axis]-pos;}else{delta=pos-deltaPosOld[axis];}
      if(delta<0){delta*=-1;}
      if(pos==deltaPosOld[axis]){delta=0;}
      deltaTimeOld[axis]=time;
      deltaPosOld[axis]=pos;
      long deltaRet=(delta/deltaTime)*1000;
      if(deltaRet>0){
        return deltaRet; // rate of change per second.
      }else{
        return 0;
      }
    }

  #if useVectors
    
    long getVector(int axis,long pos){
      if(vectorEnable){
        long heading=pos-targetPosOld[axis];
        long vector=pos+(heading*steppersInMotion);
        return vector;
      }else{
        return pos;
      }
    }
  #endif
#endif

