# Wireless-Forest-Fire-Detection-System
This is an IOT Based Wireless FF Detection Based on some DIY Components

## About The Project

![Product Screen Shot](https://user-images.githubusercontent.com/76607486/127271076-1ae858de-08d4-4c3e-91e1-d1f0a243c81e.jpg)

Our Project Work On Node MCU base We have Attached All sensor on one platform making it a slave node and taking all its data on one platform making it the master node
Also all are data is pushed on Blynk Server that can be easily accessed

## Features
* It has 3 Level of False alarm detection 
* It has integrated Weather API
* All data can be easily accessed on mobile platform via Blynk
* It has Inbuilt DeepSleep Mode which can sevearly Decrease the power consumption of the unit
* It has A Gps module attached to it Which Streams the live Location In case of a Forest Fire

### Built With

This section should list any major frameworks that you built your project using. Leave any add-ons/plugins for the acknowledgements section. Here are a few examples.
* [Arduino IDE](https://www.arduino.cc/en/software/)
* [Blynk](https://blynk.io/)
* [Node MCU Help](https://nodemcu.readthedocs.io/en/release/)

## Getting Started
U will need Blynk App and wifi network connected to internet make sure the wifi details you enter are the same as the one on which blynk app is running  


### Components Used
* [Node MCU](https://robu.in/product/nodemcu-cp2102-board/)
* [MQ 135](https://robu.in/product/mq-135-air-quality-gas-detector-sensor-module-for-arduino/)
* [DHT 11](https://robu.in/product/dht-11-digital-temperature-humidity-sensor/)
* [Oled Display](https://robu.in/product/1-3-inch-i2c-iic-oled-lcd-module-4pin-with-vcc-gnd-white/)
* [Gps Module](https://robu.in/product/ublox-neo-6m-gps-module/)
* [IR Flame Sensor](https://www.pcboard.ca/flame-sensor-module)


### Connnection on Board
* Dht sensor --  D4 on Node MCU and for HUmidity pin (v-5) temperature pin (v-6) on blynk app (Use Gauge)
* IR flame sensor -- D5 on Node MCU
* gps module -- rx-D1 tx-D2 on Node MCU  and for  map(v0)   lat(v3)  long(v4) on blynk app (use map and Value display)
* MQ  sensor -- A0 on Node MCU and  (v1) on Blynk APP (Use Value display)
* display module -- D1 D2

### Demo Video
https://user-images.githubusercontent.com/76607486/130248834-0e4b0627-dedf-46f6-83a3-f2ffaa9ac1a0.mp4

## Notes
* Get you API via open weather api
* currently we are unable to procure the process of running Oled display and gps module simultaneously hence we request you to comment out the one of either and use


## Contact

Harsh Pandey - harshpandey00701@gmail.com
Subhanshu Arya - subhanshu20101@iiitnr.edu.in
Project Link: [https://github.com/harshp77/Wireless-Forest-Fire-Detection-System](https://github.com/harshp77/Wireless-Forest-Fire-Detection-System)
