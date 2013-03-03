HAL2Arduino install mini-guide. (I'm lazy.)

Start by coping the "HAL2Arduino_0_4" folder to your collection of Arduino projects.

Next, load the HAL2Arduino.ino file in the Arduino IDE.
(The other .ino files will automatically open up as separate 'tabs' within the HAL2Arduino sketch.)

Read through each tab and edit them to fit your machine. The tabs with names that start with 'mod' are optional add ons.
You can either enable/disable them as you choose, or even delete the ones you really don't need/want.

Onwards:
I've included the config files from my own machine so you have a fair chance to see how I got it working, also I put a drawing.ngc of a female linerae model as an inkpen 'engraving' test file. (Since the LinuxCNC demo file gets boring after awhile.)

Next, copying files...
(Back up your files first.)
The files go into the following directories/folders:
HAL2Arduino.py	 - your 'my-mill' directory/folder
custom.hal	 - your 'my-mill' directory/folder
my-mill.hal	 - your 'my-mill' directory/folder
my-mill.ini	 - your 'my-mill' directory/folder

drawing.ngc	 - anywhere you like. I use the Documents folder myself.

"HAL2Arduino_0_4" folder should already be copied to your sketch stash.

Notes:
The config of this machine is for a size of 8"x11" sheet of standard printer paper.
So you may need to play with the offsets to get it to run.

Good luck with it.