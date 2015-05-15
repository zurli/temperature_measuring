# Arduino Yun, Temperaturesensor, RRD-Graph

Here is a howto, for measuring temperature Sensors DS18B20 with Arduino Yun and generate a rrd Graph with results.

Prepare your Arduino Yun
------------------------
You will expand your arduino Yun with a SD card. Find here an example http://arduino.cc/en/Tutorial/ExpandingYunDiskSpace
Please take care to connect your Arduino to USB and check your Internet connection, which is requred for this SD Card expansion.

Install additional software to Yun
----------------------------------

an sftp Server to easy connect your yun with an sftp client.
	
	root@Arduino:# opkg update
	root@Arduino:# opkg install openssh-sftp-server

the rrd software 
	
	root@Arduino:# opkg install rrdcgi1 rrdtool1

Arduino Sketch
--------------
Because of having multiple sensors, i decided to define the sensors one by one.
Find the Arduino Sketch getTempDeviceID in examples to determine the adresses of your sensors.
After read out the sensors dervice adresses define them in sketch.

	DeviceAddress Boiler_O = { 0x28, 0x61, 0x02, 0xBB, 0x03, 0x00, 0x00, 0x88 };

Reading the temperatures with

	sensors.requestTemperatures();  // read temperatures
  	int boiler_o = sensors.getTempC(Boiler_O);

and finally put them into the bridge

	Bridge.put("boiler_oben", String(boiler_o));  // put values to bridge

Remarks: with Arduino 1.6.3 it was not possible to put more than one value to the bridge.
Therefore use at least Arduino Version 1.6.4 and above.
If you need temperature, with commas use float. Whitout commas use int.


Generate the rrd Database
-------------------------

	rdtool create /www/sd/temperature_measuring/heating.rrd --step 300 \
	DS:boiler_oben:GAUGE:900:0:U \
	DS:boiler_mitte:GAUGE:900:0:U \
	DS:boiler_unten:GAUGE:900:0:U \
	RRA:AVERAGE:0.5:1:288 \
    RRA:AVERAGE:0.5:3:672 \
    RRA:AVERAGE:0.5:12:744 \
    RRA:AVERAGE:0.5:72:1460

creates a database that
is updated every 5 minutes<br>
has heartbeat of 900, means may miss 3 measures bevore coming UNKNOWN<br>
has data sources that that can save values from 0 to unlimited<br>
saves 1 day in 5-minute resolution (288 * (300*1/60) / 60/24)<br>
saves 1 week in in 15-minute resolution (672 * (300*3/60) / 60/24)<br>
saves 1 month in 1-hour resolution (744 * (300*12/60) / 60/24)<br>
saves 1 year in 6-hour resolution (1460 * (300*72/60) / 60/24)<br>

Reading values on yun side and generate rrd database entry
----------------------------------------------------------

With a simple curl command the results of each sensor can be read

	boiler_oben=$(curl http://localhost/data/get/boiler_oben 2>/dev/null | awk -F"\"" '{ print $4 }')

after we read all these values in variables we can update the rrd database

	rrdtool update /www/sd/temperature_measuring/heating.rrd N:$boiler_oben:$boiler_mitte:$boiler_unten

and finally we will generate the graph wit rrdtool graph

	rrdtool graph $rrddir/Speicher_Tag.gif -s "-1d" -w 600 -h 300 --x-grid "MINUTE:30:HOUR:1:MINUTE:120:0:%H" \
	--title="Speicher 24h" --vertical-label "Celsius"  \
	'DEF:boiler_oben=/www/sd/temperature_measuring/heating.rrd:boiler_oben:AVERAGE' \
	'DEF:boiler_mitte=/www/sd/temperature_measuring/heating.rrd:boiler_mitte:AVERAGE' \
	'DEF:boiler_unten=/www/sd/temperature_measuring/heating.rrd:boiler_unten:AVERAGE' \
	'LINE2:boiler_oben#ff0000:Boiler Oben' 'LINE2:boiler_mitte#FF8C00:Boiler Mitte' 'LINE2:boiler_unten#0400ff:Boiler Unten\n'

to automate the enties we will generate a crontab entry as follow:

	
	*/5 * * * * /bin/sh /root/generate_heating.sh 2>> /root/error.log

if there are any errors while running the update script, the errors will be written into /root/error.log
you might need to control, if cronjob is running. You can enable crontab with

	root@Arduino:# /etc/init.d/cron start
	root@Arduino:# /etc/init.d/cron enable


Testing
-------

To read the collected data go to http://ip-your-yun/keystore_manager_example/
The generated rrd image can be accesed on http://ip-your-yun/temperature_measuring/rrd/Speicher_Tag.gif