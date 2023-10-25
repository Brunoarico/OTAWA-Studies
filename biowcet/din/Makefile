# Compiler and flags
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
MCUFLAGS = -mthumb -mcpu=cortex-m3
CFLAGS = -O0 -std=c99 -DSTM32F103x6 -g3 
INCLUDE = -I./aux_files/inc -I./aux_files/cmsis
DFLAGS = -DCRON#-DSIZE=150

# Linker and flags
LD = arm-none-eabi-ld
LDFLAGS = -T./aux_files/scripts/STM32F103X6_FLASH.ld --specs=nosys.specs --specs=nano.specs 

# Source files
SRCDIR = src
ASMDIR = aux_files/startup
SRC = $(wildcard $(SRCDIR)/*.c)
ASM := $(wildcard $(ASMDIR)/*.s)

# Output files
APP_NAME = main
OUTDIR = ./build
ELF = $(OUTDIR)/$(APP_NAME).elf
ELF_OTAWA = $(OUTDIR)/$(APP_NAME)_otawa.elf
BIN = $(OUTDIR)/$(APP_NAME).bin

# Utilities
OCD = openocd
GDB = gdb-multiarch
FLASH = st-flash

.DEFAULT_GOAL = all

.PHONY: all clean flash debug test

# Variáveis para objetos com e sem MCUFLAGS
OBJ_WITH_MCUFLAGS = $(SRC:$(SRCDIR)/%.c=$(OUTDIR)/%.o) $(ASM:$(ASMDIR)/%.s=$(OUTDIR)/%.o)
OBJ_WITHOUT_MCUFLAGS = $(SRC:$(SRCDIR)/%.c=$(OUTDIR)/no_mcuflags_%.o) $(ASM:$(ASMDIR)/%.s=$(OUTDIR)/%.o)

all: $(BIN) $(ELF_OTAWA)
	@mkff $(ELF_OTAWA) > $(OUTDIR)/$(APP_NAME)_empty.ff
	orange --auto $(SRCDIR)/main.c main -o $(OUTDIR)/$(APP_NAME).xml > /dev/null

clean:
	rm -rf $(OUTDIR)
	rm -rf ./src/main.c

$(OUTDIR)/%.o: $(SRCDIR)/%.c | $(OUTDIR)
	
	$(CC) $(CFLAGS) $(MCUFLAGS) $(DFLAGS) $(INCLUDE) -c $< -o $@

$(OUTDIR)/no_mcuflags_%.o: $(SRCDIR)/%.c | $(OUTDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OUTDIR)/%.o: $(ASMDIR)/%.s | $(OUTDIR)
	$(AS) -c $< -o $@

$(BIN): $(ELF)
	arm-none-eabi-size $(ELF)
	arm-none-eabi-objcopy -R .stack -O binary $< $@
	arm-none-eabi-objdump -D -S $< > $@.lst

$(ELF_OTAWA): $(OBJ_WITHOUT_MCUFLAGS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(ELF): $(OBJ_WITH_MCUFLAGS)
	$(CC) $(CFLAGS) $(DFLAGS) $(MCUFLAGS) $(LDFLAGS) $^ -o $@

flash: $(BIN) | kill_openocd
	$(FLASH) write $< 0x8000000

debug: $(ELF)
	$(OCD) -f stlink_bluepill.cfg &
	$(GDB) -q -x hwdebug.py $<

test: flash debug

kill_openocd:
	@if pgrep -x "openocd" > /dev/null; then \
		pkill openocd; \
	fi

otawa: $(ELF_OTAWA)
	./xml2ff.py 
	owcet -s xmc4500 $(ELF_OTAWA)

$(OUTDIR):
	mkdir -p $@

wcet: flash debug otawa