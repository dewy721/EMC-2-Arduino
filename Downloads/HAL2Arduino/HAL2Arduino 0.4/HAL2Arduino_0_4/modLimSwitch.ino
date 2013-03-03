// Setup your switches.
#define useLimitSwitches true

#define XaxisEnableSwitches true
#define YaxisEnableSwitches true
#define ZaxisEnableSwitches true
#define AaxisEnableSwitches false
#define BaxisEnableSwitches false
#define CaxisEnableSwitches false
#define UaxisEnableSwitches false
#define VaxisEnableSwitches false
#define WaxisEnableSwitches false

// 0 + axisEnabled = individually disabled switch.
#define xMinSwitch 0
#define xMaxSwitch 0
#define xHomeSwitch 0

#define yMinSwitch 0
#define yMaxSwitch 0
#define yHomeSwitch 0

#define zMinSwitch 0
#define zMaxSwitch 0
#define zHomeSwitch 0

#define aMinSwitch 0
#define aMaxSwitch 0
#define aHomeSwitch 0

#define bMinSwitch 0
#define bMaxSwitch 0
#define bHomeSwitch 0

#define cMinSwitch 0
#define cMaxSwitch 0
#define cHomeSwitch 0

#define uMinSwitch 0
#define uMaxSwitch 0
#define uHomeSwitch 0

#define vMinSwitch 0
#define vMaxSwitch 0
#define vHomeSwitch 0

#define wMinSwitch 0
#define wMaxSwitch 0
#define wHomeSwitch 0

// Temporary section for virtual switches.
// in the future these will be fetched from my-mill.ini file during each startup.
// But until then, should you need virtual limit switches, set them here.
// 10000 = 1 inch/mm
#define xVirtualMin -1250
#define xVirtualMax 120000
#define xVirtualHome 10000
#define yVirtualMin -10000
#define yVirtualMax 90000
#define yVirtualHome 10000
#define zVirtualMin -20000
#define zVirtualMax 20000
#define zVirtualHome 2500
#define aVirtualMin 0
#define aVirtualMax 0
#define aVirtualHome 0
#define bVirtualMin 0
#define bVirtualMax 0
#define bVirtualHome 0
#define cVirtualMin 0
#define cVirtualMax 0
#define cVirtualHome 0
#define uVirtualMin 0
#define uVirtualMax 0
#define uVirtualHome 0
#define vVirtualMin 0
#define vVirtualMax 0
#define vVirtualHome 0
#define wVirtualMin 0
#define wVirtualMax 0
#define wVirtualHome 0

// End of configuration. Begin of program.
#if useLimitSwitches
  // Setup pin arrays.
  boolean faultId[9][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  boolean faultIdOld[9][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
  
  void switchesSetup(){
    #if xMinSwitch && XaxisEnableSwitches
      pinMode(xMinSwitch,INPUT);
      digitalWrite(xMinSwitch,HIGH);
    #endif
    #if xMaxSwitch && XaxisEnableSwitches
      pinMode(xMaxSwitch,INPUT);
      digitalWrite(xMaxSwitch,HIGH);
    #endif
    #if xHomeSwitch && XaxisEnableSwitches
      pinMode(xHomeSwitch,INPUT);
      digitalWrite(xHomeSwitch,HIGH);
    #endif
  
    #if yMinSwitch && YaxisEnableSwitches
      pinMode(yMinSwitch,INPUT);
      digitalWrite(yMinSwitch,HIGH);
    #endif
    #if yMaxSwitch && YaxisEnableSwitches
      pinMode(yMaxSwitch,INPUT);
      digitalWrite(yMaxSwitch,HIGH);
    #endif
    #if yHomeSwitch && YaxisEnableSwitches
      pinMode(yHomeSwitch,INPUT);
      digitalWrite(yHomeSwitch,HIGH);
    #endif
  
    #if zMinSwitch && ZaxisEnableSwitches
      pinMode(zMinSwitch,INPUT);
      digitalWrite(zMinSwitch,HIGH);
    #endif
    #if zMaxSwitch && ZaxisEnableSwitches
      pinMode(zMaxSwitch,INPUT);
      digitalWrite(zMaxSwitch,HIGH);
    #endif
    #if zHomeSwitch && ZaxisEnableSwitches
      pinMode(zHomeSwitch,INPUT);
      digitalWrite(zHomeSwitch,HIGH);
    #endif
  
    #if aMinSwitch && AaxisEnableSwitches
      pinMode(aMinSwitch,INPUT);
      digitalWrite(aMinSwitch,HIGH);
    #endif
    #if aMaxSwitch && AaxisEnableSwitches
      pinMode(aMaxSwitch,INPUT);
      digitalWrite(aMaxSwitch,HIGH);
    #endif
    #if aHomeSwitch && AaxisEnableSwitches
      pinMode(aHomeSwitch,INPUT);
      digitalWrite(aHomeSwitch,HIGH);
    #endif
  
    #if bMinSwitch && BaxisEnableSwitches
      pinMode(bMinSwitch,INPUT);
      digitalWrite(bMinSwitch,HIGH);
    #endif
    #if bMaxSwitch && BaxisEnableSwitches
      pinMode(bMaxSwitch,INPUT);
      digitalWrite(bMaxSwitch,HIGH);
    #endif
    #if bHomeSwitch && BaxisEnableSwitches
      pinMode(bHomeSwitch,INPUT);
      digitalWrite(bHomeSwitch,HIGH);
    #endif
  
    #if cMinSwitch && CaxisEnableSwitches
      pinMode(cMinSwitch,INPUT);
      digitalWrite(cMinSwitch,HIGH);
    #endif
    #if cMaxSwitch && CaxisEnableSwitches
      pinMode(cMaxSwitch,INPUT);
      digitalWrite(cMaxSwitch,HIGH);
    #endif
    #if cHomeSwitch && CaxisEnableSwitches
      pinMode(cHomeSwitch,INPUT);
      digitalWrite(cHomeSwitch,HIGH);
    #endif
  
    #if uMinSwitch && UaxisEnableSwitches
      pinMode(uMinSwitch,INPUT);
      digitalWrite(uMinSwitch,HIGH);
    #endif
    #if uMaxSwitch && UaxisEnableSwitches
      pinMode(uMaxSwitch,INPUT);
      digitalWrite(uMaxSwitch,HIGH);
    #endif
    #if uHomeSwitch && UaxisEnableSwitches
      pinMode(uHomeSwitch,INPUT);
      digitalWrite(uHomeSwitch,HIGH);
    #endif
  
    #if vMinSwitch && VaxisEnableSwitches
      pinMode(vMinSwitch,INPUT);
      digitalWrite(vMinSwitch,HIGH);
    #endif
    #if vMaxSwitch && VaxisEnableSwitches
      pinMode(vMaxSwitch,INPUT);
      digitalWrite(vMaxSwitch,HIGH);
    #endif
    #if vHomeSwitch && VaxisEnableSwitches
      pinMode(vHomeSwitch,INPUT);
      digitalWrite(vHomeSwitch,HIGH);
    #endif
  
    #if wMinSwitch && WaxisEnableSwitches
      pinMode(wMinSwitch,INPUT);
      digitalWrite(wMinSwitch,HIGH);
    #endif
    #if wMaxSwitch && WaxisEnableSwitches
      pinMode(wMaxSwitch,INPUT);
      digitalWrite(wMaxSwitch,HIGH);
    #endif
    #if wHomeSwitch && WaxisEnableSwitches
      pinMode(wHomeSwitch,INPUT);
      digitalWrite(wHomeSwitch,HIGH);
    #endif
  }
  
  int faultCountOld;
  void checkSwitches(){
    boolean switchState;
    int faultCount=0;
    #if XaxisEnableSwitches
      #if xMinSwitch
        switchState=digitalRead(xMinSwitch);
        faultId[0][0]=switchState;
        faultCount+=switchState;
      #else
        if(xTargetPos<xVirtualMin){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[0][0]=switchState;
        faultCount+=switchState;
      #endif
      #if xMaxSwitch
        switchState=digitalRead(xMaxSwitch);
        faultId[0][1]=switchState;
        faultCount+=switchState;
      #else
        if(xTargetPos>xVirtualMax){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[0][1]=switchState;
        faultCount+=switchState;
      #endif
      #if xHomeSwitch
        switchState=digitalRead(xHomeSwitch);
        faultId[0][2]=switchState;
        faultCount+=switchState;
      #else
        if(xTargetPos>xVirtualHome&&xTargetPos<(xVirtualHome+2500)){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[0][2]=switchState;
        faultCount+=switchState;
      #endif
    #endif
    
    #if YaxisEnableSwitches
      #if yMinSwitch
        switchState=digitalRead(yMinSwitch);
        faultId[1][0]=switchState;
        faultCount+=switchState;
      #else
        if(yTargetPos<yVirtualMin){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[1][0]=switchState;
        faultCount+=switchState;
      #endif
      #if yMaxSwitch
        switchState=digitalRead(yMaxSwitch);
        faultId[1][1]=switchState;
        faultCount+=switchState;
      #else
        if(yTargetPos<yVirtualMax){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[1][1]=switchState;
        faultCount+=switchState;
      #endif
      #if yHomeSwitch
        switchState=digitalRead(yHomeSwitch);
        faultId[1][2]=switchState;
        faultCount+=switchState;
      #else
        if(yTargetPos>yVirtualHome&&yTargetPos<(yVirtualHome+2500)){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[1][2]=switchState;
        faultCount+=switchState;
      #endif
    #endif
  
    #if ZaxisEnableSwitches
      #if zMinSwitch
        switchState=digitalRead(zMinSwitch);
        faultId[2][0]=switchState;
        faultCount+=switchState;
      #else
        if(zTargetPos<zVirtualMin){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[2][0]=switchState;
        faultCount+=switchState;
      #endif
      #if zMaxSwitch
        switchState=digitalRead(zMaxSwitch);
        faultId[2][1]=switchState;
        faultCount+=switchState;
      #else
        if(zTargetPos<zVirtualMax){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[2][1]=switchState;
        faultCount+=switchState;
      #endif
      #if zHomeSwitch
        switchState=digitalRead(zHomeSwitch);
        faultId[2][2]=switchState;
        faultCount+=switchState;
      #else
        if(zTargetPos>zVirtualHome&&zTargetPos<(zVirtualHome+2500)){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[2][2]=switchState;
        faultCount+=switchState;
      #endif
    #endif
  
    #if AaxisEnableSwitches
      #if aMinSwitch
        switchState=digitalRead(aMinSwitch);
        faultId[3][0]=switchState;
        faultCount+=switchState;
      #else
        if(aTargetPos<aVirtualMin){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[3][0]=switchState;
        faultCount+=switchState;
      #endif
      #if aMaxSwitch
        switchState=digitalRead(aMaxSwitch);
        faultId[3][1]=switchState;
        faultCount+=switchState;
      #else
        if(aTargetPos<aVirtualMax){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[3][1]=switchState;
        faultCount+=switchState;
      #endif
      #if aHomeSwitch
        switchState=digitalRead(aHomeSwitch);
        faultId[3][2]=switchState;
        faultCount+=switchState;
      #else
        if(aTargetPos>aVirtualHome&&aTargetPos<(aVirtualHome+2500)){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[3][2]=switchState;
        faultCount+=switchState;
      #endif
    #endif
  
    #if BaxisEnableSwitches
      #if bMinSwitch
        switchState=digitalRead(bMinSwitch);
        faultId[4][0]=switchState;
        faultCount+=switchState;
      #else
        if(bTargetPos<bVirtualMin){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[4][0]=switchState;
        faultCount+=switchState;
      #endif
      #if bMaxSwitch
        switchState=digitalRead(bMaxSwitch);
        faultId[4][1]=switchState;
        faultCount+=switchState;
      #else
        if(bTargetPos<bVirtualMax){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[4][1]=switchState;
        faultCount+=switchState;
      #endif
      #if bHomeSwitch
        switchState=digitalRead(bHomeSwitch);
        faultId[4][2]=switchState;
        faultCount+=switchState;
      #else
        if(bTargetPos>bVirtualHome&&bTargetPos<(bVirtualHome+2500)){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[4][2]=switchState;
        faultCount+=switchState;
      #endif
    #endif
  
    #if CaxisEnableSwitches
      #if cMinSwitch
        switchState=digitalRead(cMinSwitch);
        faultId[5][0]=switchState;
        faultCount+=switchState;
      #else
        if(cTargetPos<cVirtualMin){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[5][0]=switchState;
        faultCount+=switchState;
      #endif
      #if cMaxSwitch
        switchState=digitalRead(cMaxSwitch);
        faultId[5][1]=switchState;
        faultCount+=switchState;
      #else
        if(cTargetPos<cVirtualMax){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[5][1]=switchState;
        faultCount+=switchState;
      #endif
      #if cHomeSwitch
        switchState=digitalRead(cHomeSwitch);
        faultId[5][2]=switchState;
        faultCount+=switchState;
      #else
        if(cTargetPos>cVirtualHome&&cTargetPos<(cVirtualHome+2500)){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[5][2]=switchState;
        faultCount+=switchState;
      #endif
    #endif
  
    #if UaxisEnableSwitches
      #if uMinSwitch
        switchState=digitalRead(uMinSwitch);
        faultId[6][0]=switchState;
        faultCount+=switchState;
      #else
        if(uTargetPos<uVirtualMin){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[6][0]=switchState;
        faultCount+=switchState;
      #endif
      #if uMaxSwitch
        switchState=digitalRead(uMaxSwitch);
        faultId[6][1]=switchState;
        faultCount+=switchState;
      #else
        if(uTargetPos<uVirtualMax){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[6][1]=switchState;
        faultCount+=switchState;
      #endif
      #if uHomeSwitch
        switchState=digitalRead(uHomeSwitch);
        faultId[6][2]=switchState;
        faultCount+=switchState;
      #else
        if(uTargetPos>uVirtualHome&&uTargetPos<(uVirtualHome+2500)){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[6][2]=switchState;
        faultCount+=switchState;
      #endif
    #endif
  
    #if VaxisEnableSwitches
      #if vMinSwitch
        switchState=digitalRead(vMinSwitch);
        faultId[7][0]=switchState;
        faultCount+=switchState;
      #else
        if(vTargetPos<vVirtualMin){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[7][0]=switchState;
        faultCount+=switchState;
      #endif
      #if vMaxSwitch
        switchState=digitalRead(vMaxSwitch);
        faultId[7][1]=switchState;
        faultCount+=switchState;
      #else
        if(vTargetPos<vVirtualMax){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[7][1]=switchState;
        faultCount+=switchState;
      #endif
      #if vHomeSwitch
        switchState=digitalRead(vHomeSwitch);
        faultId[7][2]=switchState;
        faultCount+=switchState;
      #else
        if(vTargetPos>vVirtualHome&&vTargetPos<(vVirtualHome+2500)){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[7][2]=switchState;
        faultCount+=switchState;
      #endif
    #endif
  
    #if WaxisEnableSwitches
      #if wMinSwitch
        switchState=digitalRead(wMinSwitch);
        faultId[8][0]=switchState;
        faultCount+=switchState;
      #else
        if(wTargetPos<wVirtualMin){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[8][0]=switchState;
        faultCount+=switchState;
      #endif
      #if wMaxSwitch
        switchState=digitalRead(wMaxSwitch);
        faultId[8][1]=switchState;
        faultCount+=switchState;
      #else
        if(wTargetPos<wVirtualMax){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[8][1]=switchState;
        faultCount+=switchState;
      #endif
      #if wHomeSwitch
        switchState=digitalRead(wHomeSwitch);
        faultId[8][2]=switchState;
        faultCount+=switchState;
      #else
        if(wTargetPos>wVirtualHome&&wTargetPos<(wVirtualHome+2500)){
          switchState=1;
        }else{
          switchState=0;
        }
        faultId[8][2]=switchState;
        faultCount+=switchState;
      #endif
    #endif
    
    if(faultCount!=faultCountOld){ //One or more switches has changed.
      canMove=0;
      faultCountOld=faultCount;
      for(int i=0;i<9;i++){
        for(int e=0;e<3;e++){
          if(faultId[i][e]!=faultIdOld[i][e]){
            faultIdOld[i][e]=faultId[i][e];
            if(e==0){
              Serial.print("233 ");
              Serial.print(i);
              Serial.print(" ");
              Serial.print(faultId[i][e]);
              Serial.print(" ");
              Serial.print(233+i+faultId[i][e]);
              Serial.println(";");
            }
            if(e==1){
              Serial.print("231 ");
              Serial.print(i);
              Serial.print(" ");
              Serial.print(faultId[i][e]);
              Serial.print(" ");
              Serial.print(231+i+faultId[i][e]);
              Serial.println(";");
            }
            if(e==2){
              Serial.print("215 ");
              Serial.print(i);
              Serial.print(" ");
              Serial.print(faultId[i][e]);
              Serial.print(" ");
              Serial.print(215+i+faultId[i][e]);
              Serial.println(";");
            }
          }
        }
      }
    }else{
      canMove=1;
    }
  }
#endif
