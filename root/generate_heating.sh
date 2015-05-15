#!/bin/sh

rrddir=/www/sd/temperature_measuring/rrd/
HEATING=/www/sd/temperature_measuring/index.html

boiler_unten=$(curl http://localhost/data/get/boiler_unten 2>/dev/null | awk -F"\"" '{ print $4 }')
boiler_mitte=$(curl http://localhost/data/get/boiler_mitte 2>/dev/null | awk -F"\"" '{ print $4 }')
boiler_oben=$(curl http://localhost/data/get/boiler_oben 2>/dev/null | awk -F"\"" '{ print $4 }')


rrdtool update /www/sd/temperature_measuring/heating.rrd N:$boiler_oben:$boiler_mitte:$boiler_unten

echo -n "create boiler day: "
rrdtool graph $rrddir/Speicher_Tag.gif -s "-1d" -w 600 -h 300 --x-grid "MINUTE:30:HOUR:1:MINUTE:120:0:%H" \
	--title="Boiler 24h" --vertical-label "Celsius"  \
	'DEF:boiler_oben=/www/sd/temperature_measuring/heating.rrd:boiler_oben:AVERAGE' \
	'DEF:boiler_mitte=/www/sd/temperature_measuring/heating.rrd:boiler_mitte:AVERAGE' \
	'DEF:boiler_unten=/www/sd/temperature_measuring/heating.rrd:boiler_unten:AVERAGE' \
	'LINE2:boiler_oben#ff0000:Boiler Oben' 'LINE2:boiler_mitte#FF8C00:Boiler Mitte' 'LINE2:boiler_unten#0400ff:Boiler Unten\n'
	
