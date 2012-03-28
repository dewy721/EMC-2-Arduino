long curTime;
long oldTime;
boolean xHomeStateOld=false;
boolean yHomeStateOld=false;
boolean zHomeStateOld=false;
boolean aHomeStateOld=false;
long noOpTime=0;

void loop() {
  //First. check the state of the pwrSwitch
  if(digitalRead(pwrSwitch) == HIGH) {
    if(pwrSwitchState == 0) {           //the state has changed
//      Serial.print("+S");       //Send a message to the PC
      pwrSwitchState = 1;              //Update the pwrSwitchitch state
    }
    //If the pwrSwitchitch state was 1, then we wouldn't care
    //since it means it hasn't changed.
  }
  else { //the pwrSwitchitch is low, see if it's gone high
    if(pwrSwitchState == 1) {
//      Serial.print("-S");
      pwrSwitchState = 0;
    }
  }
  if(ATXpsu==true) {
    long curTime=millis();
    if(Serial.available() > 0) {
      digitalWriteFast2(atxPowerCtrl,LOW); // turn psu on
      oldTime=curTime;
    }else{
      if(curTime - oldTime > atxPowerTimeout) {
        digitalWriteFast2(atxPowerCtrl,HIGH); // turn psu off
      }
    }
  }
    // listen for serial commands
    if(Serial.available() > 0) {
      while(Serial.available() > 0) {
        buffer[128];
        buffer[sofar++]=Serial.read();
        if(buffer[sofar-1]==';')
        {
//          Serial2.println(";");
          break; // in case there are multiple instructions
        }else{
//          Serial2.print(buffer);
        }
      }
    }else{

      long curTime=millis();
      if(curTime - noOpTime >= idleTime)
      {
        Serial.println(".."); // keep connection alive.
        noOpTime=curTime+idleTime;
      }else{
        noOpTime=curTime+idleTime;
      }
    }
   
    // if we hit a semi-colon, assume end of instruction.
    if(sofar>0 && buffer[sofar-1]==';') {
      // what if message fails/garbled?
   
      // echo confirmation
      buffer[sofar]=0;
      //Serial.println(buffer);
   
      // do something with the command
      processCommand();
   
      // reset the buffer
      sofar=0;
   
      // echo completion
  //    Serial.println("Done.");
    }
    if(digitalReadFast2(atxPowerState))
    {
      digitalWriteFast2(atxPowerLed,HIGH); // Turn power indicator on.
    }else{
      digitalWriteFast2(atxPowerLed,LOW); // Turn power indicator off.
    }
//boolean xHomeState=digitalReadFast2(xHomePin);
if(xHomeState != xHomeStateOld)
{
  xHomeStateOld=xHomeState;
  Serial.print("x");
  Serial.print(xHomeState+4);
}
//boolean yHomeState=digitalReadFast2(yHomePin);
if(yHomeState != yHomeStateOld)
{
  yHomeStateOld=yHomeState;
  Serial.print("y");
  Serial.print(yHomeState+4);
}
if(zHomeState != zHomeStateOld)
{
  zHomeStateOld=zHomeState;
  Serial.print("z");
  Serial.print(zHomeState+4);
}
if(aHomeState != aHomeStateOld)
{
  aHomeStateOld=aHomeState;
  Serial.print("a");
  Serial.print(aHomeState+4);
}
    processLimits('X',targetX); 
    processLimits('Y',targetY); 
    processLimits('Z',targetZ);
    processLimits('A',targetA); 
    doStepperMove('X',targetX);
    doStepperMove('Y',targetY);
    doStepperMove('Z',targetZ);
    doStepperMove('A',targetA);
//Serial.println(targetX);
//delayMicroseconds(125);
}

