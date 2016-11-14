BINDIR = C:\OlimexODS\yagarto\bin\\

CROSS = arm-none-eabi-

CC = $(BINDIR)$(CROSS)gcc
OBJCOPY = $(BINDIR)$(CROSS)objcopy
CFLAGS = -Wall -Dat91sam7l128 -mcpu=arm7tdmi -marm

LDFLAGS = -nostartfiles -Wl,--gc-sections -static
# -Wl,--print-map
#-Wl,--verbose 

LDSCRIPT = at91sam7l128.lds

DIR = lab4

SRC = main.c lcd.c keyboard.c

OBJS =	crt0.o $(SRC:.c=.o)

HEADERS = AT91SAM7L128.h lcd.h keyboard.h flash.cfg hp-20b-calculator.cfg \
	at91sam7l128.cfg flash.cfg flash.bat flash-h.bat clean.bat

ZIPFILES = Makefile $(LDSCRIPT) $(SRC) $(HEADERS) crt0.S

main.hex :

main.elf : $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) -T$(LDSCRIPT) -o main.elf $(OBJS)

%.hex : %.elf
	$(OBJCOPY) -O ihex $< $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.o : %.S
	$(CC) $(CFLAGS) -c $<

$(DIR).zip : $(ZIPFILES)
	cd .. && zip $(DIR)/$@ $(ZIPFILES:%=$(DIR)/%)

.PHONY : clean
clean :
	rm -rf *.o *.elf *.hex
