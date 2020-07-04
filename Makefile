#------------------------------------
#
-include site.mk
include $(PROJDIR:%=%/)builder/proj.mk

.DEFAULT_GOAL=all

ARDUINO_DLC_PATH?=/home/joelai/.arduino15/packages/arduino

ARM_TOOLCHAIN_PATH?=$(ARDUINO_DLC_PATH)/tools/arm-none-eabi-gcc/7-2017q4
CROSS_COMPILE?=$(patsubst %gcc,%,$(notdir $(wildcard $(ARM_TOOLCHAIN_PATH)/bin/*gcc)))
EXTRA_PATH+=$(ARM_TOOLCHAIN_PATH:%=%/bin)

#BOSSA_PATH?=$(PROJDIR)/bossa
#BOSSAC?=bossac --info --debug --offset=0x2000 --verify \
#  --port=$(notdir $(wildcard /dev/ttyACM0 /dev/ttyACM1))
#EXTRA_PATH+=$(BOSSA_PATH:%=%/bin)

BOSSA_PATH?=$(ARDUINO_DLC_PATH)/tools/bossac/1.7.0-arduino3
BOSSAC?=bossac --info --debug -U true \
  --port=$(notdir $(firstword $(wildcard /dev/ttyACM0 /dev/ttyACM1)))
EXTRA_PATH+=$(BOSSA_PATH)

CMSIS_PATH?=$(ARDUINO_DLC_PATH)/tools/CMSIS/4.5.0/CMSIS

CMSIS_ATMEL_PATH=$(ARDUINO_DLC_PATH)/tools/CMSIS-Atmel/1.2.0/CMSIS/Device/ATMEL

ARDUINO_PLATFORM_PATH?=$(ARDUINO_DLC_PATH)/hardware/samd_beta/1.6.25

export PATH:=$(call ENVPATH,$(EXTRA_PATH))

$(info Makefile ... CROSS_COMPILE: $(CROSS_COMPILE), PATH: $(PATH)) 

#------------------------------------
#
sk1_CPPFLAGS+=-mcpu=cortex-m0plus -mthumb -g -Os -Wall -Wextra \
  -Wno-expansion-to-defined -ffunction-sections -fdata-sections \
  --param max-inline-insns-single=500 \
  -fno-rtti -fno-exceptions -MMD -DF_CPU=48000000L -DARDUINO=10812 \
  -DARDUINO_SAMD_MKRVIDOR4000 -DARDUINO_ARCH_SAMD_BETA -D__SAMD21G18A__ \
  -DUSB_VID=0x2341 -DUSB_PID=0x8056 -DUSBCON \
  "-DUSB_MANUFACTURER=\"Arduino LLC\"" \
  "-DUSB_PRODUCT=\"Arduino MKR Vidor 4000\"" \
  -DUSE_BQ24195L_PMIC \
  -I$(CMSIS_PATH)/Include -I$(CMSIS_ATMEL_PATH) \
  -I$(ARDUINO_PLATFORM_PATH)/cores/arduino \
  -I$(ARDUINO_PLATFORM_PATH)/variants/mkrvidor4000

sk1_CFLAGS+=-std=gnu11
sk1_CXXFLAGS+=-std=gnu++11 -fno-threadsafe-statics
sk1_LDFLAGS+=-Wl,--gc-sections \
  -Wl,-Map,$@.map \
  --specs=nano.specs --specs=nosys.specs \
  -Wl,--cref -Wl,--check-sections -Wl,--unresolved-symbols=report-all \
  -Wl,--warn-common -Wl,--warn-section-align \
  -lm

sk1_APP=$(BUILDDIR)/mkr/sk1

sk1_SRC+= \
  $(CMSIS_PATH)/Lib/GCC/libarm_cortexM0l_math.a \
  $(addprefix $(ARDUINO_PLATFORM_PATH)/variants/mkrvidor4000/, \
    clockout.c variant.cpp) \
  $(addprefix $(ARDUINO_PLATFORM_PATH)/cores/arduino/, \
    pulse_asm.S wiring.c hooks.c startup.c WInterrupts.c cortex_handlers.c \
    pulse.c delay.c itoa.c wiring_analog.c wiring_digital.c wiring_private.c \
    wiring_shift.c USB/samd21_host.c avr/dtostrf.c \
    IPAddress.cpp WMath.cpp Reset.cpp Uart.cpp Stream.cpp Tone.cpp SERCOM.cpp \
    Print.cpp WString.cpp abi.cpp main.cpp new.cpp USB/CDC.cpp USB/PluggableUSB.cpp \
    USB/USBCore.cpp) \
  $(ARDUINO_PLATFORM_PATH)/variants/mkrvidor4000/linker_scripts/gcc/flash_with_bootloader.ld \
  $(PWD)/test/sketch_feb12a.cpp

$(eval $(call BUILD2,sk1,mkr,$(sk1_SRC)))

sk1: $(sk1_APP)
	$(SIZE) -A $(sk1_APP)

sk1_install: sk1
	$(MKDIR) $(DESTDIR)
	$(OBJCOPY) -O binary $(sk1_APP) $(DESTDIR)/sk1.bin
	$(OBJCOPY) -O ihex -R .eeprom $(sk1_APP) $(DESTDIR)/sk1-eeprom.hex

sk1_flash:
	@echo "Force target reset ..."
	stty -F $(firstword $(wildcard /dev/ttyACM0 /dev/ttyACM1)) 1200
	sleep 1
	@echo "Assume target ready to flash ..."
	$(BOSSAC) -e -w $(DESTDIR)/sk1.bin -R

#------------------------------------
#
$(eval $(call BUILD2_OBJGEN,mkr))

#------------------------------------
#------------------------------------
#------------------------------------
#------------------------------------
