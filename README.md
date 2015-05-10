# Temperature Measuring with Arduino Yun

Here is a howto, for measuring temperature Sensors DS18B20 with Arduino Yun and generate a rrd Graph with results.

Prepare your Arduino Yun
------------------------
You will expand your arduino Yun with a SD card. Find here an example http://arduino.cc/en/Tutorial/ExpandingYunDiskSpace
Please take care to connect your Arduino to USB and check your Internet connection, which is requred for this SD Card expansion.

Install additional software to Yun
----------------------------------
Connect to your Yun via SSH Terminal and install rrd

an sftp Server to easy connect your yun with an sftp client.
# opkg update
# opkg install openssh-sftp-server

the rrd software 
# opkg install rrdcgi1 rrdtool1

