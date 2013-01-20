// This is a basic LCD mod for HAL2Arduino 0.1 or higher.
// It just displays the current coordinates of X, Y and Z axes.
// To enable:
// 1. Copy this file to your HAL2Arduino folder.
// 2. Set your pins assignments below.
// 3. Add lcdUpdate(); to doIdleStuff() function in HAL2Arduino file.

// Default pins as per LadyAda's wonderful tutorial at http://learn.adafruit.com/character-lcds/wiring-a-character-lcd
#define RS 7
#define EN 8
#define D4 9
#define D5 10
#define D6 11
#define D7 12

#include <LiquidCrystal.h>
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

long xPos=-1;
long yPos=-1;
long zPos=-1;
boolean lcdStarted=false;

void lcdUpdate()
{
  if(!lcdStarted){
      lcd.begin(16,2);
      lcdStarted=true;
  }
  // If something changed update the LCD screen.
  if(xTargetPos != xPos || yTargetPos != yPos || zTargetPos != zPos){
    xPos=xTargetPos;
    yPos=yTargetPos;
    zPos=zTargetPos;
    lcd.clear();
    lcd.print("X=");
    lcd.print(xPos);
    lcd.setCursor(8,0);
    lcd.print("Y=");
    lcd.print(yPos);
    lcd.setCursor(0,1);
    lcd.print("Z=");
    lcd.print(zPos);
  }
}

