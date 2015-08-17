PROJECT = main

OUTDIR = build
EXECUTABLE = $(OUTDIR)/$(PROJECT).elf
BIN_IMAGE = $(OUTDIR)/$(PROJECT).bin
HEX_IMAGE = $(OUTDIR)/$(PROJECT).hex
MAP_FILE = $(OUTDIR)/$(PROJECT).map
LIST_FILE = $(OUTDIR)/$(PROJECT).lst

# Set the path to STM32F4xx firmware package
STM32_LIB = lib/CORTEX_M4F_STM32F4/Libraries/STM32F4xx_StdPeriph_Driver

# Set the path to STM32F429I-Discovery firmware package
STM32DISC_LIB = lib/Utilities/STM32F429I-Discovery

# Toolchain configurations
CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc-4.8
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE = $(CROSS_COMPILE)size

# Cortex-M4 implements the ARMv7E-M architecture
CPU = cortex-m4
CFLAGS = -mcpu=$(CPU) -march=armv7e-m -mtune=cortex-m4
CFLAGS += -mlittle-endian -mthumb
# Need study, FPU
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

define get_library_path
    $(shell dirname $(shell $(CC) $(CFLAGS) -print-file-name=$(1)))
endef

# Basic configurations
CFLAGS += -g -std=c99
CFLAGS += -Wall -Werror
CFLAGS += -DUSER_NAME=\"$(USER)\"

# Optimizations
CFLAGS += -O0 -ffast-math
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections
CFLAGS += -fno-common
CFLAGS += --param max-inline-insns-single=1000

# specify STM32F429
CFLAGS += -DSTM32F429_439xx

# to run from FLASH
CFLAGS += -DVECT_TAB_FLASH
LDFLAGS += -T $(PWD)/lib/CORTEX_M4F_STM32F4/stm32f429zi_flash.ld
LDFLAGS += -L $(call get_library_path,libc.a)
LDFLAGS += -L $(call get_library_path,libgcc.a)

# STM32F4xx_StdPeriph_Driver
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -D"assert_param(expr)=((void)0)"

#files
SRCDIR += \
	src \

SRC += $(wildcard $(addsuffix /*.c,$(SRCDIR))) \
	$(wildcard $(addsuffix /*.s,$(SRCDIR)))

SRC += lib/CORTEX_M4F_STM32F4/startup_stm32f429_439xx.s \
       lib/CORTEX_M4F_STM32F4/startup/system_stm32f4xx.c \

SRC += $(STM32_LIB)/src/misc.c \
	$(STM32_LIB)/src/stm32f4xx_gpio.c \
	$(STM32_LIB)/src/stm32f4xx_rcc.c \
	$(STM32_LIB)/src/stm32f4xx_usart.c \
	$(STM32_LIB)/src/stm32f4xx_syscfg.c \
	$(STM32_LIB)/src/stm32f4xx_i2c.c \
	$(STM32_LIB)/src/stm32f4xx_dma.c \
	$(STM32_LIB)/src/stm32f4xx_spi.c \
	$(STM32_LIB)/src/stm32f4xx_exti.c \
	$(STM32_LIB)/src/stm32f4xx_dma2d.c \
	$(STM32_LIB)/src/stm32f4xx_ltdc.c \
	$(STM32_LIB)/src/stm32f4xx_fmc.c \
	$(STM32_LIB)/src/stm32f4xx_rng.c \

SRC += $(STM32DISC_LIB)/stm32f429i_discovery.c \
	$(STM32DISC_LIB)/stm32f429i_discovery_sdram.c \
	$(STM32DISC_LIB)/stm32f429i_discovery_l3gd20.c \
	$(STM32DISC_LIB)/stm32f429i_discovery_lcd.c \
	$(STM32DISC_LIB)/stm32f429i_discovery_ioe.c \


OBJS += $(addprefix $(OUTDIR)/,$(patsubst %.s,%.o,$(SRC:.c=.o)))

INCDIR = lib/CORTEX_M4F_STM32F4 \
	 lib/CORTEX_M4F_STM32F4/board \
	 lib/CORTEX_M4F_STM32F4/Libraries/CMSIS/Device/ST/STM32F4xx/Include \
	 lib/CORTEX_M4F_STM32F4/Libraries/CMSIS/Include \
	 $(STM32_LIB)/inc \
	 lib/Utilities/Common \
	 lib/Utilities/STM32F429I-Discovery \

INCLUDES = $(addprefix -I, $(INCDIR))

all:
	@echo "Using : make clean && make main && make flash"

main: $(EXECUTABLE)-main
	@echo "OBJCOPY	$(BIN_IMAGE)"
	@$(OBJCOPY) -O binary $^ $(BIN_IMAGE) 
	@echo "OBJCOPY	$(HEX_IMAGE)"
	@$(OBJCOPY) -O ihex $^ $(HEX_IMAGE) 
	@echo "OBJDUMP	$(LIST_FILE)"
	@$(OBJDUMP) -h -S -D $^ > $(LIST_FILE) 
	@echo "SIZE $^"
	@$(SIZE) $^

$(EXECUTABLE)-main: $(OUTDIR)/src/main.o $(OBJS)
	@echo "LD	$@"
	@$(LD) -o $@ $^ -lm -lc -lnosys -Map=$(MAP_FILE)	$(LDFLAGS)

$(OUTDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "CC	$@"
	@$(CC) $(CFLAGS)  -c $(INCLUDES)  $< -o $@

$(OUTDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo "CC	$@"
	@$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

flash:
	st-flash write $(BIN_IMAGE) 0x8000000

openocd_flash:
	openocd \
	-f board/stm32f429discovery.cfg \
	-c "init" \
	-c "reset init" \
	-c "flash probe 0" \
	-c "flash info 0" \
	-c "flash write_image erase $(BIN_IMAGE)  0x08000000" \
	-c "reset run" -c shutdown

.PHONY: clean all flash openocd_flash 

clean:
	rm -f $(EXECUTABLE)*
	rm -f $(BIN_IMAGE)
	rm -f $(HEX_IMAGE)
	rm -f $(LIST_FILE)
	rm -f $(MAP_FILE)
	rm -f $(OBJS)
	rm -rf $(OUTDIR) 
