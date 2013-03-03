#!/usr/bin/python

#    HAL2Arduino
#    This user HAL space component is meant primarily as a 'breakout' interface
#    between HAL and the Arduino platform.

#    Note: This interface will always have (at least) serveral milliseconds of
#    lag, compared to the standard parallel port latency with lag times of just
#    nanoseconds.

#    Through it is possible to approximate a CNC controller via this interface
#    it is best used when considered as:
#     A.    Experimental.
#     B.    Not intended for production use.
#     C.    Cannot compete with a real controller vs. speed/cost/accuracy.
#     D.    Best thought of as a 'Duct Tape' interface.
#     E.    Should NEVER be used for THREADING or SYNCRONIZED operations.

#    With that said, it IS good for:
#     A.    Bootstrapping DIY toy CNC machines for existing Arduino owners
#               that would just like to "try stuff out".
#     B.    Hobby grade CNC 3-axis wood router/plasma/printing/plotting tables.
#     C.    Temporary addons to existing CNC machines.
#     D.    Can easily be made to automate tool changers.
#     E.    Interfacing of non-timing critical CNC subsystems such as pumps,
#           pendant controls, LCD displays and such.

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

import sys, string
#We only need some of the functions from the following modules
from thread import start_new_thread, exit
from Queue import Queue
from serial import Serial
from time import sleep
from tkMessageBox import showinfo
from Tkinter import Tk
from decimal import *
# To locate the corresponding arduino(s) with acceptible firmware.
# We'll need to scan for any/all of them.
firmware = "HAL-2-Arduino"
firmwareVersion = 0.4
#maxUnits default is 10, up to 255.
maxClients = 10
global codesAccepted
global axisesRequested
codesAccepted = ""
axisesRequested = ""
global commandList
global listOfCommands
global listOfAxis
global c

commandList = []
simulation = False #Enable for console debugging.
linkData = ""
listOfPorts = [] #human readable
listOfCommands = []
listOfAxis = []
listOfUnits = []
linkMsg=""
maxQueSize=1000
masterTx = Queue(maxQueSize)
masterRx = Queue(maxQueSize)
clientQueueArray = []
threadsRun=True

# We'll need an arrary to hold the previous states of upto 392 separate variables.
# Note: hostCheckOld[0~99] are not used per default setup and are availible for custom usage.
hostCheckOld=range(0,313)
# Commands 200~234 will need to have sub arrays to hold each axis...
for command in range(200,235):
    hostCheckOld[command]=range(0,9)

#set floats to 4 digits of precision. 1/10000ths of an inch (or mm).
getcontext().prec = 4
# Raise floats to whole numbers, because Arduinos will choke on floats
floatMultiplier=10000

if simulation == False:
    import hal # LinuxCNC must be up and running pior to loading this.
    c = hal.component("Hal2Arduino")



def msgBox(message):
    window = Tk()
    string = str(message)
    showinfo(title=firmware, message=string)
    window.wm_withdraw()
    #showinfo.lower()

def comSetup(ser, command, data1, data2):
    lastChecksum=command + data1 + data2
    lastCommand="%s %s %s %s%s" % (str(command), str(data1), str(data2), str(lastChecksum), ";")
    ser.write(lastCommand)
    for i in range(0,6): # attemp to send a command, but no more then 5 times.
        linkMsg = ser.readline()
        if linkMsg.find('resend') > -1:
            ser.flushInput()
            ser.write(lastCommand)
        else: # Command sent successfully.
            return "%s" % linkMsg.rstrip('\r\n')

def comThread(port):
    global threadsRun
    global masterRx
    try:
        ser = Serial(port, 115200, timeout=0.05)
        sleep(2.25) # Wait for arduino to boot.....
        linkMsg = ser.readline()
        if linkMsg.find('ok') > -1:
            listOfPorts.append(port)
            clientQueueArray.append(Queue(maxQueSize))
            print "Found device on %s\nAsking for firmware info." % port
            print "Firmware:",
            if comSetup(ser, 990, 0, 0) == firmware:
                print firmware
                unitId=comSetup(ser, 992, 0, 0)
                if unitId != "":
                    print "Unit: %r" % unitId
                    listOfUnits.append(unitId)
                print "Version: ",
                version=float(comSetup(ser, 991, 0, 0))
                if version >= firmwareVersion:
                    print "%.1f" % version
                    codesAccepted = comSetup(ser, 993, 0, 0)
                    listOfCommands.append(codesAccepted)
                    axisesRequested = comSetup(ser, 994, 0, 0)
                    listOfAxis.append(axisesRequested)
                    commandList.append("")
                    #locate this unit's position within the arrays.
                    for n in range(0,len(listOfPorts)):
                        if listOfPorts[n] == port:
                            arrayPos=n
                            pMsg=str(clientQueueArray[n])
                            pMsg=pMsg.replace('<Queue.Queue instance at ', '')
                            pMsg=pMsg.rstrip('>')
                            print "comThread: port=%s\tarrayPos:%s\tQueue:%s" % (port,n,pMsg)
                    clientQueue=clientQueueArray[arrayPos]
                    # Client init complete. Proceed to operational loop.
                    while threadsRun == True:
                        while ser.inWaiting() > 0: # Listen for incomming data when txBuffer is empty
                            linkMsg = ser.readline()
                            if linkMsg.find('\r\n') > -1: # is message comeplete with EOL string.
                                linkMsg=linkMsg.rstrip('\r\n') # Omit EOL string from message.
                                masterRx.put(linkMsg) # return filtered data to main buffer for processing.
                                print "comThread: Received %s" % linkMsg
                                lnkMsgPart = ""
                            else:
                                linkMsgPart = linkMsgPart+linkMsg 
                        while not clientQueue.empty(): # txBuffer has data, filter it, then perhaps send it out and listen to reponse.
                            lastCommand=clientQueue.get()
                            thisCmdString=lastCommand.rstrip(";")
                            thisCmd=thisCmdString.split()
                            cmd=int(thisCmd[0])
                            #Is this command a '900' series (device query)?
                            if cmd > 899 and cmd < 1000:
                                send=True
                            #Or is this command specifically supported by this device?
                            elif codesAccepted.find(str(cmd)) > -1:
                                send=True
                            #I know, maybe its an '800' command for clientside global broadcasts?
                            # ie. "800 807 1 1608" aka: Ok(800), who has coffeePot(807), anybody?(0), seriously(1607)
                            # ie2: Unit 12 could respond with "800 807 12 1619;" aka: Yells out blindly(800), Coffee making brought to you by Unit (12), just sayin'(1619).
                            elif cmd == 800:
                                send=True
                            #Could this be a custom "801~899" command to this unit's Id? (For targeted data transmissions.)
                            # ie, User pendant issues something like "807 12 1 820;" (Aka: "Hey unit=12,  cmd=makeCoffee, value=1(yes), checksum=Thanx)
                            elif cmd > 800 and cmd < 900 and unitId.find(thisCmd[1]) > -1:
                                send=True
                            #Nope. Just some spam in the buffer, don't waste bandwidth on it.
                            else:
                                send=False
                            if send:
                                ser.write(lastCommand)
                                for i in range(0,6): # attemp to send a command, but no more then 5 times.
                                    sleep(0.005) # Give the Arduino 5 millisec (per attempt) to respond.
                                    linkMsg = ser.readline()
                                    if linkMsg.find('resend') > -1: # received a "What?" from Arduino.
                                        ser.flushInput() # possibly some garbage too. So pitch it.
                                        print "comThread: Re-sending %s to %s" % (lastCommand, port)
                                        ser.write(lastCommand)
                                    else: # Command sent successfully, report result only once.
#                                        print "comThread: Sent %s to %s" % (lastCommand, port)
                                        if linkMsg.find(';\r\n') > -1: # is message comeplete with EOL string.
                                            linkMsg=linkMsg.rstrip(';\r\n') # Omit EOL string from message.
                                            masterRx.put(linkMsg) # return filtered data to main buffer for processing.
                                            print "comThread: Received %s" % linkMsg
                                        break
                                        commandList[arrayPos] = ""
                            clientQueue.task_done()
                        if not threadsRun:
                            print "Closing port: %s" % port
                            ser.close()
                            exit()
                else:
                    msgBox("\aConnection Error:\nUnit %s needs firmware version %s or higher." % (unitId, firmwareVersion))
                    threadsRun=False
                    print "Failed. Unit %s needs firmware version %s or higher." % (unitId, firmwareVersion)
                    ser.close()
                    raise SystemExit
#                    exit()
            else:
                print "Failed. %s not found on this device\n" % firmware
                ser.close()
                exit()
    except Exception:
        if not threadsRun:
            ser.close()
            print "Closing port: %s" % port
        exit()

    finally:
        if not threadsRun:
            ser.close()
            print "Closing port: %s" % port
        exit()

def commandHandler(codesAccepted, axisesRequested):
    if not simulation:
        sleep(2)
        c.ready()

    while threadsRun:
        while not masterRx.empty():
            msg=masterRx.get()
            try:
                msgj=msg.rstrip(';')
                dataSet=[]
                dataSet=msgj.split()
                #dataSet=msg.split()
                for i in range(0,len(dataSet)):
                    if i == 2:
                        if dataSet[i].find('.') > -1: #If float, make it one.
                            dataSet[i]=Decimal(dataSet[i])
                        else:
                            dataSet[i]=int(dataSet[i]) # Just another integer.
                buildSum=int(dataSet[0]) + int(dataSet[1]) + int(dataSet[2])
                print "commandHandler: received %s" % msg
                print "commandHandler: interpreted as %d %d %d %d" % (int(dataSet[0]), int(dataSet[1]), int(dataSet[2]), buildSum)
                if  buildSum == int(dataSet[3]):
                    print "commandHandler: checkSum passed"
                    if int(dataSet[0]) >= 800 and int(dataSet[0]) <= 899: #Client to Client traffic.
                        #Broadcast it to all clients.
                        print "commandHandler: Clientside event:%r" % msg
                        masterTx.put(msg)
                    elif int(dataSet[0]) > 100 and int(dataSet[0]) < 400: # Inbound data for the Host.
                        #Process it.
                        command=int(dataSet[0])
                        axis=int(dataSet[1])
                        # Need parsing option of int vs. Decimal for 'val'
                        val=int(dataSet[2])
                        print "commandHandler: Got %d %d %d %d" % (command, axis, val, buildSum)
                        if command == 100:
                            c['motion_adaptive-feed'] = val

                        if command == 101:
                            c['motion_analog-in-00'] = val

                        if command == 102:
                            c['motion_analog-in-01'] = val

                        if command == 103:
                            c['motion_analog-in-02'] = val

                        if command == 104:
                            c['motion_analog-in-03'] = val

                        if command == 112:
                            c['motion_digital-in-00'] = val

                        if command == 113:
                            c['motion_digital-in-01'] = val

                        if command == 114:
                            c['motion_digital-in-02'] = val

                        if command == 115:
                            c['motion_digital-in-03'] = val

                        if command == 121:
                            c['motion_enable'] = val

                        if command == 122:
                            c['motion_feed-hold'] = val

                        if command == 126:
                            c['motion_probe-input'] = val

                        if command == 129:
                            c['motion_spindle-at-speed'] = val

                        if command == 132:
                            c['motion_spindle-index-enable'] = val

                        if command == 135:
                            c['motion_spindle-revs'] = val

                        if command == 136:
                            c['motion_spindle-speed-in'] = val

                        if command == 150:
                            c['motion-command-handler_tmax'] = val

                        if command == 152:
                            c['motion-controller_tmax'] = val

                        if command == 163:
                            c['motion_servo_overruns'] = val

                        if command == 202:
                            input="axis_" + str(axis) + "_amp-fault-in"
                            c[input] = val

                        if command == 215:
                            input="axis_" + str(axis) + "_home-sw-in"
                            c[input] = val
                            print "%s = %d" % (input, val)

                        if command == 219:
                            input="axis_" + str(axis) + "_index-enable"
                            c[input] = val

                        if command == 220:
                            input="axis_" + str(axis) + "_jog-counts"
                            c[input] = val

                        if command == 221:
                            input="axis_" + str(axis) + "_jog-enable"
                            c[input] = val

                        if command == 222:
                            input="axis_" + str(axis) + "_jog-scale"
                            c[input] = val

                        if command == 223:
                            input="axis_" + str(axis) + "_jog-vel-mode"
                            c[input] = val

                        if command == 229:
                            input="axis_" + str(axis) + "_motor-pos-fb"
                            c[input] = val

                        if command == 231:
                            input="axis_" + str(axis) + "_pos-lim-sw-in"
                            c[input] = val

                        if command == 233:
                            input="axis_" + str(axis) + "_neg-lim-sw-in"
                            c[input] = val

                        if command == 302:
                            c['iocontrol_emc-enable-in'] = val

                        if command == 304:
                            c['iocontrol_lube_level'] = val

                        if command == 306:
                            c['iocontrol_tool-changed'] = val

                        if command == 310:
                            c['iocontrol_tool-prepared'] = val

                else:
                    print "commandHandler: INVALID COMMAND, command:%d axis:%d val:%d sum:%d" % (dataSet[0], dataSet[1], dataSet[2], dataSet[3])
            except Exception:
                print "commandHandler: INVALID COMMAND, format error: %r" % msg
                pass
            finally:
                masterRx.task_done()

# ## outputs here
        if codesAccepted.find("105") > -1:
            command=105
            hostCheck=c['motion_analog-out-00']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("106") > -1:
            command=106
            hostCheck=c['motion_analog-out-01']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("107") > -1:
            command=107
            hostCheck=c['motion_analog-out-02']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("108") > -1:
            command=108
            hostCheck=c['motion_analog-out-03']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("109") > -1:
            command=109
            hostCheck=c['motion_coord-error']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("110") > -1:
            command=110
            hostCheck=c['motion_coord-mode']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("111") > -1:
            command=111
            hostCheck=c['motion_current-vel']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("116") > -1:
            command=116
            hostCheck=c['motion_digital-out-00']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("117") > -1:
            command=117
            hostCheck=c['motion_digital-out-01']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("118") > -1:
            command=118
            hostCheck=c['motion_digital-out-02']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("119") > -1:
            command=119
            hostCheck=c['motion_digital-out-03']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("120") > -1:
            command=120
            hostCheck=c['motion_distance-to-go']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("123") > -1:
            command=123
            hostCheck=c['motion_in-position']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("124") > -1:
            command=124
            hostCheck=c['motion_motion-enabled']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("125") > -1:
            command=125
            hostCheck=c['motion_on-soft-limit']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("127") > -1:
            command=127
            hostCheck=c['motion_program-line']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("128") > -1:
            command=128
            hostCheck=c['motion_requested-vel']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("130") > -1:
            command=130
            hostCheck=c['motion_spindle-brake']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("131") > -1:
            command=131
            hostCheck=c['motion_spindle-forward']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("132") > -1:
            command=132
            hostCheck=c['motion_spindle-index-enable']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("133") > -1:
            command=133
            hostCheck=c['motion_spindle-on']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("134") > -1:
            command=134
            hostCheck=c['motion_spindle-reverse']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("137") > -1:
            command=137
            hostCheck=c['motion_spindle-speed-out']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("138") > -1:
            command=138
            hostCheck=c['motion_spindle-speed-out-rps']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("139") > -1:
            command=139
            hostCheck=c['motion_teleop-mode']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("140") > -1:
            command=140
            hostCheck=c['motion_tooloffset_x']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("141") > -1:
            command=141
            hostCheck=c['motion_tooloffset_y']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("142") > -1:
            command=142
            hostCheck=c['motion_tooloffset_z']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("143") > -1:
            command=143
            hostCheck=c['motion_tooloffset_a']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("144") > -1:
            command=144
            hostCheck=c['motion_tooloffset_b']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("145") > -1:
            command=145
            hostCheck=c['motion_tooloffset_c']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("146") > -1:
            command=146
            hostCheck=c['motion_tooloffset_u']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("147") > -1:
            command=147
            hostCheck=c['motion_tooloffset_v']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("148") > -1:
            command=148
            hostCheck=c['motion_tooloffset_w']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("149") > -1:
            command=149
            hostCheck=c['motion-command-handler_time']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("150") > -1:
            command=150
            hostCheck=c['motion-command-handler_tmax']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("151") > -1:
            command=151
            hostCheck=c['motion-controller_time']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("152") > -1:
            command=152
            hostCheck=c['motion-controller_tmax']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("153") > -1:
            command=153
            hostCheck=c['motion_debug-bit-0']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("154") > -1:
            command=154
            hostCheck=c['motion_debug-bit-1']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("155") > -1:
            command=155
            hostCheck=c['motion_debug-float-0']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("156") > -1:
            command=156
            hostCheck=c['motion_debug-float-1']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("157") > -1:
            command=157
            hostCheck=c['motion_debug-float-2']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("158") > -1:
            command=158
            hostCheck=c['motion_debug-float-3']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("159") > -1:
            command=159
            hostCheck=c['motion_debug-s32-0']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("160") > -1:
            command=160
            hostCheck=c['motion_debug-s32-1']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("161") > -1:
            command=161
            hostCheck=c['motion_servo_last-period']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("162") > -1:
            command=162
            hostCheck=c['motion_servo_last-period-ns']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("163") > -1:
            command=163
            hostCheck=c['motion_servo_overruns']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        for thisAxis in range(0,9):
            command=200
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_active"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=201
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_amp-enable-out"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=203
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_backlash-corr"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=204
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_backlash-filt"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=205
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_backlash-vel"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=206
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_coarse-pos-cmd"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=207
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_error"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=208
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_f-error"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=209
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_f-error-lim"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=210
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_f-errored"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=211
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_faulted"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=212
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_free-pos-cmd"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=213
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_free-tp-enable"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=214
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_free-vel-lim"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=216
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_homed"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=217
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_homing"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=218
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_in-position"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=219
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_index-enable"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=224
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_joint-pos-cmd"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=225
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_joint-pos-fb"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=226
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_joint-vel-cmd"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=227
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_kb-jog-active"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=228
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_motor-pos-cmd"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=230
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_neg-hard-limit"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=232
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_pos-hard-limit"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

            command=234
            if codesAccepted.find(str(command)) > -1 and axisesRequested.find(str(thisAxis)) > -1:
                preHostCheck="axis_" + str(thisAxis) + "_wheel-jog-active"
                hostCheck=c[preHostCheck]
                if hostCheck != hostCheckOld[command][thisAxis]:
                    hostCheckOld[command][thisAxis]=hostCheck
                    checkSum=command+thisAxis+hostCheck
                    comString=str(command) + " " + str(thisAxis) + " " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                    masterTx.put(comString)

        if codesAccepted.find("300") > -1:
            command=300
            hostCheck=c['iocontrol_coolant-flood']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("301") > -1:
            command=301
            hostCheck=c['iocontrol_coolant-mist']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("303") > -1:
            command=303
            hostCheck=c['iocontrol_lube']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("305") > -1:
            command=305
            hostCheck=c['iocontrol_tool-change']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("307") > -1:
            command=307
            hostCheck=c['iocontrol_tool-number']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("308") > -1:
            command=308
            hostCheck=c['iocontrol_tool-prep-number']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("309") > -1:
            command=309
            hostCheck=c['iocontrol_tool-prepare']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("311") > -1:
            command=311
            hostCheck=c['iocontrol_user-enable-out']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)

        if codesAccepted.find("312") > -1:
            command=312
            hostCheck=c['iocontrol_user-request-enable']
            if hostCheck != hostCheckOld[command]:
                hostCheckOld[command]=hostCheck
                checkSum=hostCheck+command
                comString=str(command) + " 0 " + str(hostCheck) + " " + str(int(checkSum)) + ";"
                masterTx.put(comString)
                
        if masterTx.qsize():
            sleep((masterTx.qsize() * 0.075))
            # Grow a delay as needed to avoid que overflow. Dwell for 1 millsec when idling.
        else:
            sleep(0.001)

    
def comService():
    print "comService: started."
    while threadsRun:
        sleep(0.001)
        try:
            readyForNextCommand = 0
            for e in range(0,len(commandList)):
                if commandList[e] == "":
                    readyForNextCommand = readyForNextCommand + 1
            while not masterTx.empty():
                msg=masterTx.get()
                
                msgj=msg.rstrip(';')
                dataSet=[]
                dataSet=msgj.split()
                for i in range(0,len(dataSet)):
                    if i == 2:
                        if dataSet[i].find('.') > -1: #If float, make it a proper lenth one.
                            dataSet[i]=Decimal(dataSet[i])
                            dataSet[i]=int(dataSet[i] * floatMultiplier) # multiply up into a whole number and convert it to a non-float
                            
                        else:
                            dataSet[i]=int(dataSet[i]) # Just another integer.
                buildSum=int(dataSet[0]) + int(dataSet[1]) + int(dataSet[2])
                buildString=str(dataSet[0]) + " " + str(dataSet[1]) + " " + str(dataSet[2]) + " " + str(buildSum) + ";"
                nextCmd=buildString

                for n in range(0,len(commandList)):
                    clientCmd=clientQueueArray[n]
                    clientCmd.put(nextCmd)
                    pMsg=str(clientQueueArray[n])
                    pMsg=pMsg.replace('<Queue.Queue instance at ', '')
                    pMsg=pMsg.rstrip('>')
#                    print "comService: issued %s to clientQueue: %s (%s)" % ( nextCmd, n, pMsg )
                masterTx.task_done()
#        sleep(0.001)
        except Exception:
            pass
    else:
        print "Closing: %s" % port

def makePins(codesAccepted, axisesRequested):
#Here we make our HAL pins (without duplicates.)
    if codesAccepted.find("100") > -1:
        if simulation == True:
            print "creating: motion_adaptive-feed"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_adaptive-feed",hal.HAL_FLOAT,hal.HAL_OUT)
    if codesAccepted.find("101") > -1:
        if simulation == True:
            print "creating: motion_analog-in-00"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_analog-in-00",hal.HAL_FLOAT,hal.HAL_OUT)
    if codesAccepted.find("102") > -1:
        if simulation == True:
            print "creating: motion_analog-in-01"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_analog-in-01",hal.HAL_FLOAT,hal.HAL_OUT)
    if codesAccepted.find("103") > -1:
        if simulation == True:
            print "creating: motion_analog-in-02"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_analog-in-02",hal.HAL_FLOAT,hal.HAL_OUT)
    if codesAccepted.find("104") > -1:
        if simulation == True:
            print "creating: motion_analog-in-03"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_analog-in-03",hal.HAL_FLOAT,hal.HAL_OUT)
    if codesAccepted.find("105") > -1:
        if simulation == True:
            print "creating: motion_analog-out-00"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_analog-out-00",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("106") > -1:
        if simulation == True:
            print "creating: motion_analog-out-01"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_analog-out-01",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("107") > -1:
        if simulation == True:
            print "creating: motion_analog-out-02"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_analog-out-02",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("108") > -1:
        if simulation == True:
            print "creating: motion_analog-out-03"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_analog-out-03",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("109") > -1:
        if simulation == True:
            print "creating: motion_coord-error"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_coord-error",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("110") > -1:
        if simulation == True:
            print "creating: motion_coord-mode"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_coord-mode",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("111") > -1:
        if simulation == True:
            print "creating: motion_current-vel"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_current-vel",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("112") > -1:
        if simulation == True:
            print "creating: motion_digital-in-00"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_digital-in-00",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("113") > -1:
        if simulation == True:
            print "creating: motion_digital-in-01"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_digital-in-01",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("114") > -1:
        if simulation == True:
            print "creating: motion_digital-in-02"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_digital-in-02",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("115") > -1:
        if simulation == True:
            print "creating: motion_digital-in-03"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_digital-in-03",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("116") > -1:
        if simulation == True:
            print "creating: motion_digital-out-00"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_digital-out-00",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("117") > -1:
        if simulation == True:
            print "creating: motion_digital-out-01"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_digital-out-01",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("118") > -1:
        if simulation == True:
            print "creating: motion_digital-out-02"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_digital-out-02",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("119") > -1:
        if simulation == True:
            print "creating: motion_digital-out-03"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_digital-out-03",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("120") > -1:
        if simulation == True:
            print "creating: motion_distance-to-go"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_distance-to-go",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("121") > -1:
        if simulation == True:
            print "creating: motion_enable"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_enable",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("122") > -1:
        if simulation == True:
            print "creating: motion_feed-hold"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_feed-hold",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("123") > -1:
        if simulation == True:
            print "creating: motion_in-position"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_in-position",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("124") > -1:
        if simulation == True:
            print "creating: motion_motion-enabled"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_motion-enabled",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("125") > -1:
        if simulation == True:
            print "creating: motion_on-soft-limit"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_on-soft-limit",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("126") > -1:
        if simulation == True:
            print "creating: motion_probe-input"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_probe-input",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("127") > -1:
        if simulation == True:
            print "creating: motion_program-line"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_program-line",hal.HAL_S32,hal.HAL_IN)
    if codesAccepted.find("128") > -1:
        if simulation == True:
            print "creating: motion_requested-vel"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_requested-vel",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("129") > -1:
        if simulation == True:
            print "creating: motion_spindle-at-speed"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-at-speed",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("130") > -1:
        if simulation == True:
            print "creating: motion_spindle-brake"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-brake",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("131") > -1:
        if simulation == True:
            print "creating: motion_spindle-forward"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-forward",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("132") > -1:
        if simulation == True:
            print "creating: motion_spindle-index-enable"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-index-enable",hal.HAL_BIT,hal.HAL_IO)
    if codesAccepted.find("133") > -1:
        if simulation == True:
            print "creating: motion_spindle-on"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-on",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("134") > -1:
        if simulation == True:
            print "creating: motion_spindle-reverse"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-reverse",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("135") > -1:
        if simulation == True:
            print "creating: motion_spindle-revs"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-revs",hal.HAL_FLOAT,hal.HAL_OUT)
    if codesAccepted.find("136") > -1:
        if simulation == True:
            print "creating: motion_spindle-speed-in"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-speed-in",hal.HAL_FLOAT,hal.HAL_OUT)
    if codesAccepted.find("137") > -1:
        if simulation == True:
            print "creating: motion_spindle-speed-out"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-speed-out",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("138") > -1:
        if simulation == True:
            print "creating: motion_spindle-speed-out-rps"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_spindle-speed-out-rps",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("139") > -1:
        if simulation == True:
            print "creating: motion_teleop-mode"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_teleop-mode",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("140") > -1:
        if simulation == True:
            print "creating: motion_tooloffset_x"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_tooloffset_x",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("141") > -1:
        if simulation == True:
            print "creating: motion_tooloffset_y"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_tooloffset_y",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("142") > -1:
        if simulation == True:
            print "creating: motion_tooloffset_z"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_tooloffset_z",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("143") > -1:
        if simulation == True:
            print "creating: motion_tooloffset_a"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_tooloffset_a",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("144") > -1:
        if simulation == True:
            print "creating: motion_tooloffset_b"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_tooloffset_b",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("145") > -1:
        if simulation == True:
            print "creating: motion_tooloffset_c"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_tooloffset_c",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("146") > -1:
        if simulation == True:
            print "creating: motion_tooloffset_u"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_tooloffset_u",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("147") > -1:
        if simulation == True:
            print "creating: motion_tooloffset_v"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_tooloffset_v",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("148") > -1:
        if simulation == True:
            print "creating: motion_tooloffset_w"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_tooloffset_w",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("149") > -1:
        if simulation == True:
            print "creating: motion-command-handler_time"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion-command-handler_time",hal.HAL_S32,hal.HAL_IN)
    if codesAccepted.find("150") > -1:
        if simulation == True:
            print "creating: motion-command-handler_tmax"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion-command-handler_tmax",hal.HAL_S32,hal.HAL_IO)
    if codesAccepted.find("151") > -1:
        if simulation == True:
            print "creating: motion-controller_time"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion-controller_time",hal.HAL_S32,hal.HAL_IN)
    if codesAccepted.find("152") > -1:
        if simulation == True:
            print "creating: motion-controller_tmax"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion-controller_tmax",hal.HAL_S32,hal.HAL_IO)
    if codesAccepted.find("153") > -1:
        if simulation == True:
            print "creating: motion_debug-bit-0"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_debug-bit-0",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("154") > -1:
        if simulation == True:
            print "creating: motion_debug-bit-1"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_debug-bit-1",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("155") > -1:
        if simulation == True:
            print "creating: motion_debug-float-0"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_debug-float-0",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("156") > -1:
        if simulation == True:
            print "creating: motion_debug-float-1"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_debug-float-1",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("157") > -1:
        if simulation == True:
            print "creating: motion_debug-float-2"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_debug-float-2",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("158") > -1:
        if simulation == True:
            print "creating: motion_debug-float-3"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_debug-float-3",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("159") > -1:
        if simulation == True:
            print "creating: motion_debug-s32-0"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_debug-s32-0",hal.HAL_S32,hal.HAL_IN)
    if codesAccepted.find("160") > -1:
        if simulation == True:
            print "creating: motion_debug-s32-1"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_debug-s32-1",hal.HAL_S32,hal.HAL_IN)
    if codesAccepted.find("161") > -1:
        if simulation == True:
            print "creating: motion_servo_last-period"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_servo_last-period",hal.HAL_U32,hal.HAL_IN)
    if codesAccepted.find("162") > -1:
        if simulation == True:
            print "creating: motion_servo_last-period-ns"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_servo_last-period-ns",hal.HAL_FLOAT,hal.HAL_IN)
    if codesAccepted.find("163") > -1:
        if simulation == True:
            print "creating: motion_servo_overruns"
        else:
            print "makePins: creating: %r" % pinLabel
            c.newpin("motion_servo_overruns",hal.HAL_U32,hal.HAL_IO)

    # The following pins need to be iterated for each axis used.
    for i in range(0, 10):
        if codesAccepted.find("200") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_active"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("201") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_amp-enable-out"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("202") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_amp-fault-in"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_OUT)

        if codesAccepted.find("203") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_backlash-corr"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("204") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_backlash-filt"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("205") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_backlash-vel"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("206") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_coarse-pos-cmd"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("207") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_error"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("208") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_f-error"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("209") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_f-error-lim"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("210") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_f-errored"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("211") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_faulted"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("212") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_free-pos-cmd"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("213") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_free-tp-enable"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("214") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_free-vel-lim"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("215") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_home-sw-in"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_OUT)

        if codesAccepted.find("216") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_homed"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("217") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_homing"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("218") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_in-position"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("219") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_index-enable"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IO)

        if codesAccepted.find("220") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_jog-counts"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_S32,hal.HAL_OUT)

        if codesAccepted.find("221") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_jog-enable"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_OUT)

        if codesAccepted.find("222") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_jog-scale"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_OUT)

        if codesAccepted.find("223") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_jog-vel-mode"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_OUT)

        if codesAccepted.find("224") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_joint-pos-cmd"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("225") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_joint-pos-fb"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("226") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_joint-vel-cmd"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("227") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_kb-jog-active"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("228") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_motor-pos-cmd"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_IN)

        if codesAccepted.find("229") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_motor-pos-fb"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_FLOAT,hal.HAL_OUT)

        if codesAccepted.find("230") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_neg-hard-limit"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("231") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_pos-lim-sw-in"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_OUT)

        if codesAccepted.find("232") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_pos-hard-limit"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

        if codesAccepted.find("233") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_neg-lim-sw-in"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_OUT)

        if codesAccepted.find("234") > -1 and axisesRequested.find(str(i)) > -1:
            pinLabel = "axis_" + str(i) + "_wheel-jog-active"
            if simulation == True:
                print "creating: %s" % pinLabel
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin(pinLabel,hal.HAL_BIT,hal.HAL_IN)

    # Finally we follow up with the iocontrol pins.
    if codesAccepted.find("300") > -1:
            if simulation == True:
                print "creating: iocontrol_coolant-flood"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_coolant-flood",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("301") > -1:
            if simulation == True:
                print "creating: iocontrol_coolant-mist"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_coolant-mist",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("302") > -1:
            if simulation == True:
                print "creating: iocontrol_emc-enable-in"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_emc-enable-in",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("303") > -1:
            if simulation == True:
                print "creating: iocontrol_lube"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_lube",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("304") > -1:
            if simulation == True:
                print "creating: iocontrol_lube_level"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_lube_level",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("305") > -1:
            if simulation == True:
                print "creating: iocontrol_tool-change"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_tool-change",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("306") > -1:
            if simulation == True:
                print "creating: iocontrol_tool-changed"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_tool-changed",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("307") > -1:
            if simulation == True:
                print "creating: iocontrol_tool-number"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_tool-number",hal.HAL_S32,hal.HAL_IN)
    if codesAccepted.find("308") > -1:
            if simulation == True:
                print "creating: iocontrol_tool-prep-number"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_tool-prep-number",hal.HAL_S32,hal.HAL_IN)
    if codesAccepted.find("309") > -1:
            if simulation == True:
                print "creating: iocontrol_tool-prepare"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_tool-prepare",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("310") > -1:
            if simulation == True:
                print "creating: iocontrol_tool-prepared"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_tool-prepared",hal.HAL_BIT,hal.HAL_OUT)
    if codesAccepted.find("311") > -1:
            if simulation == True:
                print "creating: iocontrol_user-enable-out"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_user-enable-out",hal.HAL_BIT,hal.HAL_IN)
    if codesAccepted.find("312") > -1:
            if simulation == True:
                print "creating: iocontrol_user-request-enable"
            else:
                print "makePins: creating: %r" % pinLabel
                c.newpin("iocontrol_user-request-enable",hal.HAL_BIT,hal.HAL_IN)



try:
    for i in range(0, maxClients - 1):
        port = "/dev/ttyACM" + str(i)
        start_new_thread(comThread, (port, ) )
        sleep(0.1)
    sleep(3)

except Exception:
    threadsRun=False
    ser.close()
    sleep(3)
    raise SystemExit
finally:
    sleep(3)
    if len(listOfUnits) == len(listOfPorts) and len(listOfUnits) > 0:
        sleep(3)
        makePins(str(listOfCommands), str(listOfAxis))
        start_new_thread( comService, (), )
        masterTx.put("996 0 0 996;")
        if simulation:
            masterTx.put("996 0 0 996;")
            masterRx.put("800 1 2 803;")
            masterRx.put("801 2 1 804;")
            masterRx.put("802 1 2 805;")
        #    masterRx.put("894 0 0 994;") #deliberatly bad command.
        # Now that the lists of units, commands, axises and ports have been populated. Move on.
        print "commandList=%r" % commandList
        print "linkData=%r" % linkData
        print "listOfPorts=%r" % listOfPorts
        print "listOfCommands=%r" % listOfCommands
        print "listOfAxis=%r" % listOfAxis
        print "listOfUnits=%r" % listOfUnits
        print "linkMsg=%r" % linkMsg
        sleep(1)
        commandHandler(str(listOfCommands), str(listOfAxis))
        msgBox("Units found: %s\nCommands used: %s\nAxises in use: %s\nPorts in use: %s" % (listOfUnits, listOfCommands, listOfAxis, len(listOfPorts)))
        
    else:
        msgBox("\aConnection Error:\nPlease reload the host application.")
        print("\aWait, something is not right here.")
        threadsRun=False
        raise SystemExit
