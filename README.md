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
	root@Arduino:# opkg update
	root@Arduino:# opkg install openssh-sftp-server

the rrd software 
	root@Arduino:# opkg install rrdcgi1 rrdtool1

Arduino Sketch
--------------
Because of having multiple sensors, i decided to define the sensors one by one.

	