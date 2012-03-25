void moveTo(char axis, long target)
{
  if(axis == 'X') {
    targetX=target;
    oldTimeX=millis();
    doStepperMove('X',target);
    targetX=target;
  }

  if(axis == 'Y') {
    targetY=target;
    oldTimeY=millis();
    doStepperMove('Y',target);
    targetY=target;
  }

  if(axis == 'Z') {
    targetZ=target;
    oldTimeZ=millis();
    doStepperMove('Z',target);
    targetZ=target;
  }
    if(lcdInUse) {
    lcdPrint(targetX,targetY,targetZ);
  }

}
boolean xHomeState=false;
boolean xStepState=0;
boolean yHomeState=false;
boolean yStepState=0;
boolean zHomeState=false;
boolean zStepState=0;
boolean aHomeState=false;
boolean aStepState=0;
long stepTimeX=0;
long stepTimeXOld=0;
long stepTimeY=0;
long stepTimeYOld=0;
long stepTimeZ=0;
long stepTimeZOld=0;
long stepTimeA=0;
long stepTimeAOld=0;
void doStepperMove(char axis, long Position)
{
  int incBy=0;
  int incDir=0;
  long xSteps=0;
  unsigned long curTime = micros();

  if(axis == 'X')
  {
    if(Position == currentX)
    {
      incDir=0;
    }else{
      xSteps=findDiff(Position,currentX)*2;
      digitalWrite(xEnablePin,LOW);
//      stepMode('X',xSteps/2);
    }

    if(currentX > 2000 && currentX < 3250)
    {
      xHomeState=true;
    }else{
      xHomeState=false;
    }

    if(xSteps!=0)
    {
      digitalWriteFast2(13,LOW);
      if(curTime - stepTimeXOld >= (minStepTime/2))
      {
        if(Position < currentX)
        {
          digitalWriteFast2(xDirPin,HIGH);
          currentX--;
    //      currentX=currentX-stepModeX;
        }
        if(Position > currentX)
        {
          digitalWriteFast2(xDirPin,LOW);
          currentX++;
    //      currentX=currentX+stepModeX;
        }
        xStepState=!xStepState;
        digitalWriteFast2(xStepPin,xStepState);
        stepTimeXOld=curTime;
      }
    }
  }
  
  long ySteps=0;
  if(axis == 'Y')
  {
    if(Position == currentY)
    {
      incDir=0;
    }else{
      ySteps=findDiff(Position,currentY)*2;
      digitalWrite(yEnablePin,LOW);
    }

    if(currentY > 2000 && currentY < 3250)
    {
      yHomeState=true;
    }else{
      yHomeState=false;
    }

    if(ySteps!=0)
    {
      digitalWriteFast2(13,LOW);
      if(curTime - stepTimeYOld >= (minStepTime/2))
      {
        if(Position < currentY)
        {
          digitalWriteFast2(yDirPin,HIGH);
          currentY--;
        }
        if(Position > currentY)
        {
          digitalWriteFast2(yDirPin,LOW);
          currentY++;
        }
        yStepState=!yStepState;
        digitalWriteFast2(yStepPin,yStepState);
        stepTimeYOld=curTime;
      }
    }
  }
  
  
  long zSteps=0;
  if(axis == 'Z')
  {
    if(Position == currentZ)
    {
      incDir=0;
    }else{
      zSteps=findDiff(Position,currentZ)*2;
      digitalWrite(zEnablePin,LOW);
    }
    
    if(currentZ > 2000 && currentZ < 3250)
    {
      zHomeState=true;
    }else{
      zHomeState=false;
    }

    if(zSteps!=0)
    {
      digitalWriteFast2(13,LOW);
      if(curTime - stepTimeZOld >= (minStepTime/2))
      {
        if(Position < currentZ)
        {
          digitalWriteFast2(zDirPin,HIGH);
          currentZ--;
        }
        if(Position > currentZ)
        {
          digitalWriteFast2(zDirPin,LOW);
          currentZ++;
        }
        zStepState=!zStepState;
        digitalWriteFast2(zStepPin,zStepState);
        stepTimeZOld=curTime;
      }
    }
  }
  
  
  long aSteps=0;
  if(axis == 'A')
  {
    if(Position == currentA)
    {
      incDir=0;
    }else{
      aSteps=findDiff(Position,currentA)*2;
      digitalWrite(aEnablePin,LOW);
    }
      
    if(currentA > 2000 && currentA < 3250)
    {
      aHomeState=true;
    }else{
      aHomeState=false;
    }

    if(aSteps!=0)
    {
      digitalWriteFast2(13,LOW);
      if(curTime - stepTimeAOld >= (minStepTime/2))
      {
        if(Position < currentA)
        {
          digitalWriteFast2(aDirPin,HIGH);
          currentA--;
        }
        if(Position > currentA)
        {
          digitalWriteFast2(aDirPin,LOW);
          currentA++;
        }
        aStepState=!aStepState;
        digitalWriteFast2(aStepPin,aStepState);
        stepTimeAOld=curTime;
      }
    }
  }
  if(xSteps+ySteps+zSteps+aSteps==0)
    digitalWriteFast2(13,HIGH);
}

void stepMode(char axis, int mode)
{
  boolean ms1;
  boolean ms2;
  boolean ms3;
  int count;
  if(mode>=16)
  {
    ms1=LOW;
    ms2=LOW;
    ms3=LOW;
    count=16;
  }
  if(mode>=8 && mode<=15)
  {
    ms1=HIGH;
    ms2=LOW;
    ms3=LOW;
    count=8;
  }
  if(mode>=4 && mode<=7)
  {
    ms1=LOW;
    ms2=HIGH;
    ms3=LOW;
    count=4;
  }
  if(mode>=2 && mode<=3)
  {
    ms1=HIGH;
    ms2=HIGH;
    ms3=LOW;
    count=2;
  }
  if(mode<=1)
  {
    ms1=HIGH;
    ms2=HIGH;
    ms3=HIGH;
    count=1;
  }
  if(axis == 'X' || 'ALL')
  {
    if(mode!=stepModeX)
    {
      digitalWriteFast2(chanXms1,ms1);
      digitalWriteFast2(chanXms2,ms2);
      digitalWriteFast2(chanXms3,ms3);
      stepModeX=count;
    }
  }
  if(axis == 'Y' || 'ALL')
  {
    if(mode!=stepModeY)
    {
      digitalWriteFast2(chanYms1,ms1);
      digitalWriteFast2(chanYms2,ms2);
      digitalWriteFast2(chanYms3,ms3);
      stepModeY=count;
    }
  }
  if(axis == 'Z' || 'ALL')
  {
    if(mode!=stepModeZ)
    {
      digitalWriteFast2(chanZms1,ms1);
      digitalWriteFast2(chanZms2,ms2);
      digitalWriteFast2(chanZms3,ms3);
      stepModeZ=count;
    }
  }
  if(axis == 'A' || 'ALL')
  {
    if(mode!=stepModeA)
    {
      digitalWriteFast2(chanAms1,ms1);
      digitalWriteFast2(chanAms2,ms2);
      digitalWriteFast2(chanAms3,ms3);
      stepModeA=count;
    }
  }
}

      

