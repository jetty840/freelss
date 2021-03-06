
#FreeLSS

FreeLSS is a laser scanning program for the Raspberry Pi. It allows a Raspberry Pi to function as the core to a complete turn table laser scanning system.



###THIS FORK

This fork adds Neopixel ring support to freelss.  It includes the circuit/board design for a small mainly SMD shield to handle Neopixels / lasers and steppers.

The advantage of the Neopixel ring is adjustable color and brightness for illumination of the object and reporting of the ip address at startup (useful in DHCP configs).

If you are not usng Neopixels, you can still use the board and omit a few components if you require a small form factor, or use a different board.

Additionally, custom PWM (White) lighting can be setup such that the model and lasers can have different ambient lighting.



###COMPILE

These instructions assume you are running the latest version of Raspbian.  Other distros will likely require changes.

First, update the firmware to the latest version and reboot.
```
$ sudo apt-get update
$ sudo apt-get upgrade
```

Install the dependencies that are managed by the package manager.
```
$ sudo apt-get install libpng-dev libjpeg-dev cmake vlc git-core gcc build-essential unzip sqlite3 libsqlite3-dev libmicrohttpd-dev libcurl4-openssl-dev libiw-dev libssl-dev
```

Download and install wiringPi
```
$ git clone git://git.drogon.net/wiringPi
$ cd wiringPi
$ ./build
$ cd ..
```

Disable the soundcard on the Pi (interferes with Neopixels):
Reference: http://www.instructables.com/id/Disable-the-Built-in-Sound-Card-of-Raspberry-Pi/
```
$ cd /etc/modprobe.d
$ sudo vi alsa-blacklist.conf
Add the following line:
  blacklist snd_bcm2835
Save
$ sudo reboot

After reboot:
$ aplay -l
  Should say "No soundcards found"

```

Download and install rpi_ws821x
Reference: https://learn.adafruit.com/neopixels-on-raspberry-pi/software
```
$ sudo apt-get install build-essential python-dev git scone swig
$ git clone https://github.com/jgarff/rpi_ws281x.git
$ cd rpi_ws281x
$ scons
$ sudo ./test
(Neopixels will go through color cycling, press CTRL-C when done)
$ cd ..
```

Download and build FreeLSS
```
$ git clone https://github.com/jetty840/freelss
$ cd freelss/src
$ make
```
###Running FreeLSS
FreeLSS must be ran as root (or another user with access to the hardware pins).  The interface for FreeLSS is web based and by default runs on port 80.  When running, access it by navigating to http://localhost/ from the Raspberry Pi itself. Or access it from another machine on the network by the Raspberry Pi's IP or hostname.  For Example: http://raspberrypi/

The following command starts FreeLSS.
```
$ sudo ./freelss
```

The following command automatically starts FreeLSS everytime the Raspberry Pi is powered on.
```
$ make startup
```

###Pin Configuraton
For FreeLSS running with the provided schematic/PCB in this fork, configuration for this board is as follows:
```
Right Laser Pin: 4
Left Laser Pin: 0
Laser On Value: 1
Motor Enable Pin: 5
Motor Step Pin: 2
Motor Step Delay: 5000
Motor Direction Pin: 3
Motor Response Delay: 2
Enable Lighting: Checked
Light Type: WS281x
Lighting Pin: 18
```

###Building the board
Populate R4/R6 OR D1/D2, depending on your laser diodes.  If it's 5V and can control it's own current, then populate R4/R6 with a zero ohm resistor.  Otherwise to get the correct voltage for the laser diode, D1/D2 can be used as a Diode drop, and the trace from the middle of the diode pair to +5V can be cut and used as 2 diode drops if required.  Alternative R4/R6 can be used to limit voltage/current less accurately.

For the PI header, decide if you want to use it as a shield (use a PI Stacking Header), or wire 0.1" pin headers if you wish to cable it out (verify pin 1 = pin 1 if you're using IDC headers and a cable).

Adafruit 12 LED Neopixel ring recommended (720ma).  Be careful of current here, the Pi needs to be able to handle it, or supply power using a different method.

2A Minimum 5V PSU required.

DISCLAIMER: By using this design and/or fork, you acknowledge that you do so entirely at your own risk and that you will not hold the author liable under any cicrumstances.
