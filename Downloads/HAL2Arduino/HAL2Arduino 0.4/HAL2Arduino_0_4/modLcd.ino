#define modLcd false // set to true to enable lcd support. (for standard 'parallel' lcd screens.)
// Just displays:
//  X=pos  Y=pos
//  Z=pos

// Its just an example, thats it. (Unless someone decides to make a better version.)
#if modLcd
  
  #define lcdCols 16
  #define lcdRows 2

  #include <LiquidCrystal.h>
  LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
  
  unsigned long curTime;
  unsigned long oldTime;
  unsigned long lagTime;
  unsigned long lcdTime=millis();
  unsigned long lcdTimeOld=millis();
  float xPos;
  float yPos;
  float zPos;
  
  void lcdSetup(){
    lcd.begin(lcdCols,lcdRows);
    lcd.print(firmwareTitle);
    lcd.setCursor(0,1);
    lcd.print(Version);
  }

  void lcdUpdate()
  {
    unsigned long time=millis();
    if((time - lcdTimeOld) > 250){
      xPos=xTargetPos*0.0001;
      yPos=yTargetPos*0.0001;
      zPos=zTargetPos*0.0001;
      lcd.clear();
      lcd.print("X=");
      lcd.print(xPos);
      lcd.setCursor(8,0);
      lcd.print("Y=");
      lcd.print(yPos);
      lcd.setCursor(0,1);
      lcd.print("Z=");
      lcd.print(zPos);
      lcdTimeOld=millis();
    }
  }
#endif
