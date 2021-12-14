This is a fork of tinyusb for portenta h7, current testing is limited but seems ok. Will put two versions of the a script(wrapper) for dfu-util in portenta-tools one will be manual (requires typing the /dev/ttyACM* the other will use lib serial to auto find the portenta, both should auto-reset the protenta using 1200baud request. Currently the bsp folder protenta works for builds that i have tried IE "make BOARD=protenta clean" "make BOARD=protenta all" Currently after loading firmware the protenta comes up then reboots 3 times then is rock solid even after unplugs...dunno why seems strange, I can even hot reset before the 3 reboots to stop them and all is good. I modified the dual cdc example to pull the rgb Red and Blue low on connection leaving green (boots to white)

In order for the examples to work we need to offset the 128k from arduinos bootloader to the app, add    SCB->VTOR = 0x08040000;    to first line of main loop and the following includes to each example/firmware you wish to use (ld file already edited) 

#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include <math.h>



