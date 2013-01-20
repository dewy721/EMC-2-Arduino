// This is a sample mod to add (quadrature encoded) servo support to HAL2Arduino 0.1 or higher.
// To enable:
// 1. Copy this file into the HAL2Arduino folder.
// 2. Add moveServos(); to the doUrgentStuff() function located in the HAL2Arduino file.


#include <Encoder.h> // http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <digitalWriteFast.h> // http://code.google.com/p/digitalwritefast/
#define encoderXa 21 // Your quadrature encoder pins go here.
#define encoderXb 20
#define encoderYa 18
#define encoderYb 19
#define pwmX 11
#define pwmY 3
#define xPowerOutput 210 // Aright Scotty. How fast you wanna go. Too much = noisy,fuzzy (but interesting) drawings
#define yPowerOutput 80 //  and a hot driver circuit,  Too little = weak, wobbly or no movement at all. Min.value=0 Max.value=255
#define xOverdrivePwr 255 // Reffer to the l_variables tab. (Section: Servo options)
#define yOverdrivePwr 255
#define dirX 13
#define dirY 12
#define enableX 8
#define enableY 9
#define zSolenoid 2
#define xServoTolerance 1 // Slop allowance, too little = oscillation vs. too much = sloppy deadzone, inaccuracy.
#define yServoTolerance 1 // Ditto.
boolean xPowerRamping=true; // Kick power up if servos they get stuck
boolean yPowerRamping=true; // Leave this on unless your running your rig near its breaking point.
                            // PowerOutput+findDiff(target,currentPos) = Ramped power output.

// Plotter head
boolean zSolenoidInvert=true; // true = engaged deploy pen/pencil/etc.
                              // false = engage to lift pen/pencil/etc.

long posXold;
long posYold;
long posZold;
boolean encServoFirstRun=true;
long curPos;
int dirPin;
int pwmPin;
int pwmMin;
int pwmMax;
int servoTolerance;
int enable;
boolean powerRamping;
Encoder encoderX(encoderXa,encoderXb);
Encoder encoderY(encoderYa,encoderYb);
long currentPosX;
long currentPosY;

long findDiff(long a, long b)
{ // find distance between to integers.
  long c;
  if(a>0 && b>0){if(a<b){c=b-a;}else{c=a-b;}}
  if(a<0 && b<0){if(a<b){c=b-a;}else{c=a-b;}}
  if(a<0 && b>0){c=b+(a*-1);}
  if(a>0 && b<0){c=a+(b*-1);}
  if(c<0){c=c*-1;}
  return c; // a positive integer.
}

void moveServos(){
  if(encServoFirstRun){
    encoderX.write(-1);
    encoderY.write(-1);
    encServoFirstRun=false;
  }
  currentPosX=encoderX.read();
  long diff=findDiff(posX,currentPosX);
  if(diff > xServoTolerance) // We need to move, so power up motor and move.
  {
    digitalWriteFast2(enableX,LOW);
    if(xPowerRamping)
      analogWrite(pwmX,constrain(diff, xPowerOutput, xOverdrivePwr));
    if(posX < currentPosX)
    {
      digitalWriteFast2(dirX,HIGH); // Move X back
    }else{
      digitalWriteFast2(dirX,LOW); // Move X forward
    }
  }else{ // were on target, while we're here disable moter. (Don't cook the contact brushes.)
    digitalWriteFast2(enableX,HIGH);
    analogWrite(pwmX,0);
  }
  currentPosY=encoderY.read();
  diff=findDiff(posY,currentPosY);
  if(diff > yServoTolerance)
  {
    digitalWriteFast2(enableY,LOW);
    if(yPowerRamping)
      analogWrite(pwmY,constrain(diff, yPowerOutput, yOverdrivePwr));
    if(posY > currentPosY)
    {
      digitalWriteFast2(dirY,HIGH); // Move Y back
    }else{
      digitalWriteFast2(dirY,LOW); // Move Y forward
    }
  }else{
    digitalWriteFast2(enableY,HIGH);
    analogWrite(pwmY,0);
  }
  if(zSolenoidInvert==true)
  {
    if(posZ < 0)
    {
      digitalWriteFast2(zSolenoid,HIGH);
    }else{
      digitalWriteFast2(zSolenoid,LOW);
    }
  }else{
    if(posZ > 0)
    {
      digitalWriteFast2(zSolenoid,HIGH);
    }else{
      digitalWriteFast2(zSolenoid,LOW);
    }
  }
}
