// Servo options
boolean xPowerRamping=true; // Kick power up if servos they get stuck
boolean yPowerRamping=true; // Leave this on unless your running your rig near its breaking point.
                            // PowerOutput+findDiff(target,currentPos) = Ramped power output.

// Plotter head
boolean zSolenoidInvert=true; // true = engaged deploy pen/pencil/etc.
                              // false = engage to lift pen/pencil/etc.
//User settings
boolean lcdInUse=false; // Causes lag and jitters, best to leave this off for now (Useful for debugging though).
boolean lcdXEnabled=true;
boolean lcdYEnabled=true;
boolean lcdZEnabled=false;
long lcdTime=1000; // Time between lcd updates. (milliSeconds)

// ATX psu control
boolean ATXpsu=true; // Do you want automatic power control?


// Coffee * not used yet * :(
boolean makeCoffee=true; // No seriously, EMC supports custom commands. I 'will' have this one day.
// Picture it, power up your cnc in the morning and default start proggy will be make coffee.

//non-user, system variables.
int pwrSwitchState = 0;
char buffer[128];
int sofar;
float pos_x;
float pos_y;
float pos_z;
float pos_a;
char itoaBuff[48];

int xLimitOld=-1;
int yLimitOld=-1;
int zLimitOld=-1;
long limitPollingTime=10;
long limitPollingTimeOld;
long oldTimeX=-999999;
long oldTimeY=-999999;
long oldTimeZ=-999999;
long lcdTimeOld;
long targetX=1;
long targetY=1;
long targetZ=1;
long targetA=1;
long currentX=1;
long currentY=1;
long currentZ=1;
long currentA=1;
int stepModeX=4;
int stepModeY=4;
int stepModeZ=4;
int stepModeA=4;
long rollTime=0; // rollover dectection.
