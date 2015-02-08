// This mod file enables support for quadrature encoders. (Including Jog Wheels.)
// Required library: http://www.pjrc.com/teensy/td_libs_Encoder.html

#define useEncoder false //enables the use of encoders

  #if useEncoder
    #include <Encoder.h> // Load the library, if needed.
   //Using hardware interrupt pins of the mega2560
   // for X & Y axises as a template.
   // Note: If your encoder is reading data in the wrong direction, just swap the A&B pin assignments.
    #define encoderXpinA 20
    #define encoderXpinB 21
    #define encoderXindexPin 0
    #define encoderYpinA 18
    #define encoderYpinB 19
    #define encoderYindexPin 0
    #define encoderZpinA 0
    #define encoderZpinB 0
    #define encoderZindexPin 0
    #define encoderApinA 0
    #define encoderApinB 0
    #define encoderAindexPin 0
    #define encoderBpinA 0
    #define encoderBpinB 0
    #define encoderBindexPin 0
    #define encoderCpinA 0
    #define encoderCpinB 0
    #define encoderCindexPin 0
    #define encoderUpinA 0
    #define encoderUpinB 0
    #define encoderUindexPin 0
    #define encoderVpinA 0
    #define encoderVpinB 0
    #define encoderVindexPin 0
    #define encoderWpinA 0
    #define encoderWpinB 0
    #define encoderWindexPin 0
    #define encoderJWpinA 0 // Jog wheel
    #define encoderJWpinB 0
  
    // Based on the entries above, compile in the only necessary instances if the library to conserve memory.
    #if encoderXpinA >= 1 && encoderXpinB >= 1
      #define encX true
      Encoder encoderX(encoderXpinA, encoderXpinB);
    #endif
    #if encoderYpinA >= 1 && encoderYpinB >= 1
      #define encY true
      Encoder encoderY(encoderYpinA,encoderYpinB);
    #endif
    #if encoderZpinA >= 1 && encoderZpinB >= 1
      #define encZ true
      Encoder encoderZ(encoderZpinA,encoderZpinB);
    #endif
    #if encoderApinA >= 1 && encoderApinB >= 1
      #define encA true
      Encoder encoderA(encoderApinA,encoderApinB);
    #endif
    #if encoderBpinA >= 1 && encoderBpinB >= 1
      #define encB true
      Encoder encoderB(encoderBpinA,encoderBpinB);
    #endif
    #if encoderCpinA >= 1 && encoderCpinB >= 1
      #define encC true
      Encoder encoderC(encoderCpinA,encoderCpinB);
    #endif
    #if encoderUpinA >= 1 && encoderUpinB >= 1
      #define encU true
      Encoder encoderU(encoderUpinA,encoderUpinB);
    #endif
    #if encoderVpinA >= 1 && encoderVpinB >= 1
      #define encV true
      Encoder encoderV(encoderVpinA,encoderVpinB);
    #endif
    #if encoderWpinA >= 1 && encoderWpinB >= 1
      #define encW true
      Encoder encoderW(encoderWpinA,encoderWpinB);
    #endif
    #if encoderJWpinA >= 1 && encoderJWpinB >= 1
      #define encJW true
      Encoder encoderJW(encoderJWpinA,encoderJWpinB);
    #endif
  
    // Some variables we'll need.
    boolean broadcastEncoderFeedback[10];
    double encoderPosition[10];
    double encoderPositionOld[10];
    void encoderSetup(){
      for(byte i=0;i<10;i++){
        encoderPosition[i]=0;
        encoderPositionOld[i]=0;
      }
      #if encX
        encoderX.write(0);
      #endif
      #if encY
        encoderY.write(0);
      #endif
      #if encZ
        encoderZ.write(0);
      #endif
      #if encA
        encoderA.write(0);
      #endif
      #if encB
        encoderB.write(0);
      #endif
      #if encC
        encoderC.write(0);
      #endif
      #if encU
        encoderU.write(0);
      #endif
      #if encV
        encoderV.write(0);
      #endif
      #if encW
        encoderW.write(0);
      #endif
      #if encJW
        encoderJW.write(0);
      #endif
      
      // Check commandsRequested to see if user wants encoder feedback output.
      if(stringSearch(commandsRequested,"229")){
        for(int i=0;i<10;i++){
          // If so, enable it for each axis listed in axisRequested.
          if(stringSearch(axisRequested,String(i))){
            broadcastEncoderFeedback[i]=true;
          }
        }
      }
    }
  
    void readEncoders(){
      #if encX
        encoderPosition[0]=encoderX.read();
      #endif
      #if encY
        encoderPosition[1]=encoderY.read();
      #endif
      #if encZ
        encoderPosition[2]=encoderZ.read();
      #endif
      #if encA
        encoderPosition[3]=encoderA.read();
      #endif
      #if encB
        encoderPosition[4]=encoderB.read();
      #endif
      #if encC
        encoderPosition[5]=encoderC.read();
      #endif
      #if encU
        encoderPosition[6]=encoderU.read();
      #endif
      #if encV
        encoderPosition[7]=encoderV.read();
      #endif
      #if encW
        encoderPosition[7]=encoderW.read();
      #endif
      #if encJW
        encoderPosition[9]=encoderJW.read();
      #endif
      long output;
      for(int i=0;i<10;i++){
        if(encoderPosition[i]!=encoderPositionOld[i]){
          if(broadcastEncoderFeedback[i]){
            Serial.print("229 ");
            Serial.print(i);
            Serial.print(" ");
            output=encoderPosition[i];
            Serial.print(output); // convert from double to long.
            Serial.print(" ");
            Serial.print(229+i+output);
            Serial.println(";");
          }
          encoderPositionOld[i]=encoderPosition[i];
        }
      }
    }
  #endif

