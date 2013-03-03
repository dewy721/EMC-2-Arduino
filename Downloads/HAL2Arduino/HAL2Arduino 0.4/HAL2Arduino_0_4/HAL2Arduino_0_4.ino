/*

This sketch provides an breakout interface between LinuxCNC and the Arduino platforms.
(By background use of HAL and a python script.)

It just moves data between LinuxCNC and the Arduino.

What gets done with the data is up to you.

The serial link works as follows:
  Only send data when change is detected, no repeats, no spammming, host keeps a small log of recent commands.
  LinuxCNC and the Arduino send each other single line of 4 or 5 numerical 'word'
  statements back and forth such as "tansaction# codeNumber axis(ifUsed) dataValue Checksum".
  The checksum is simply the sum of the preceeding arguments of the sentance.
  Sentances are deliniated with a semicolon.
  
  Some examples of sentances would be like the following:
  
    107 0 1 109;   (motion.digital-out pin"0" HIGH) (if 107+0+1=108 checksum=good)
    118 0 0 118;   (Disable spindle brake.) (if 118+0+0=118 checksum=good)
  
  The legend of numerical "LinuxCNC 2 Arduino" codes are translated from 
  http://linuxcnc.org/docs/EMC2_Integrator_Manual.pdf (Section "6.1.2 Pins")
  
  Please reffer to the above link for detailed information.
    If possible, avoid calculating floating point numbers directly, 
    they're very expensive in Arduino processor cycles.
    Instead, try to multiply them by factors of ten into whole numbers
    before working on them directly.
    
  [Note: in=toward LinuxCNC, out=toward the Arduino]
  100  motion.adaptive-feed - (float, in)
  101  motion.analog-in-00 - (float, in)
  102  motion.analog-in-01 - (float, in)
  103  motion.analog-in-02 - (float, in)
  104  motion.analog-in-03 - (float, in)
  105  motion.analog-out-00 - (float, out)
  106  motion.analog-out-01 - (float, out)
  107  motion.analog-out-02 - (float, out)
  108  motion.analog-out-03 - (float, out)
  109  motion.coord-error - (bit, out)
  110  motion.coord-mode - (bit, out)
  111  motion.current-vel - (float, out)
  112  motion.digital-in-00 - (bit, in)
  113  motion.digital-in-01 - (bit, in)
  114  motion.digital-in-02 - (bit, in)
  115  motion.digital-in-03 - (bit, in)
  116  motion.digital-out-00 - (bit, out)
  117  motion.digital-out-01 - (bit, out)
  118  motion.digital-out-02 - (bit, out)
  119  motion.digital-out-03 - (bit, out)
  120  motion.distance-to-go - (float,out)
  121  motion.enable - (bit, in)
  122  motion.feed-hold - (bit, in)
  123  motion.in-position - (bit, out)
  124  motion.motion-enabled - (bit, out)
  125  motion.on-soft-limit - (bit, out)
  126  motion.probe-input - (bit, in)
  127  motion.program-line - (s32, out)
  128  motion.requested-vel - (float, out)
  129  motion.spindle-at-speed - (bit, in)
  130  motion.spindle-brake - (bit, out)
  131  motion.spindle-forward - (bit, out)
  132  motion.spindle-index-enable - (bit, I/O)
  133  motion.spindle-on - (bit, out)
  134  motion.spindle-reverse - (bit, out)
  135  motion.spindle-revs - (float, in)
  136  motion.spindle-speed-in - (float, in)
  137  motion.spindle-speed-out - (float, out)
  138  motion.spindle-speed-out-rps - (float, out)
  139  motion.teleop-mode - (bit, out)
  140  motion.tooloffset.x - (float, out)
  141  motion.tooloffset.y - (float, out)
  142  motion.tooloffset.z - (float, out)
  143  motion.tooloffset.a - (float, out)
  144  motion.tooloffset.b - (float, out)
  145  motion.tooloffset.c - (float, out)
  146  motion.tooloffset.u - (float, out)
  147  motion.tooloffset.v - (float, out)
  148  motion.tooloffset.w - (float, out)
  149  motion-command-handler.time - (s32, RO)
  150  motion-command-handler.tmax - (s32, RW)
  151  motion-controller.time - (s32, RO)
  152  motion-controller.tmax - (s32, RW)
  153  motion.debug-bit-0 - (bit, RO)
  154  motion.debug-bit-1 - (bit, RO)
  155  motion.debug-float-0 - (float, RO)
  156  motion.debug-float-1 - (float, RO)
  157  motion.debug-float-2 - (float, RO)
  158  motion.debug-float-3 - (float, RO)
  159  motion.debug-s32-0 - (s32, RO)
  160  motion.debug-s32-1 - (s32, RO)
  161  motion.servo.last-period - (u32, RO)
  162  motion.servo.last-period-ns - (float, RO)
  163  motion.servo.overruns - (u32, RW)
  
  200  axis.N.active - (bit, out)
  201  axis.N.amp-enable-out - (bit, out)
  202  axis.N.amp-fault-in - (bit, in)
  203  axis.N.backlash-corr - (float, out)
  204  axis.N.backlash-filt - (float, out)
  205  axis.N.backlash-vel - (float, out)
  206  axis.N.coarse-pos-cmd - (float, out)
  207  axis.N.error - (bit, out)
  208  axis.N.f-error - (float, out)
  209  axis.N.f-error-lim - (float, out)
  210  axis.N.f-errored - (bit, out)
  211  axis.N.faulted - (bit, out)
  212  axis.N.free-pos-cmd - (float, out)
  213  axis.N.free-tp-enable - (bit, out)
  214  axis.N.free-vel-lim - (float, out)
  215  axis.N.home-sw-in - (bit, in)
  216  axis.N.homed - (bit, out)
  217  axis.N.homing - (bit, out)
  218  axis.N.in-position - (bit, out)
  219  axis.N.index-enable - (bit, I/O)
  220  axis.N.jog-counts - (s32, in)
  221  axis.N.jog-enable - (bit, in)
  222  axis.N.jog-scale - (float, in)
  223  axis.N.jog-vel-mode - (bit, in)
  224  axis.N.joint-pos-cmd - (float, out)
  225  axis.N.joint-pos-fb - (float, out)
  226  axis.N.joint-vel-cmd - (float, out)
  227  axis.N.kb-jog-active - (bit, out)
  228  axis.N.motor-pos-cmd - (float, out)
  229  axis.N.motor-pos-fb - (float, in)
  230  axis.N.neg-hard-limit - (bit, out)
  231  axis.N.pos-lim-sw-in - (bit, in)
  232  axis.N.pos-hard-limit - (bit, out)
  233  axis.N.neg-lim-sw-in - (bit, in)
  234  axis.N.wheel-jog-active - (bit, out)

  300  iocontrol.0.coolant-flood - (bit, out)
  301  iocontrol.0.coolant-mist - (bit, out)
  302  iocontrol.0.emc-enable-in - (bit, in)
  303  iocontrol.0.lube - (bit, out)
  304  iocontrol.0.lube_level - (bit, in)
  305  iocontrol.0.tool-change - (bit, out)
  306  iocontrol.0.tool-changed - (bit, in)
  307  iocontrol.0.tool-number - (s32, out)
  308  iocontrol.0.tool-prep-number - (s32, out)
  309  iocontrol.0.tool-prepare - (bit, out)
  310  iocontrol.0.tool-prepared - (bit, in)
  311  iocontrol.0.user-enable-out - (bit, out)
  312  iocontrol.0.user-request-enable - (bit, out)
  
  800  Send global broadcast to all other (Arduino) clients. ie: 800 numVal numVal sumOfFirstThreeValues;

  801~899  Send send a single numVal to a specific client. ie. 801 targetUnitId numVal sumOfFirstThreeValues;
           This gives 98 possible channels (per targetId) to send numerical words in an ordered and verified manner.
           Happy hacking. :)

  990  Send firmware title.
  991  Send version.
  992  Send unit Id.
  993  Send commands requested list.
  994  Send axises requested list.
  995  Toggle debug output. (Hint: 995 0 0 995;)
  996  Received confirmation to begin normal operation.
*/

// What baudrate should we use.
#define BAUD 115200

// When asked, identify to host.
#define firmwareTitle "HAL-2-Arduino"

// When asked, send version info.
#define Version "0.4"

// When asked, send unit number. (In case you need multiple Arduinos.)
// Default max is 10, if you need more
#define unitNumber 0

// When asked, request access to these commands.
#define commandsRequested "215 224 226 231 233"

// When asked, request access to these axises (for commands that use them).
#define axisRequested "0 1 2"

// A command transmitted from the host needs time to be completed.
#define statementTimeout 5 // milliSeconds we should listen (per word) before acting.


// Setup some variables needed by HAL2Arduino and/or it modFiles.
boolean canMove=1;
long transId;
long command;
long dataOne;
long dataTwo;
long checksum;
long xTargetPos=-1;
long yTargetPos=-1;
long zTargetPos=-1;
long aTargetPos=-1;
long bTargetPos=-1;
long cTargetPos=-1;
long uTargetPos=-1;
long vTargetPos=-1;
long wTargetPos=-1;
boolean clientOps=false; // Host will set to true when ready.
