#define useControls false //Enable/Disable support for control panel button switches.
// This modFile supports basic user input controls
#if useControls
  #define eStopSwitchPin 0
  #define machinePowerSwitchPin 0
  #define startSwitchPin 0
  #define programStepSwitchPin 0
  #define pauseSwitchPin 0
  #define stopSwitchPin 0

  #define eStopIndicatorPin 5
  #define machineIndicatorSwitchPin 6
  #define startIndicatorPin 7
  #define programStepIndicatorPin 0
  #define pauseIndicatorPin 0
  #define stopIndicatorPin 0

void contolsSetup(){
  pinMode(eStopSwitchPin,INPUT);
  pinMode(machinePowerSwitchPin,INPUT);
  pinMode(startSwitchPin,INPUT);
  pinMode(programStepSwitchPin,INPUT);
  pinMode(pauseSwitchPin,INPUT);
  pinMode(stopSwitchPin,INPUT);
  pinMode(eStopIndicatorPin,OUTPUT);
  pinMode(machineIndicatorSwitchPin,OUTPUT);
  pinMode(startIndicatorPin,OUTPUT);
  pinMode(programStepIndicatorPin,OUTPUT);
  pinMode(pauseIndicatorPin,OUTPUT);
  pinMode(stopIndicatorPin,OUTPUT);
  digitalWrite(eStopSwitchPin,HIGH);
  digitalWrite(machinePowerSwitchPin,HIGH);
  digitalWrite(startSwitchPin,HIGH);
  digitalWrite(programStepSwitchPin,HIGH);
  digitalWrite(pauseSwitchPin,HIGH);
  digitalWrite(stopSwitchPin,HIGH);
}
#endif

