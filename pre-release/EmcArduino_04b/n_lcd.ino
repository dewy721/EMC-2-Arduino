LiquidCrystal lcd(lcdRS,lcdEN,lcdD4,lcdD5,lcdD6,lcdD7);

void lcdPrint(long xLong, long yLong, long zLong)
{ // Usefull for keeping lcd readable while lowering resources overall.
  if(lcdInUse==true) {
    long currentTime = millis();
    double x=xLong*0.000001;
    double y=yLong*0.000001;
    double z=zLong*0.000001;
    if(currentTime - lcdTime > lcdTimeOld)
    {
      lcd.clear();
      if(lcdXEnabled == true)
      {
        lcd.write("X:");
        lcd.print(x,3);
      }
      if(lcdYEnabled == true)
      {
        lcd.setCursor(0,1);
        lcd.write("Y:");
        lcd.print(y,3);
      }
      if(lcdZEnabled == true)
      {
        lcd.setCursor(0,2);
        lcd.write("Z:");
  //      PString(buffer, sizeof(buffer), z);
        lcd.print(z,3);
  //      lcd.write(itoa(z,itoaBuff,10));
      }
      lcdTimeOld=currentTime;
    }
  }
}

