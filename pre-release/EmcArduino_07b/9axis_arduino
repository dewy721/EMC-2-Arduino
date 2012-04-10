#!/usr/bin/python
#    HAL userspace component to interface with Arduino board
#    by Colin Kingsbury (http://ckcnc.wordpress.com_)
#    Inspired by the earlier example from Jeff Epler
#
#    Modified by Duane Bishop for use with EMC-2-Arduino
#	(https://github.com/dewy721/EMC-2-Arduino)
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
ser = serial.Serial(PORT, 115200, timeout=15)

#Now we create the HAL component and its pins
c = hal.component("arduino")
c.newpin("switch-on",hal.HAL_BIT,hal.HAL_IN)
c.newpin("switch-off",hal.HAL_BIT,hal.HAL_IN)
c.newpin("eStop",hal.HAL_BIT,hal.HAL_IN)
c.newpin("probe",hal.HAL_BIT,hal.HAL_IN)
c.newpin("spindleEnable",hal.HAL_BIT,hal.HAL_IN)
c.newpin("spindleDirection",hal.HAL_BIT,hal.HAL_IN)
c.newpin("spindleAtSpeed",hal.HAL_BIT,hal.HAL_IN)
c.newpin("coolantMist",hal.HAL_BIT,hal.HAL_IN)
c.newpin("coolantFlood",hal.HAL_BIT,hal.HAL_IN)
c.newpin("start",hal.HAL_BIT,hal.HAL_IN)
c.newpin("stop",hal.HAL_BIT,hal.HAL_IN)
c.newpin("pause",hal.HAL_BIT,hal.HAL_IN)
c.newpin("resume",hal.HAL_BIT,hal.HAL_IN)
c.newpin("step",hal.HAL_BIT,hal.HAL_IN)
c.newpin("machine-state",hal.HAL_BIT,hal.HAL_IN)
c.newpin("axis0-cmd",hal.HAL_FLOAT,hal.HAL_IN)
c.newpin("axis1-cmd",hal.HAL_FLOAT,hal.HAL_IN)
c.newpin("axis2-cmd",hal.HAL_FLOAT,hal.HAL_IN)
c.newpin("axis3-cmd",hal.HAL_FLOAT,hal.HAL_IN)
c.newpin("axis4-cmd",hal.HAL_FLOAT,hal.HAL_IN)
c.newpin("axis5-cmd",hal.HAL_FLOAT,hal.HAL_IN)
c.newpin("axis6-cmd",hal.HAL_FLOAT,hal.HAL_IN)
c.newpin("axis7-cmd",hal.HAL_FLOAT,hal.HAL_IN)
c.newpin("axis8-cmd",hal.HAL_FLOAT,hal.HAL_IN)
c.newpin("xMinLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("yMinLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("zMinLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("aMinLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("bMinLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("cMinLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("uMinLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("vMinLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("wMinLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("xMaxLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("yMaxLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("zMaxLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("aMaxLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("bMaxLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("cMaxLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("uMaxLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("vMaxLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("wMaxLmt",hal.HAL_BIT,hal.HAL_IN)
c.newpin("xHome",hal.HAL_BIT,hal.HAL_IN)
c.newpin("yHome",hal.HAL_BIT,hal.HAL_IN)
c.newpin("zHome",hal.HAL_BIT,hal.HAL_IN)
c.newpin("aHome",hal.HAL_BIT,hal.HAL_IN)
c.newpin("bHome",hal.HAL_BIT,hal.HAL_IN)
c.newpin("cHome",hal.HAL_BIT,hal.HAL_IN)
c.newpin("uHome",hal.HAL_BIT,hal.HAL_IN)
c.newpin("vHome",hal.HAL_BIT,hal.HAL_IN)
c.newpin("wHome",hal.HAL_BIT,hal.HAL_IN)
time.sleep(2)
c.ready()

#We save the machine state (i.e. whether it's off or on) so that we only
#send a message to the Arduino when it changes
machineState = c['machine-state']
eStopState = c['eStop']
axis0cmd = c['axis0-cmd']
axis1cmd = c['axis1-cmd']
axis2cmd = c['axis2-cmd']
axis3cmd = c['axis3-cmd']
axis4cmd = c['axis4-cmd']
axis4cmd = c['axis5-cmd']
axis6cmd = c['axis6-cmd']
axis7cmd = c['axis7-cmd']
axis8cmd = c['axis8-cmd']

#Check if the machine is on and set the LED accordingly
if(machineState != 1):
  ser.write("+P")

axis0cmdOld = 0;
axis1cmdOld = 0;
axis2cmdOld = 0;
axis3cmdOld = 0;
axis4cmdOld = 0;
axis5cmdOld = 0;
axis6cmdOld = 0;
axis7cmdOld = 0;
axis8cmdOld = 0;
coolantMistOld = 0;
coolantFloodOld = 0;
spindleEnableOld = 0;
spindleDirectionOld = 0;

try:
  while 1:
    time.sleep(.01)

    axis0cmd = c['axis0-cmd'];
    if axis0cmd != axis0cmdOld:
      axis0cmdOld = axis0cmd;
      axis0icmd = c['axis0-cmd'] * 1000000
      ser.write("jog X");
      ser.write(repr(axis0icmd))
      ser.write(";");
      ser.write("\n");

    axis1cmd = c['axis1-cmd'];
    if axis1cmd != axis1cmdOld:
      axis1cmdOld = axis1cmd;
      axis1icmd = c['axis1-cmd'] * 1000000
      ser.write("jog Y");
      ser.write(repr(axis1icmd))
      ser.write(";");
      ser.write("\n");

    axis2cmd = c['axis2-cmd'];
    if axis2cmd != axis2cmdOld:
      axis2cmdOld = axis2cmd;
      axis2icmd = c['axis2-cmd'] * 1000000
      ser.write("jog Z");
      ser.write(repr(axis2icmd))
      ser.write(";");
      ser.write("\n");

    axis3cmd = c['axis3-cmd'];
    if axis3cmd != axis3cmdOld:
      axis3cmdOld = axis3cmd;
      axis3icmd = c['axis3-cmd'] * 1000000
      ser.write("jog A");
      ser.write(repr(axis3icmd))
      ser.write(";");
      ser.write("\n");

    axis4cmd = c['axis4-cmd'];
    if axis4cmd != axis4cmdOld:
      axis4cmdOld = axis4cmd;
      axis4icmd = c['axis4-cmd'] * 1000000
      ser.write("jog B");
      ser.write(repr(axis4icmd))
      ser.write(";");
      ser.write("\n");

    axis5cmd = c['axis5-cmd'];
    if axis5cmd != axis5cmdOld:
      axis5cmdOld = axis5cmd;
      axis5icmd = c['axis5-cmd'] * 1000000
      ser.write("jog C");
      ser.write(repr(axis5icmd))
      ser.write(";");
      ser.write("\n");

    axis6cmd = c['axis6-cmd'];
    if axis6cmd != axis6cmdOld:
      axis6cmdOld = axis6cmd;
      axis6icmd = c['axis6-cmd'] * 1000000
      ser.write("jog U");
      ser.write(repr(axis6icmd))
      ser.write(";");
      ser.write("\n");

    axis7cmd = c['axis7-cmd'];
    if axis7cmd != axis7cmdOld:
      axis7cmdOld = axis7cmd;
      axis7icmd = c['axis7-cmd'] * 1000000
      ser.write("jog V");
      ser.write(repr(axis7icmd))
      ser.write(";");
      ser.write("\n");

    axis8cmd = c['axis8-cmd'];
    if axis8cmd != axis8cmdOld:
      axis8cmdOld = axis8cmd;
      axis8icmd = c['axis8-cmd'] * 1000000
      ser.write("jog W");
      ser.write(repr(axis8icmd))
      ser.write(";");
      ser.write("\n");

    spindleEnable = c['spindleEnable'];
    if spindleEnable != spindleEnableOld:
      spindleEnableOld = spindleEnable;
      if spindleEnable == 1:
        ser.write("+S;");
      else:
        ser.write("-S;");

    spindleDirection = c['spindleDirection'];
    if spindleDirection != spindleDirectionOld:
      spindleDirectionOld = spindleDirection;
      if spindleDirection == 1:
        ser.write("+D;");
      else:
        ser.write("-D;");



    coolantMist = c['coolantMist'];
    if coolantMist != coolantMistOld:
      coolantMistOld = coolantMist;
      if coolantMist == 1:
        ser.write("+M;");
      else:
        ser.write("-M;");

    coolantFlood = c['coolantFlood'];
    if coolantFlood != coolantFloodOld:
      coolantFloodOld = coolantFlood;
      if coolantFlood == 1:
        ser.write("+F;");
      else:
        ser.write("-F;");

    #Check the machine State
    if(machineState != c['machine-state']):
      if(c['machine-state'] == 1):
        #The machine is on, so turn on the power LED
        ser.write("+P;")
      else:
        #opposite of above
        ser.write("-P;")
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
      if(key == "a0"):
        c['aMinLmt'] = 1
      if(key == "x0"):
        c['bMinLmt'] = 1
      if(key == "y0"):
        c['cMinLmt'] = 1
      if(key == "z0"):
        c['uMinLmt'] = 1
      if(key == "a0"):
        c['vMinLmt'] = 1
      if(key == "a0"):
        c['wMinLmt'] = 1

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
      if(key == "a1"):
        c['aMinLmt'] = 0
        c['aMaxLmt'] = 0
      if(key == "b1"):
        c['bMinLmt'] = 0
        c['bMaxLmt'] = 0
      if(key == "c1"):
        c['cMinLmt'] = 0
        c['cMaxLmt'] = 0
      if(key == "u1"):
        c['uMinLmt'] = 0
        c['uMaxLmt'] = 0
      if(key == "v1"):
        c['vMinLmt'] = 0
        c['vMaxLmt'] = 0
      if(key == "w1"):
        c['wMinLmt'] = 0
        c['wMaxLmt'] = 0

# Set max limit triggers
      if(key == "x2"):
        c['xMaxLmt'] = 1
      if(key == "y2"):
        c['yMaxLmt'] = 1
      if(key == "z2"):
        c['zMaxLmt'] = 1
      if(key == "a2"):
        c['aMaxLmt'] = 1
      if(key == "b2"):
        c['bMaxLmt'] = 1
      if(key == "c2"):
        c['cMaxLmt'] = 1
      if(key == "u2"):
        c['uMaxLmt'] = 1
      if(key == "v2"):
        c['vMaxLmt'] = 1
      if(key == "w2"):
        c['wMaxLmt'] = 1

# set home switches
      if(key == "x4"):
        c['xHome'] = 0
      if(key == "y4"):
        c['yHome'] = 0
      if(key == "z4"):
        c['zHome'] = 0
      if(key == "a4"):
        c['aHome'] = 0
      if(key == "b4"):
        c['bHome'] = 0
      if(key == "c4"):
        c['cHome'] = 0
      if(key == "u4"):
        c['uHome'] = 0
      if(key == "v4"):
        c['vHome'] = 0
      if(key == "w4"):
        c['wHome'] = 0

# unset home switches
      if(key == "x5"):
        c['xHome'] = 1
      if(key == "y5"):
        c['yHome'] = 1
      if(key == "z5"):
        c['zHome'] = 1
      if(key == "a5"):
        c['aHome'] = 1
      if(key == "b5"):
        c['bHome'] = 1
      if(key == "c5"):
        c['cHome'] = 1
      if(key == "u5"):
        c['uHome'] = 1
      if(key == "v5"):
        c['vHome'] = 1
      if(key == "w5"):
        c['wHome'] = 1

#power
      if(key == "p0"):
        c['switch-on'] = 0
        c['switch-off'] = 1
      if(key == "p1"):
        c['switch-on'] = 1
        c['switch-off'] = 0
      if(key == "pt"):
        #If the machine is currently on, we turn it off, and vice-versa
        if(machineState == 1):
          c['switch-on'] = 1
          c['switch-off'] = 0
        else:
          c['switch-on'] = 0
          c['switch-off'] = 1

#eStop
      if(key == "e0"):
        c['eStop'] = 1
      if(key == "e1"):
        c['eStop'] = 0

#probe
      if(key == "P0"):
        c['probe'] = 1
      if(key == "P1"):
        c['probe'] = 0

#program start
      if(key == "h0"):
        c['start'] = 0
      if(key == "h1"):
        c['start'] = 1

#program stop
      if(key == "h2"):
        c['stop'] = 0
      if(key == "h3"):
        c['stop'] = 1

#program pause
      if(key == "h4"):
        c['pause'] = 1
      if(key == "h5"):
        c['pause'] = 0

#program resume
      if(key == "h6"):
        c['resume'] = 1
      if(key == "h7"):
        c['resume'] = 0

#program step
      if(key == "h8"):
        c['step'] = 1
      if(key == "h9"):
        c['step'] = 0

#spindle at speed signal
      if(key == "S1"):
        c['spindleAtSpeed'] = 1
      if(key == "S0"):
        c['spindleAtSpeed'] = 0

except KeyboardInterrupt:
    ser.write("-P;");
    raise SystemExit

