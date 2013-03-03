// That's it your done. :)
// This is the program that will actually run things.
// There should be no need to modify it.


void setup()
{
  #if modLcd
    lcdSetup();
  #endif
  #if useAccelStepper
    stepperSetup();
  #endif

  Serial.begin(BAUD);
  Serial.setTimeout(statementTimeout);
  Serial.println("ok");
}


long checksumOld=0;
boolean debug=false;

void processCommand(long command, long dataOne, long dataTwo)
{
  if(command + dataOne + dataTwo != checksumOld){
    if(command > 99 && command < 200){
      // This is a command from the "motion" catagory.
    }else if(command > 199 && command < 300){
      // This is a command from the "axis" catagory.
      if(command == 224){
        if(dataOne == 0){
          xTargetPos=dataTwo;
          #if stepperX
            stprX.moveTo(xTargetPos/stpsPrInX);
          #endif
        }
        if(dataOne == 1){
          yTargetPos=dataTwo;
          #if stepperY
            stprY.moveTo(yTargetPos/stpsPrInY);
          #endif
        }
        if(dataOne == 2){
          zTargetPos=dataTwo;
          #if stepperZ
            stprZ.moveTo(zTargetPos/stpsPrInZ);
          #endif
        }
        if(dataOne == 3){
          aTargetPos=dataTwo;
          #if stepperA
            stprA.moveTo(aTargetPos/stpsPrInA);
          #endif
        }
        if(dataOne == 4){
          bTargetPos=dataTwo;
          #if stepperB
            stprB.moveTo(bTargetPos/stpsPrInB);
          #endif
        }
        if(dataOne == 5){
          cTargetPos=dataTwo;
          #if stepperC
            stprC.moveTo(cTargetPos/stpsPrInC);
          #endif
        }
        if(dataOne == 6){
          uTargetPos=dataTwo;
          #if stepperU
            stprU.moveTo(uTargetPos/stpsPrInU);
          #endif
        }
        if(dataOne == 7){
          vTargetPos=dataTwo;
          #if stepperV
            stprV.moveTo(vTargetPos/stpsPrInV);
          #endif
        }
        if(dataOne == 8){
          wTargetPos=dataTwo;
          #if stepperW
            stprW.moveTo(wTargetPos/stpsPrInW);
          #endif
        }
        if(debug){
          Serial.print("X=");
          Serial.println(xTargetPos);
          Serial.print("Y=");
          Serial.println(yTargetPos);
          Serial.print("Z=");
          Serial.println(zTargetPos);
        }
      }
      if(command == 226){
        #if stepperX
          if(dataOne==0){
            stprX.setMaxSpeed((dataTwo+1)*stepsPerInchX);
          }
        #endif
        #if stepperY
          if(dataOne==1){
            stprY.setMaxSpeed((dataTwo+1)*stepsPerInchY);
          }
        #endif
        #if stepperZ
          if(dataOne==2){
            stprZ.setMaxSpeed((dataTwo+1)*stepsPerInchZ);
          }
        #endif
        #if stepperA
          if(dataOne==3){
            stprA.setMaxSpeed((dataTwo+1)*stepsPerInchA);
          }
        #endif
        #if stepperB
          if(dataOne==4){
            stprB.setMaxSpeed((dataTwo+1)*stepsPerInchB);
          }
        #endif
        #if stepperC
          if(dataOne==5){
            stprC.setMaxSpeed((dataTwo+1)*stepsPerInchC);
          }
        #endif
        #if stepperU
          if(dataOne==6){
            stprU.setMaxSpeed((dataTwo+1)*stepsPerInchU);
          }
        #endif
        #if stepperV
          if(dataOne==7){
            stprV.setMaxSpeed((dataTwo+1)*stepsPerInchV);
          }
        #endif
        #if stepperW
          if(dataOne==8){
            stprW.setMaxSpeed((dataTwo+1)*stepsPerInch);
          }
        #endif
      }
    }else if(command > 299 && command < 400){
      // This is a command from the "iocontrol" catagory.
    }else if(command > 799 && command < 900){
      // This is a custom USER made command. 800~899
    }else if(command > 989 && command < 999){
      // This is a firmware query.
      if(command == 990){ // Firmware title.
        Serial.println(firmwareTitle);
      }else if(command == 991){ // Version info.
        Serial.println(Version);
      }else if(command == 992){ // unitId if used in a swarm.
        Serial.println(unitNumber);
      }else if(command == 993){ // request HAL hooks per command list.
        Serial.println(commandsRequested);
      }else if(command == 994){ // request HAL hooks per axis list.
        Serial.println(axisRequested);
      }else if(command == 995){ // toggle debugging output.
        debug=!debug;
        if(debug){
          Serial.println("Debug: on");
        }else{
          Serial.println("Debug: off");
        }
      }else if(command == 996){ // Got 'green light' from host to begin normal operations.
        clientOps=true;
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
}

void doIdleStuff()
{
  /* This is a good place to call less time sensitive
     functions like coolant/lube pumps, jog wheels,
     button controls, status LEDs and LCD updates.
  */
  #if modLcd
    lcdUpdate();
  #endif
}

#define numWords 4 
long integerValue[numWords];
boolean serRst=true;
int n=0; 
char incomingByte;
boolean makeNeg=false;

void validateCommand(){
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
  if((integerValue[0]+integerValue[1]+integerValue[2])==integerValue[3]){
    processCommand(integerValue[0],integerValue[1],integerValue[2]);
    //Serial.println("ok"); //Crashes python code. Don't use yet.
  }else{
    Serial.println("resend");
  }
}

void loop(){
  if(Serial.available()){
    if(serRst){for(int i=0;i<numWords;i++){if(clientOps){doUrgentStuff();}integerValue[i]=0;};n=0;serRst=false;}
    incomingByte = Serial.read();
    if(incomingByte == ' '){if(makeNeg){integerValue[n] *= -1;makeNeg=false;}n++;incomingByte = Serial.read();}
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

