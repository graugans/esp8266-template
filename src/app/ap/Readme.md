# Stoft-AP Demo

This example puts the ESP8266 in Soft-AP mode so you can connect to the ESP with your mobile for example. The ESP8266 runs a DHCP server and assigns a IP for each connected device.


# Sample output on serial port
```
---------------------------------------------------
Starting Wifi SOFTAP demo application 
Espressif SDK version  : 1.3.0 
Espressif BOOT version : 0x031f 
CPU Speed              : 80 MHz
Mem information        :  
data  : 0x3ffe8000 ~ 0x3ffe8358, len: 856
rodata: 0x3ffe8358 ~ 0x3ffe8820, len: 1224
bss   : 0x3ffe8820 ~ 0x3ffef1d8, len: 27064
heap  : 0x3ffef1d8 ~ 0x3fffc000, len: 52776
---------------------------------------------------
-= Wifi SoftAP setup =-
Wifi SSID     : ESP8266
Wifi Password : 1234
ESP MAC       : 1A:FE:34:A5:XX:XX
ESP IP        : 192.168.4.1 
ESP gateway   : 192.168.4.1 
ESP netmask   : 255.255.255.0 
DHCP Server started!

EVENT_SOFTAPMODE_STACONNECTED:
stationCount = 1
station: 10:68:3f:36:XX:XX join, AID = 1
bssid : 10:68:3f:36:XX:XX, ip : 192.168.4.2
bssid : 10:68:3f:36:XX:XX, ip : 192.168.4.2
bssid : 10:68:3f:36:XX:XX, ip : 192.168.4.2
bssid : 10:68:3f:36:XX:XX, ip : 192.168.4.2

EVENT_SOFTAPMODE_STADISCONNECTED:
```