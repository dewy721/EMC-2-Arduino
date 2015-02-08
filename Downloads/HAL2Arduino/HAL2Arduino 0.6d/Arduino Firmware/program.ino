// That's it your done. :)
// This is the program that will actually run things.
// There should be no need to modify it.


void setup()
{
  #if useDueNativePort
    #define BUF_N 10001
    char buf[BUF_N];
    SerialUSB.begin(BAUD);
    memset(buf, 'A', BUF_N-1);
    buf[BUF_N-1] = '\0';
    while(!SerialUSB){
      delay(400);
    }
  #else
    while(!Serial){
      delay(400);
    }
    Serial.begin(BAUD);
  #endif
  #if modLcd
    lcdSetup();
  #endif
  #if useAccelStepper
    stepperSetup();
  #endif
  #if useEncoder
    encoderSetup();
  #endif
  #if useEncoderServo
    encoderServoSetup();
  #endif
  #if useEthernet
    Ethernet.begin(mac,ip);
    udp.begin(localPort);
  #endif
  #if useDueNativePort
    SerialUSB.setTimeout(statementTimeout);
    SerialUSB.println("ok");
  #else
    Serial.setTimeout(statementTimeout);
    Serial.println("ok");
  #endif
  for(int i=0;i<9;i++){
    targetPos[i]=0;
    targetPosOld[i]=0;
    #if autoSpeedAdjust
        deltaOld[i]=1;
    #endif
  }
}


long checksumOld=0;
boolean debug=false;

void processCommand(long command, long dataOne, long dataTwo)
{
  if(command + dataOne + dataTwo != checksumOld){
    if(command > 99 && command < 200){
      // This is a command from the "motion" catagory.
      #if useSpindle
        if(command == 130){ // motion.spindle-brake
          spindleBrake=dataTwo;
        }
        if(command == 131){ // motion.spindle-forward
          spindleSetDir=dataTwo;
        }
      #if useEncoder
        if(command == 132){ // motion.spindle-index-enable
          sendSpindleIndexSignal=dataTwo;
//          spindleIndexEnabled[dataOne]=dataTwo;
        }
      #endif
        if(command == 133){ // motion.spindle-on
          spindleEnabled=dataTwo;
        }
        if(command == 134){ // motion.spindle-reverse
          spindleSetDir=!dataTwo;
        }
        if(command == 137){ // motion.spindle-speed-out
          spindleSetRpm=dataTwo;
        }
        if(command == 138){ // motion.spindle-speed-out-rps
          spindleSetRpm=dataTwo*60;
        }
      #endif
    }else if(command > 199 && command < 300){
      // This is a command from the "axis" catagory.
      if(command == 224){ // axis.N.joint-pos-cmd
        if(dataOne == 0){
          #if stepperX
            targetPos[dataOne]=(dataTwo/10000.0)*stepsPerUnitX;
            if(targetPos[dataOne]!=targetPosOld[dataOne]){
              #if useVectors
                targetPos[dataOne]=getVector(dataOne, targetPos[dataOne]);
              #endif
              stprX.moveTo(targetPos[dataOne]);
              targetPosOld[dataOne]=targetPos[dataOne];
            }
          #endif
          #if encodedServoX
          #endif
        }
        if(dataOne == 1){
          #if stepperY
            targetPos[dataOne]=(dataTwo/10000.0)*stepsPerUnitY;
            if(targetPos[dataOne]!=targetPosOld[dataOne]){
              #if useVectors
                targetPos[dataOne]=getVector(dataOne, targetPos[dataOne]);
              #endif
              stprY.moveTo(targetPos[dataOne]);
              targetPosOld[dataOne]=targetPos[dataOne];
            }
          #endif
        }
        if(dataOne == 2){
          #if stepperZ
            targetPos[dataOne]=(dataTwo/10000.0)*stepsPerUnitZ;
            if(targetPos[dataOne]!=targetPosOld[dataOne]){
              #if useVectors
                targetPos[dataOne]=getVector(dataOne, targetPos[dataOne]);
              #endif
              stprZ.moveTo(targetPos[dataOne]);
              targetPosOld[dataOne]=targetPos[dataOne];
            }
          #endif
        }
        if(dataOne == 3){
          #if stepperA
            targetPos[dataOne]=(dataTwo/10000.0)*stepsPerUnitA;
            if(targetPos[dataOne]!=targetPosOld[dataOne]){
              #if useVectors
                targetPos[dataOne]=getVector(dataOne, targetPos[dataOne]);
              #endif
              stprA.moveTo(targetPos[dataOne]);
              targetPosOld[dataOne]=targetPos[dataOne];
            }
          #endif
        }
        if(dataOne == 4){
          #if stepperB
            targetPos[dataOne]=(dataTwo/10000.0)*stepsPerUnitB;
            if(targetPos[dataOne]!=targetPosOld[dataOne]){
              #if useVectors
                targetPos[dataOne]=getVector(dataOne, targetPos[dataOne]);
              #endif
              stprB.moveTo(targetPos[dataOne]);
              targetPosOld[dataOne]=targetPos[dataOne];
            }
          #endif
        }
        if(dataOne == 5){
          #if stepperC
            targetPos[dataOne]=(dataTwo/10000.0)*stepsPerUnitC;
            if(targetPos[dataOne]!=targetPosOld[dataOne]){
              #if useVectors
                targetPos[dataOne]=getVector(dataOne, targetPos[dataOne]);
              #endif
              stprC.moveTo(targetPos[dataOne]);
              targetPosOld[dataOne]=targetPos[dataOne];
            }
          #endif
        }
        if(dataOne == 6){
          #if stepperU
            targetPos[dataOne]=(dataTwo/10000.0)*stepsPerUnitU;
            if(targetPos[dataOne]!=targetPosOld[dataOne]){
              #if useVectors
                targetPos[dataOne]=getVector(dataOne, targetPos[dataOne]);
              #endif
              stprU.moveTo(targetPos[dataOne]);
              targetPosOld[dataOne]=targetPos[dataOne];
            }
          #endif
        }
        if(dataOne == 7){
          #if stepperV
            targetPos[dataOne]=(dataTwo/10000.0)*stepsPerUnitV;
            if(targetPos[dataOne]!=targetPosOld[dataOne]){
              #if useVectors
                targetPos[dataOne]=getVector(dataOne, targetPos[dataOne]);
              #endif
              stprV.moveTo(targetPos[dataOne]);
              targetPosOld[dataOne]=targetPos[dataOne];
            }
          #endif
        }
        if(dataOne == 8){
          #if stepperW
            targetPos[dataOne]=(dataTwo/10000.0)*stepsPerUnitW;
            if(targetPos[dataOne]!=targetPosOld[dataOne]){
              #if useVectors
                targetPos[dataOne]=getVector(dataOne, targetPos[dataOne]);
              #endif
              stprW.moveTo(targetPos[dataOne]);
              targetPosOld[dataOne]=targetPos[dataOne];
            }
          #endif
        }
        if(debug){
          Serial.print("X=");
          Serial.println(targetPos[0]);
          Serial.print("Y=");
          Serial.println(targetPos[1]);
          Serial.print("Z=");
          Serial.println(targetPos[2]);
        }
      }
      if(command == 226){
        if(dataTwo<0){dataTwo=dataTwo*-1;}
        float reqSpeed=(dataTwo/10000.0)*stepsPerUnit[dataOne];
        float delta=getDelta(dataOne,targetPos[dataOne]);
        // ok, LinuxCNC likes to spam these about 0.5 seconds too early. So we delay them,
        // and repeats reset the timer. (Otherwise we'll stall to zero prematurely and jerk back to life later.)
        velocityTimer[dataOne]=millis();
        if(reqSpeed+avgDist[dataOne]>delta){
          velocity[dataOne]=reqSpeed+avgDist[dataOne];
        }else{
          velocity[dataOne]=delta;
        }
//          velocity[dataOne]=dataTwo;
/*
        #if stepperX
          if(dataOne==0){
            stprX.setMaxSpeed(velocity[dataOne]);
          }
        #endif
        #if stepperY
          if(dataOne==1){
            stprY.setMaxSpeed(velocity[dataOne]);
          }
        #endif
        #if stepperZ
          if(dataOne==2){
            stprZ.setMaxSpeed(velocity[dataOne]);
          }
        #endif
        #if stepperA
          if(dataOne==3){
            stprA.setMaxSpeed(velocity[dataOne]);
          }
        #endif
        #if stepperB
          if(dataOne==4){
            stprB.setMaxSpeed(velocity[dataOne]);
          }
        #endif
        #if stepperC
          if(dataOne==5){
            stprC.setMaxSpeed(velocity[dataOne]);
          }
        #endif
        #if stepperU
          if(dataOne==6){
            stprU.setMaxSpeed(velocity[dataOne]);
          }
        #endif
        #if stepperV
          if(dataOne==7){
            stprV.setMaxSpeed(velocity[dataOne]);
          }
        #endif
        #if stepperW
          if(dataOne==8){
            stprW.setMaxSpeed(velocity[dataOne]);
          }
        #endif
*/
      }

    }else if(command > 299 && command < 400){
      // This is a command from the "iocontrol" catagory.
    }else if(command > 799 && command < 900){
      // This is a custom USER made command. 800~899
    }else if(command > 989 && command < 999){
      // This is a firmware query.
      if(command == 990){ // Firmware title.
      #if useEthernet
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.write(firmwareTitle);
        udp.endPacket();
      #endif
      #if useDueNativePort
        SerialUSB.println(firmwareTitle);
      #else
        Serial.println(firmwareTitle);
      #endif
      }else if(command == 991){ // Version info.
      #if useEthernet
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.write(Version);
        udp.endPacket();
      #endif
      #if useDueNativePort
        SerialUSB.println(Version);
      #else
        Serial.println(Version);
      #endif
      }else if(command == 992){ // unitId if used in a swarm.
      #if useEthernet
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        int uid=unitNumber;
        udp.write(uid);
        udp.endPacket();
      #endif
      #if useDueNativePort
        SerialUSB.println(unitNumber);
      #else
        Serial.println(unitNumber);
      #endif
      }else if(command == 993){ // request HAL hooks per command list.
      #if useEthernet
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.write(commandsRequested);
        udp.endPacket();
      #endif
      #if useDueNativePort
        SerialUSB.println(commandsRequested);
      #else
        Serial.println(commandsRequested);
      #endif
      }else if(command == 994){ // request HAL hooks per axis list.
      #if useEthernet
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.write(axisRequested);
        udp.endPacket();
      #endif
      #if useDueNativePort
        SerialUSB.println(axisRequested);
      #else
        Serial.println(axisRequested);
      #endif
      }else if(command == 995){ // toggle debugging output.
        debug=!debug;
        if(debug){
        #if useDueNativePort
          SerialUSB.println("Debug: on");
        #else
          Serial.println("Debug: on");
        #endif
        }else{
        #if useDueNativePort
          SerialUSB.println("Debug: off");
        #else
          Serial.println("Debug: off");
        #endif
        }
      }else if(command == 996){ // Got 'green light' from host to begin normal operations.
        clientOps=true;
 /*
        Serial.print("997 ");
        Serial.print(unitNumber);
        Serial.print(" ");
        Serial.print(clientOps);
        Serial.print(" ");
        Serial.print(997+unitNumber+clientOps);
        Serial.println(";");
*/
      }
    }
    // Anti-spam check.
    checksumOld = command + dataOne + dataTwo; // Should not have any, but just in case.
  }
}

void doUrgentStuff()
{
  /* This is a good place to call a functions to handle
     limit switches and NON-blocking
     stepper or servo/encoder controller routines;
     things that are time sensitive but also don't require
     everything else wait for it to complete.
  */
  #if useLimitSwitches
    checkSwitches();
  #endif
  #if useAccelStepper
    runSteppers();
  #endif
  #if useEncoder
    readEncoders();
  #endif
  #if useEncoderServo
    moveEncoderServos();
  #endif
}

void doIdleStuff()
{
  /* This is a good place to call less time sensitive
     functions like coolant/lube pumps, jog wheels,
     button controls, status LEDs and LCD updates.
  */
  #if useSpindle
    updateSpindle();
  #endif
  #if modLcd
    lcdUpdate();
  #endif
}

#define numWords 3 
long integerValue[numWords];
boolean serRst=true;
int n=0; 
char incomingByte;
boolean makeNeg=false;

void validateCommand(){
  boolean commandValidated=false;
  #if useDueNativePort
    if(debug){
      SerialUSB.print("got: ");
      SerialUSB.print(integerValue[0]);
      SerialUSB.print(", ");
      SerialUSB.print(integerValue[1]);
      SerialUSB.print(", ");
      SerialUSB.print(integerValue[2]);
      SerialUSB.print(", ");
      SerialUSB.println(integerValue[3]);
    }
  #else
    if(debug){
      Serial.print("got: ");
      Serial.print(integerValue[0]);
      Serial.print(", ");
      Serial.print(integerValue[1]);
      Serial.print(", ");
      Serial.print(integerValue[2]);
      Serial.print(", ");
      Serial.println(integerValue[3]);
    }
  #endif
//  if((integerValue[0]+integerValue[1]+integerValue[2])==integerValue[3]){
    processCommand(integerValue[0],integerValue[1],integerValue[2]);
    commandValidated=true;
//  }else{
//    commandValidated=false;
//  }
/*
  if(integerValue[0]<990){
    long msg[4]={997,unitNumber,commandValidated,997+unitNumber+commandValidated};
    for(int i=0;i<4;i++){
    #if useEthernet
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.write(msg[i]);
      if(i<3){
        udp.write(" ");
      }else{
        udp.write(";");
      }
      udp.endPacket();
    #else
      Serial.print(msg[i],DEC);
      if(i<3){
      #if useDueNativePort
        SerialUSB.print(" ");
      }else{
        SerialUSB.println(";");
      }
      #else
        Serial.print(" ");
      }else{
        Serial.println(";");
      }
      #endif
    #endif
    }
  }
*/
}

void sendWaitPacket(unsigned int val){
  Serial.print("998 0 ");
  Serial.print(val);
  Serial.println(";");
}

void loop(){
  #if useEthernet
  int packetSize = udp.parsePacket();
  if(packetSize){
    IPAddress remote = udp.remoteIP();
    if(serRst){for(int i=0;i<numWords;i++){if(clientOps){doUrgentStuff();}integerValue[i]=0;};n=0;serRst=false;}
    incomingByte = udp.read();
    if(incomingByte == ' '){if(makeNeg){integerValue[n] *= -1;makeNeg=false;}n++;incomingByte = udp.read();}
  #elif useDueNativePort
  if(SerialUSB.available()){
    if(serRst){for(int i=0;i<numWords;i++){if(clientOps){doUrgentStuff();}integerValue[i]=0;};n=0;serRst=false;}
    incomingByte = SerialUSB.read();
    if(incomingByte == ' '){if(makeNeg){integerValue[n] *= -1;makeNeg=false;}n++;incomingByte = SerialUSB.read();}
  #else
  if(Serial.available()){
    if(Serial.available()>30){sendWaitPacket(Serial.available());}
    if(serRst){for(int i=0;i<numWords;i++){if(clientOps){doUrgentStuff();}integerValue[i]=0;};n=0;serRst=false;}
    incomingByte = Serial.read();
    if(incomingByte == ' '){if(makeNeg){integerValue[n] *= -1;makeNeg=false;}n++;incomingByte = Serial.read();}
  #endif
    if(incomingByte == ';'){if(makeNeg){integerValue[n] *= -1;makeNeg=false;}validateCommand();serRst=true;}
    if(incomingByte == '\r'){serRst=true;}
    if(incomingByte == '\n'){serRst=true;}
    if (incomingByte == '-'){ // got a "-" symbol.
      makeNeg=true; // make value negative.
    }else{
      if(incomingByte > 47 && incomingByte < 58){
        integerValue[n] *= 10;  // shift left 1 decimal place
        if(!serRst)integerValue[n] = ((incomingByte - 48) + integerValue[n]);
      }
    }
  }
  if(clientOps){
    doUrgentStuff();
    doIdleStuff();
  }
}

