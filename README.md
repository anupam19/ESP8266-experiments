# ESP8266-experiments

##### Experiments with ESP8266 ESP-01 variants

##### Understanding code :

###### Setting WiFi:

```C
#define SSID "YOURSSID" /* replace YOURSSID with your WiFi access point name */
#define PASS "YOURPASSWORD" /* replace YOUR PASSWORD with your WiFi access point password */
```
###### PIR sensor calibration :
```C
sensor_calibration (); 
/* It calibrates sensor for proper output. We need to power down PIRPIN before that */
```

###### WiFi Radio stat :
```C
check_WiFi_radio (); /* Checks whether WiFi module is OK or not */
reset_wifi (); /* resets WiFi module to clear any problem */
set_wifi_mode (); /* Set WiFi module as client for connecting to Access Point */
```
```C
check_wifi_stat (); /* Checks whether your WiFi module is connected to AP or not */
connect_ssid (); /* if not connected, then will connect with your connection parameter */
```

###### PIR data :
```C
pirRead ();
/* reads PIR value, either 0 or 1. If 1, then Motion Sensed, if 0, then nothing is there */
```

###### Sending data to Pushbullet :
```C
server_connect (); /* connecting to server */
post (); /* pushing notification to all devices */
server_disconnect (); /* close server connectioon */
```

Working Demonstration : http://www.codeproject.com/Articles/1002782/IoT-MotionSense-Not-e-FYI
