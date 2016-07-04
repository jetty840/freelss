
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
