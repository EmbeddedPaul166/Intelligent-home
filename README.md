# Intelligent home

### Brief description
This is a simple implementation of an intelligent home using STM32F103RB Nucleo board. It provides temperature regulation, intelligent lights and alarm simulation. Software is primarily using interrupts and direct memory access with 12 bit ADC to implement project functionality. Main goals of this project were to tinker with STM32F103RB, practice embedded C, learn remote debugging using openocd with gdb and write a custom Makefile.

<img src="https://github.com/EmbeddedPaul166/Intelligent-home/blob/master/stm32.jpg" height="500" width="500">

### Electronic components
- STM32F103RB Nucleo
- Grove shield
- Some grove connectors
- Grove temperature sensor
- Grove light sensor
- Grove sound sensor
- Grove button
- Grove potentiometer
- 3x LED

### Grove shield pinout
- A0 temperature sensor
- A1 light sensor
- A2 sound sensor
- A3 potentiometer
- D5 button
- D6 red LED
- D8 blue LED
- D4 green LED

### How to run
You will need arm-none-eabi-gcc compiler to run this project and you will need to set PATH and ARCH environment variables correctly. To build run:
```
make 
```
To flash you will need st-link utility for flashing. Script flash.sh was made for this purpose.
```
./flash.sh
```
You can also flash using openocd with gdb. It's also launched with a script, debug.sh. Simply run:
```
./debug.sh
```
And then from gdb menu:
```
load
```

### Temperature regulation
When temperature is above treshold value in Celsius + 1 red LED is off which symbolizes cooling. When it's below treshold - 1 red LED is on which symbolizes heating. When temperature value is between (treshold -1) and (treshold + 1) LED blinks which symbolizes no regulation.

### Intelligent lights
When light intensity is below treshold value green LED is on, when it's above LED is off.

### Intelligent alarm
When sound intensity is above treshold value blue LED is on, when it's below it's off.

### Treshold values
Treshold values can be set using button and potentiometer. On default first value that is set is temperature in Celsius in the range from 10 to 40 Celsius degrees. When button is pushed interrupt occurs that causes potentiometer to set light intensity treshold values. When it's pushed again it switched to sound intensity. After that temperature again and so on.

### UART communication
Device can be queried for raw sensor values using UART (115200, default frame). When inputting certain commands 12 bit ADC data in two 8 bit values, starting from less significant byte. Commands are: "t" - temperature, "l" - light, "s" - sound.

### Some additional details
Regulation occurs every 50 ms in a timer interrupt which is synchronised with DMA callback from ADC. In that callback data is accumulated until it reaches 100 measurements for each sensor and average values are calculated and passed to regulation function.
