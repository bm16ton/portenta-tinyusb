CFLAGS += -DCORE_CM7 -DSTM32H747xx -DHSE_VALUE=25000000

# Default is Highspeed port
PORT ?= 1
SPEED ?= high
#define BOARD_DEVICE_RHPORT_SPEED   OPT_MODE_HIGH_SPEED
SRC_S += $(ST_CMSIS)/Source/Templates/gcc/startup_stm32h747xx.s
LD_FILE = $(BOARD_PATH)/stm32h743xx_flash.ld

# For flash-jlink target
JLINK_DEVICE = stm32h743xi

# flash target using on-board stlink
flash: flash-stlink
