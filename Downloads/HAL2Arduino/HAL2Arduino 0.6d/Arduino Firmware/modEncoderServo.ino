// This mod file enables support for Quadrature encoded servo motors. (Not RC-Hobby servos!)
// Requires modEncoder.
// Requires Arduino PID Library: https://github.com/br3ttb/Arduino-PID-Library/archive/master.zip
// Info on how to use PID like a pro: http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

#define useEncoderServo false

#if useEncoder && useEncoderServo
  // Load in the PID library, if needed.
  #include <PID_v1.h>


  // dirPin = direction. (required)
  // pwmPin = power output level. (recommended)
  // enablePin = you guessed it. (recommended)
  // minPower = min amps allowed. (optional, 0~255 must be lower than maxPower)
  // maxPower = max amps allowed. (optional, 0~255 must be higher minPower) Duh.
  // Values of 0 = off/disabled/ignore/stfu
  
  // Default pin settings for the official Arduino Motor Shield R3 for both X & Y.
  #define encServoPollingTime 1 // in mlliseconds.
  
  #define XdirPin 13
  #define XpwmPin 11
  #define XenablePin 8
  #define XminPower 0
  #define XmaxPower 100
  #define Xdeadband 10
  #define Xinvert false
  #define Xenvert false
  #define Xscale 10
  #define Xproportional 1
  #define Xintegral 0.5
  #define Xderivative 0.5

  #define YdirPin 12
  #define YpwmPin 3
  #define YenablePin 9
  #define YminPower 0
  #define YmaxPower 100
  #define Ydeadband 1
  #define Yinvert false
  #define Yenvert false
  #define Yscale 10
  #define Yproportional 1
  #define Yintegral 0.5
  #define Yderivative 0.4

  #define ZdirPin 0
  #define ZpwmPin 0
  #define ZenablePin 0
  #define ZminPower 0
  #define ZmaxPower 0
  #define Zdeadband 0
  #define Zinvert false
  #define Zenvert false
  #define Zproportional 2
  #define Zintegral 5
  #define Zderivative 1

  #define AdirPin 0
  #define ApwmPin 0
  #define AenablePin 0
  #define AminPower 0
  #define AmaxPower 0
  #define Adeadband 0
  #define Ainvert false
  #define Aenvert false
  #define Aproportional 2
  #define Aintegral 5
  #define Aderivative 1

  #define BdirPin 0
  #define BpwmPin 0
  #define BenablePin 0
  #define BminPower 0
  #define BmaxPower 0
  #define Bdeadband 0
  #define Binvert false
  #define Benvert false
  #define Bproportional 2
  #define Bintegral 5
  #define Bderivative 1

  #define CdirPin 0
  #define CpwmPin 0
  #define CenablePin 0
  #define CminPower 0
  #define CmaxPower 0
  #define Cdeadband 0
  #define Cinvert false
  #define Cenvert false
  #define Cproportional 2
  #define Cintegral 5
  #define Cderivative 1

  #define UdirPin 0
  #define UpwmPin 0
  #define UenablePin 0
  #define UminPower 0
  #define UmaxPower 0
  #define Udeadband 0
  #define Uinvert false
  #define Uenvert false
  #define Uproportional 2
  #define Uintegral 5
  #define Uderivative 1

  #define VdirPin 0
  #define VpwmPin 0
  #define VenablePin 0
  #define VminPower 0
  #define VmaxPower 0
  #define Vdeadband 0
  #define Vinvert false
  #define Venvert false
  #define Vproportional 2
  #define Vintegral 5
  #define Vderivative 1

  #define WdirPin 0
  #define WpwmPin 0
  #define WenablePin 0
  #define WminPower 0
  #define WmaxPower 0
  #define Wdeadband 0
  #define Winvert false
  #define Wenvert false
  #define Wproportional 2
  #define C 5
  #define Wderivative 1

  byte encServoDirPin[9];
  byte encServoPwmPin[9];
  byte encServoEnablePin[9];
  byte encServoMinPwm[9];
  byte encServoMaxPwm[9];
  byte encServoDeadband[9];
  boolean encServoInversion[9];
  boolean encServoEnversion[9];
  unsigned long dist[9];
  unsigned long servoError[9];
  long servoScale[9];
  double Setpoint[9], Input[9], Output[9];
  
  #if XdirPin > 0 && XpwmPin > 0
    PID xPID(&Input[0], &Output[0], &Setpoint[0],Xproportional,Xproportional,Xderivative, REVERSE);
  #endif

  #if YdirPin > 0 && YpwmPin > 0
    PID yPID(&Input[1], &Output[1], &Setpoint[1],Yproportional,Yproportional,Yderivative, REVERSE);
  #endif

  #if ZdirPin > 0 && ZpwmPin > 0
    PID zPID(&Input[2], &Output[2], &Setpoint[2],Zproportional,Zproportional,Zderivative, REVERSE);
  #endif

  #if AdirPin > 0 && ApwmPin > 0
    PID aPID(&Input[3], &Output[3], &Setpoint[3],Aproportional,Aproportional,Aderivative, REVERSE);
  #endif

  #if BdirPin > 0 && BpwmPin > 0
    PID bPID(&Input[4], &Output[4], &Setpoint[4],Bproportional,Bproportional,Bderivative, REVERSE);
  #endif

  #if CdirPin > 0 && CpwmPin > 0
    PID cPID(&Input[5], &Output[5], &Setpoint[5],Cproportional,Cproportional,Cderivative, REVERSE);
  #endif

  #if UdirPin > 0 && UpwmPin > 0
    PID uPID(&Input[6], &Output[6], &Setpoint[6],Uproportional,Uproportional,Uderivative, REVERSE);
  #endif

  #if VdirPin > 0 && VpwmPin > 0
    PID vPID(&Input[7], &Output[7], &Setpoint[7],Vproportional,Vproportional,Vderivative, REVERSE);
  #endif

  #if WdirPin > 0 && WpwmPin > 0
    PID wPID(&Input[8], &Output[8], &Setpoint[8],Wproportional,Wproportional,Wderivative, REVERSE);
  #endif

  
  void encoderServoSetup(){
    for(int i=0;i<9;i++){
      dist[i]=0;
      servoError[i]=0;
      servoScale[i]=1;
      encServoDirPin[i]=0;
      encServoPwmPin[i]=0;
      encServoEnablePin[i]=0;
      encServoMinPwm[i]=0;
      encServoMaxPwm[i]=0;
      encServoDeadband[i]=0;
      encServoInversion[i]=0;
    }
    #if XdirPin > 0 && encX == true
      #if XdirPin
        encServoDirPin[0]=XdirPin;
      #endif
      #if XpwmPin
        encServoPwmPin[0]=XpwmPin;
      #endif
      #if XenablePin
        encServoEnablePin[0]=XenablePin;
      #endif
      #if XminPower
        encServoMinPwm[0]=XminPower;
      #endif
      #if XmaxPower
        encServoMaxPwm[0]=XmaxPower;
      #endif
      #if Xdeadband
        encServoDeadband[0]=Xdeadband;
      #endif
      #if Xinvert
        encServoInversion[0]=Xinvert;
      #endif
      #if Xscale
        servoScale[0]=Xscale;
      #endif
    #endif
    #if YdirPin > 0 && encY == true
      #if YdirPin
        encServoDirPin[1]=YdirPin;
      #endif
      #if YpwmPin
        encServoPwmPin[1]=YpwmPin;
      #endif
      #if YenablePin
        encServoEnablePin[1]=YenablePin;
      #endif
      #if YminPower
        encServoMinPwm[1]=YminPower;
      #endif
      #if YmaxPower
        encServoMaxPwm[1]=YmaxPower;
      #endif
      #if Ydeadband
        encServoDeadband[1]=Ydeadband;
      #endif
      #if Yinvert
        encServoInversion[1]=Yinvert;
      #endif
      #if Yscale
        servoScale[1]=Yscale;
      #endif
    #endif
    #if ZdirPin > 0 && encZ == true
      #if ZdirPin
        encServoDirPin[2]=ZdirPin;
      #endif
      #if ZpwmPin
        encServoPwmPin[2]=ZpwmPin;
      #endif
      #if ZenablePin
        encServoEnablePin[2]=ZenablePin;
      #endif
      #if ZminPower
        encServoMinPwm[2]=ZminPower;
      #endif
      #if ZmaxPower
        encServoMaxPwm[2]=ZmaxPower;
      #endif
      #if Zdeadband
        encServoDeadband[2]=Zdeadband;
      #endif
      #if Zinvert
        encServoInversion[2]=Zinvert;
      #endif
      #if Zscale
        servoScale[2]=Zscale;
      #endif
    #endif
    #if AdirPin > 0 && encA == true
      #if AdirPin
        encServoDirPin[3]=AdirPin;
      #endif
      #if ApwmPin
        encServoPwmPin[3]=ApwmPin;
      #endif
      #if AenablePin
        encServoEnablePin[3]=AenablePin;
      #endif
      #if AminPower
        encServoMinPwm[3]=AminPower;
      #endif
      #if AmaxPower
        encServoMaxPwm[3]=AmaxPower;
      #endif
      #if Adeadband
        encServoDeadband[3]=Adeadband;
      #endif
      #if Ainvert
        encServoInversion[3]=Ainvert;
      #endif
      #if Ascale
        servoScale[3]=Ascale;
      #endif
    #endif
    #if BdirPin > 0 && encB == true
      #if BdirPin
        encServoDirPin[4]=BdirPin;
      #endif
      #if BpwmPin
        encServoPwmPin[4]=BpwmPin;
      #endif
      #if BenablePin
        encServoEnablePin[4]=BenablePin;
      #endif
      #if BminPower
        encServoMinPwm[4]=BminPower;
      #endif
      #if BmaxPower
        encServoMaxPwm[4]=BmaxPower;
      #endif
      #if Bdeadband
        encServoDeadband[4]=Bdeadband;
      #endif
      #if Binvert
        encServoInversion[4]=Binvert;
      #endif
      #if Bscale
        servoScale[4]=Bscale;
      #endif
    #endif
    #if CdirPin > 0 && encC == true
      #if CdirPin
        encServoDirPin[5]=CdirPin;
      #endif
      #if CpwmPin
        encServoPwmPin[5]=CpwmPin;
      #endif
      #if CenablePin
        encServoEnablePin[5]=CenablePin;
      #endif
      #if CminPower
        encServoMinPwm[5]=CminPower;
      #endif
      #if CmaxPower
        encServoMaxPwm[5]=CmaxPower;
      #endif
      #if Cdeadband
        encServoDeadband[5]=Cdeadband;
      #endif
      #if Cinvert
        encServoInversion[5]=Cinvert;
      #endif
      #if Cscale
        servoScale[5]=Cscale;
      #endif
    #endif
    #if UdirPin > 0 && encU == true
      #if UdirPin
        encServoDirPin[6]=UdirPin;
      #endif
      #if UpwmPin
        encServoPwmPin[6]=UpwmPin;
      #endif
      #if UenablePin
        encServoEnablePin[6]=UenablePin;
      #endif
      #if UminPower
        encServoMinPwm[6]=UminPower;
      #endif
      #if UmaxPower
        encServoMaxPwm[6]=UmaxPower;
      #endif
      #if Udeadband
        encServoDeadband[6]=Udeadband;
      #endif
      #if Uinvert
        encServoInversion[6]=Uinvert;
      #endif
      #if Uscale
        servoScale[6]=Uscale;
      #endif
    #endif
    #if VdirPin > 0 && encV == true
      #if VdirPin
        encServoDirPin[7]=VdirPin;
      #endif
      #if VpwmPin
        encServoPwmPin[7]=VpwmPin;
      #endif
      #if VenablePin
        encServoEnablePin[7]=VenablePin;
      #endif
      #if VminPower
        encServoMinPwm[7]=VminPower;
      #endif
      #if VmaxPower
        encServoMaxPwm[7]=VmaxPower;
      #endif
      #if Vdeadband
        encServoDeadband[7]=Vdeadband;
      #endif
      #if Vinvert
        encServoInversion[7]=Vinvert;
      #endif
      #if Vscale
        servoScale[7]=Vscale;
      #endif
    #endif
    #if WdirPin > 0 && encW == true
      #if WdirPin
        encServoDirPin[8]=WdirPin;
      #endif
      #if WpwmPin
        encServoPwmPin[8]=WpwmPin;
      #endif
      #if WenablePin
        encServoEnablePin[8]=WenablePin;
      #endif
      #if WminPower
        encServoMinPwm[8]=WminPower;
      #endif
      #if WmaxPower
        encServoMaxPwm[8]=WmaxPower;
      #endif
      #if Wdeadband
        encServoDeadband[8]=Wdeadband;
      #endif
      #if Winvert
        encServoInversion[8]=Winvert;
      #endif
      #if Wscale
        servoScale[8]=Wscale;
      #endif
    #endif
    for(int i=0;i<9;i++){
      if(encServoDirPin[i]!=0){pinMode(encServoDirPin[i],OUTPUT);}
      if(encServoEnablePin[i]!=0){pinMode(encServoEnablePin[i],OUTPUT);}
      Setpoint[i]=encServoDeadband[i];
    }
    #if XdirPin > 0 && XpwmPin > 0
      xPID.SetSampleTime(1);
      xPID.SetOutputLimits(XminPower,XmaxPower);
      xPID.SetMode(AUTOMATIC);
    #endif
    #if YdirPin > 0 && YpwmPin > 0
      yPID.SetSampleTime(10);
      yPID.SetOutputLimits(YminPower,YmaxPower);
      yPID.SetMode(AUTOMATIC);
    #endif
    #if ZdirPin > 0 && ZpwmPin > 0
      zPID.SetSampleTime(10);
      zPID.SetOutputLimits(ZminPower,ZmaxPower);
      zPID.SetMode(AUTOMATIC);
    #endif
    #if AdirPin > 0 && ApwmPin > 0
      aPID.SetSampleTime(10);
      aPID.SetOutputLimits(AminPower,AmaxPower);
      aPID.SetMode(AUTOMATIC);
    #endif
    #if BdirPin > 0 && BpwmPin > 0
      bPID.SetSampleTime(10);
      bPID.SetOutputLimits(BminPower,BmaxPower);
      bPID.SetMode(AUTOMATIC);
    #endif
    #if CdirPin > 0 && CpwmPin > 0
      cPID.SetSampleTime(10);
      cPID.SetOutputLimits(CminPower,CmaxPower);
      cPID.SetMode(AUTOMATIC);
    #endif
    #if UdirPin > 0 && UpwmPin > 0
      uPID.SetSampleTime(10);
      uPID.SetOutputLimits(UminPower,UmaxPower);
      uPID.SetMode(AUTOMATIC);
    #endif
    #if VdirPin > 0 && VpwmPin > 0
      vPID.SetSampleTime(10);
      vPID.SetOutputLimits(VminPower,VmaxPower);
      vPID.SetMode(AUTOMATIC);
    #endif
    #if WdirPin > 0 && WpwmPin > 0
      wPID.SetSampleTime(10);
      wPID.SetOutputLimits(WminPower,WmaxPower);
      wPID.SetMode(AUTOMATIC);
    #endif
  }
  
  unsigned long encServoTime=millis();
  unsigned long encServoTimeOld=millis();
  void moveEncoderServos(){
    for(int i=0;i<2;i++){
      if(encoderPosition[i]<(targetPos[i]/servoScale[i])){
        digitalWrite(encServoDirPin[i],!encServoInversion[i]);
      }else{
        digitalWrite(encServoDirPin[i],encServoInversion[i]);
      }
      long dist=getDist(encoderPosition[i],targetPos[i]);
      if(dist<encServoDeadband[i]){
        Input[i]=0;
      }else{
        Input[i]=dist;
      }
//      Input[i]=getDist(encoderPosition[i],targetPos[i]);
      #if XdirPin > 0 && XpwmPin > 0
        xPID.Compute();
      #endif
      #if YdirPin > 0 && YpwmPin > 0
        yPID.Compute();
      #endif
      #if ZdirPin > 0 && ZpwmPin > 0
        zPID.Compute();
      #endif
      #if AdirPin > 0 && ApwmPin > 0
        aPID.Compute();
      #endif
      #if BdirPin > 0 && BpwmPin > 0
        bPID.Compute();
      #endif
      #if CdirPin > 0 && CpwmPin > 0
        cPID.Compute();
      #endif
      #if UdirPin > 0 && UpwmPin > 0
        uPID.Compute();
      #endif
      #if VdirPin > 0 && VpwmPin > 0
        vPID.Compute();
      #endif
      #if WdirPin > 0 && WpwmPin > 0
        wPID.Compute();
      #endif
      analogWrite(encServoPwmPin[i],Output[i]);
    }
  }

  void moveEncoderServosOld(){
    encServoTime=millis();
    if(encServoTime-encServoTimeOld>encServoPollingTime){
      for(int i=0;i<2;i++){
        if(encServoDirPin[i]>0){
          long dist=getDist((targetPos[i]/servoScale[i]),encoderPosition[i]);
          if(dist>encServoDeadband[i]){
            if(encoderPosition[i]<(targetPos[i]/servoScale[i])){
              digitalWrite(encServoDirPin[i],!encServoInversion[i]);
            }else{
              digitalWrite(encServoDirPin[i],encServoInversion[i]);
            }
            digitalWrite(encServoEnablePin[i],encServoEnversion[i]);
            servoError[i]++;
            analogWrite(encServoPwmPin[i],constrain(dist+servoError[i]*2,encServoMinPwm[i],encServoMaxPwm[i]));
          }else{
            servoError[i]=servoError[i]/2;
            digitalWrite(encServoEnablePin[i],!encServoEnversion[i]);
            analogWrite(encServoPwmPin[i],0); //encServoMinPwm[i]);
          }
        }
      }
      encServoTimeOld=encServoTime;
    }
  }
#endif
