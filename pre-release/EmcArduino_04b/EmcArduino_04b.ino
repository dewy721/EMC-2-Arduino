

/*
 EmcArduino version 0.02beta, this work is public domain.
 
 This BETA quality sketch is used for intergrating an
 Arduino with EMC2.4/AXIS as a CNC host. 
 
 Current Features:
   Idustry compliant G-Code Host via EMC w/tons of features.
   Automatic ATX power supply control.
   Blind Closed-loop feedback control. (Arduino controlled)
   LCD Support
   
 To Do List:
   Encoder to EMC communications for fully closed loop control.
   Stepper Support
   Spindle Support
   More axies EMC can go upto 9!!!
   Support for extras like coolant pumps, fans, and more buttons (like E-Stop.)
   Physical limit switches instead of just virtual ones.
   Multi-USB & I2C Support (technically there is no reason why one arduino has to do everything.)
   Multi-Machine control, fabshop, machine chaining.
     ie. One PC, 3 machines, running simultaneously in sync as an assembly line.
   
 Hardware requirments:
   1x PC with USB port and EMC Linux and installed.
   1x Arduino, Prefferably a Mega2560 (for more pin options.)
   1x Arduino Motor Shield (or an H-Bridge of you choice.)
   2x Encoders (or servos w/encoders)
   1x Plotter solenoid. To hold a pen for testing it out with.
  
 Credits:
 Jeff Epler http://axis.unpy.net/etchcnc for inspiring Colin Kingsbury to make the following.
 Colin Kingsbury for http://ckcnc.wordpress.com/basic-arduino-emc-howto/arduino-sketch/
             and for http://ckcnc.wordpress.com/basic-arduino-emc-howto/python-emc-module/
             and for http://ckcnc.wordpress.com/basic-arduino-emc-howto/hal-file-for-simple-arduino-example/
 Dan ? of http://www.marginallyclever.com/2011/10/controlling-your-arduino-through-the-serial-monitor/
 All the folks at http://www.linuxcnc.org/ for making the very excellent and very free EMC2.4/AXIS Linux distro.
 And lastly Duane Bishop (Myself) for playing with legos and hacking up printers. :)
//------------------------------------------------------------------------------

How to setup:

Create a text file and paste the following text into it:
*****************************************************************
*Step 1*************** Begin code snippet ***********************
*****************************************************************


        #!/usr/bin/python
        #    HAL userspace component to interface with Arduino board
        #    by Colin Kingsbury (http://ckcnc.wordpress.com_)
        #    Inspired by the earlier example from Jeff Epler
        #
        #    This program is free software; you can redistribute it and/or modify
        #    it under the terms of the GNU General Public License as published by
        #    the Free Software Foundation; either version 2 of the License, or
        #    (at your option) any later version.
        #
        #    This program is distributed in the hope that it will be useful,
        #    but WITHOUT ANY WARRANTY; without even the implied warranty of
        #    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        #    GNU General Public License for more details.
        #
        #    You should have received a copy of the GNU General Public License
        #    along with this program; if not, write to the Free Software
        #    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
        import serial
        import hal
        import sys
        import time
        
        #First we open the serial port. This should correspond to the port the Arduino
        #is connected to. This can be found in the Arduino IDE in Tools->Serial Port
        PORT = "/dev/ttyACM0"
        ser = serial.Serial(PORT, 115200, timeout=2)
        
        #Now we create the HAL component and its pins
        c = hal.component("arduino")
        c.newpin("switch-on",hal.HAL_BIT,hal.HAL_IN)
        c.newpin("switch-off",hal.HAL_BIT,hal.HAL_IN)
        c.newpin("machine-state",hal.HAL_BIT,hal.HAL_IN)
        c.newpin("axis0-cmd",hal.HAL_FLOAT,hal.HAL_IN)
        c.newpin("axis1-cmd",hal.HAL_FLOAT,hal.HAL_IN)
        c.newpin("axis2-cmd",hal.HAL_FLOAT,hal.HAL_IN)
        c.newpin("xMinLmt",hal.HAL_BIT,hal.HAL_IN)
        c.newpin("yMinLmt",hal.HAL_BIT,hal.HAL_IN)
        c.newpin("zMinLmt",hal.HAL_BIT,hal.HAL_IN)
        c.newpin("xMaxLmt",hal.HAL_BIT,hal.HAL_IN)
        c.newpin("yMaxLmt",hal.HAL_BIT,hal.HAL_IN)
        c.newpin("zMaxLmt",hal.HAL_BIT,hal.HAL_IN)
        time.sleep(1)
        c.ready()
        
        #We save the machine state (i.e. whether it's off or on) so that we only
        #send a message to the Arduino when it changes
        machineState = c['machine-state']
        axis0cmd = c['axis0-cmd']
        axis1cmd = c['axis1-cmd']
        axis2cmd = c['axis2-cmd']
        #c['xMinLmt'] = 1
        #c['xMaxLmt'] = 1
        #c['yMinLmt'] = 1
        #c['yMaxLmt'] = 1
        #c['zMinLmt'] = 1
        #c['zMaxLmt'] = 1
        #Check if the machine is on and set the LED accordingly
        if(machineState != 1):
          ser.write("+R")
        
        try:
          while 1:
            time.sleep(.001)
        
            axis0cmd = c['axis0-cmd'] * 1000000
            ser.write("jog X");
            ser.write(repr(axis0cmd))
            ser.write(";");
            ser.write("\n");
        
            axis1cmd = c['axis1-cmd'] * 1000000
            ser.write("jog Y");
            ser.write(repr(axis1cmd))
            ser.write(";");
            ser.write("\n");
        
            axis2cmd = c['axis2-cmd'] * 1000000
            ser.write("jog Z");
            ser.write(repr(axis2cmd))
            ser.write(";");
            ser.write("\n");
        
            #Check the machine State
            if(machineState != c['machine-state']):
              if(c['machine-state'] == 1):
                #The machine is on, so turn on the green LED and turn off the red one
                ser.write("+G")
                ser.write("-R")
              else:
                #opposite of above
                ser.write("-G")
                ser.write("+R")
              #update the machine state variable
              machineState = c['machine-state']
            #Check to see if we have a message waiting from the Arduino
            while ser.inWaiting():
              #This should be set to the length of whatever fixed-length message
              #you're sending from the arduino. It does not have to be the same length
              #as the outbound messages.
              key = ser.read(2)
              #The Arduino generates two different key events
              #One when the key is pressed down (+S) and another when it is released (-S)
              #In this case we are going to ignore the release
        
        # Set min limit triggers
              if(key == "x0"):
                c['xMinLmt'] = 1
              if(key == "y0"):
                c['yMinLmt'] = 1
              if(key == "z0"):
                c['zMinLmt'] = 1
        
        # Clear limit triggers
              if(key == "x1"):
                c['xMinLmt'] = 0
                c['xMaxLmt'] = 0
              if(key == "y1"):
                c['yMinLmt'] = 0
                c['yMaxLmt'] = 0
              if(key == "z1"):
                c['zMinLmt'] = 0
                c['zMaxLmt'] = 0
        
        # Set max limit triggers
              if(key == "x2"):
                c['xMaxLmt'] = 1
              if(key == "y2"):
                c['yMaxLmt'] = 1
              if(key == "z2"):
                c['zMaxLmt'] = 1
              if(key == "+S"):
                #If the machine is currently on, we turn it off, and vice-versa
                if(machineState == 1):
                  c['switch-on'] = 0
                  c['switch-off'] = 1
                else:
                  c['switch-on'] = 1
                  c['switch-off'] = 0
        
        except KeyboardInterrupt:
            ser.write("-R-G");
            raise SystemExit
        

*****************************************************************
*********************** End code snippet ************************
*****************************************************************
Now save the file as "arduino_emc".
Make it executable with: chmod +x arduino_emc
Copy to /usr/bin with: sudo cp arduino_emc /usr/bin


Run EMC2 Stepconf Wizard and setup your machine.
Afterwards:
Paste the following into ~/emc2/configs/my-mill/custom_postgui.hal
*****************************************************************
*Step 2*************** Begin code snippet ***********************
*****************************************************************

        loadusr -Wn arduino arduino_emc
        
        #Map the machine on/off state to the arduino
        net machstate arduino.machine-state <= halui.machine.is-on
        
        #Map the switch events to the halui pins for requesting on and off
        net turnoff halui.machine.off <= arduino.switch-off
        net turnon halui.machine.on <= arduino.switch-on
        
        #these lines send X,Y,Z co-ordinates to the arduino via the python script.
        net xpos-cmd axis.0.motor-pos-cmd => arduino.axis0-cmd
        net ypos-cmd axis.1.motor-pos-cmd => arduino.axis1-cmd
        net zpos-cmd axis.2.motor-pos-cmd => arduino.axis2-cmd
        
        #include these lines for arduino handled limit switches
        net min-x <= arduino.xMinLmt
        net min-y <= arduino.yMinLmt
        net min-z => arduino.zMinLmt
        net max-x <= arduino.xMaxLmt
        net max-y <= arduino.yMaxLmt
        net max-z => arduino.zMaxLmt

*****************************************************************
*********************** End code snippet ************************
*****************************************************************
Save and exit.


Step 3
Open ~/emc2/configs/my-mill/my-mill.hal
comment out the following lines that allow the default limit switch control via the parallel port. (unless you want both)

#net min-x <= parport.0.pin-10-in
#net min-y <= parport.0.pin-11-in
#net max-z <= parport.0.pin-12-in
#net max-x <= parport.0.pin-13-in
#net max-y <= parport.0.pin-15-in


*****************************************************************
Step 4
Set your pin config in the j_config tab above.
Set your preferences in the l_variables tab above.

*****************************************************************
Step 5
That's it! Your done! Grab some coffee, it's time to play with it! :)

*/



 

