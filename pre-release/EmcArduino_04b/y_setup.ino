void setup() {
  pinMode(chanXms1, OUTPUT);
  pinMode(chanXms2, OUTPUT);
  pinMode(chanXms3, OUTPUT);
  pinMode(chanYms1, OUTPUT);
  pinMode(chanYms2, OUTPUT);
  pinMode(chanYms3, OUTPUT);
  pinMode(chanZms1, OUTPUT);
  pinMode(chanZms2, OUTPUT);
  pinMode(chanZms3, OUTPUT);
  pinMode(chanAms1, OUTPUT);
  pinMode(chanAms2, OUTPUT);
  pinMode(chanAms3, OUTPUT);

  stepMode('ALL',1);

  pinMode(xDirPin, OUTPUT);
  pinMode(yDirPin, OUTPUT);
  pinMode(zDirPin, OUTPUT);
  pinMode(aDirPin, OUTPUT);

  pinMode(xEnablePin, OUTPUT);
  pinMode(yEnablePin, OUTPUT);
  pinMode(zEnablePin, OUTPUT);
  pinMode(aEnablePin, OUTPUT);
  digitalWrite(xEnablePin,LOW);
  digitalWrite(yEnablePin,LOW);
  digitalWrite(zEnablePin,LOW);
  digitalWrite(aEnablePin,LOW);

  pinMode(xStepPin, OUTPUT);
  pinMode(yStepPin, OUTPUT);
  pinMode(zStepPin, OUTPUT);
  pinMode(aStepPin, OUTPUT);

  pinMode(13, OUTPUT); // temporary DIAG indicator.

  /*
  pinMode(xSleepPin, INPUT);
  pinMode(ySleepPin, INPUT);
  pinMode(zSleepPin, INPUT);
  pinMode(aSleepPin, INPUT);
  digitalWrite(xSleepPin,HIGH);
  digitalWrite(ySleepPin,HIGH);
  digitalWrite(zSleepPin,HIGH);
  digitalWrite(aSleepPin,HIGH);

  pinMode(xResetPin, INPUT);
  pinMode(yResetPin, INPUT);
  pinMode(zResetPin, INPUT);
  pinMode(aResetPin, INPUT);
  digitalWrite(xResetPin,HIGH);
  digitalWrite(yResetPin,HIGH);
  digitalWrite(zResetPin,HIGH);
  digitalWrite(aResetPin,HIGH);
  */

  pinMode(xHomePin, INPUT);
  pinMode(yHomePin, INPUT);
  digitalWrite(xHomePin,HIGH);
  digitalWrite(yHomePin,HIGH);
  pinMode(loadX, INPUT);
  pinMode(loadY, INPUT);
  pinMode(pwmX, OUTPUT);
  pinMode(pwmY, OUTPUT);
  pinMode(dirX, OUTPUT);
  pinMode(dirY, OUTPUT);
  pinMode(enableX, OUTPUT);
  pinMode(enableY, OUTPUT);
  pinMode(zSolenoid, OUTPUT);
  pinMode(atxPowerCtrl, OUTPUT);
  pinMode(atxPowerState, INPUT);
  pinMode(atxPowerLed, OUTPUT);
  pinMode(pwrSwitch, INPUT);
  digitalWrite(pwrSwitch,HIGH);
  analogWrite(pwmX,xPowerOutput);
  analogWrite(pwmY,yPowerOutput);
  Serial.begin(115200);
//  Serial2.begin(57600);
  lcd.begin(lcdColumns, lcdRows);
}

