NAME	= Demo
CC 		= gcc
CCFLAGS = -m68020 -Wall -nostartfiles -nodefaultlibs -fno-builtin -fomit-frame-pointer -ffast-math
AS 		= as
ASFLAGS = -m68020 --register-prefix-optional
LD 		= gcc
LDFLAGS = -Wl,-Map,$(NAME).map
BIN		= objcopy
INCPATH	= \NeoDev\include
LIBS 	=
LIBPATH	= \NeoDev\lib
ROMSIZE = 0x200000
PADBYTE = 0x00
OUTPUT	= F3Cart
BUILD	= Debug
FILES	= $(BUILD)\StartUp.o
DATA	= Data\Font.o Data\ChaosPal.o Data\ChaosMap.o Data\SpritePal.o
PATH	= \NeoDev\bin;\Dos;%PATH%

all : $(NAME).bin

$(NAME).bin: $(BUILD)\$(NAME).obj
	$(BIN) --gap-fill=$(PADBYTE) --pad-to=$(ROMSIZE) -R .data -O binary $(BUILD)\$(NAME).obj $(NAME).bin
	splitf3 $(NAME).bin e29-12.rom e29-11.rom e29-10.rom e29-16.rom
	copy e29-10.rom \mame\roms\pbobble3
	copy e29-11.rom \mame\roms\pbobble3
	copy e29-12.rom \mame\roms\pbobble3
	copy e29-16.rom \mame\roms\pbobble3

$(BUILD)\$(NAME).obj: $(FILES)
	$(LD) -L$(LIBPATH) $(CCFLAGS) $(LDFLAGS) -T \NeoDev\lib\ldscripts\$(OUTPUT).x -o $(BUILD)\$(NAME).obj $(FILES) $(DATA) $(LIBS)

$(BUILD)\StartUp.o : StartUp.s
	$(AS) $(ASFLAGS) -o $(BUILD)\StartUp.o StartUp.s 2>&1 | Gcc2MS

CLEAN :
	-@erase $(BUILD)\StartUp.o
	-@erase $(BUILD)\$(NAME).obj
	-@erase $(NAME).bin
	-@erase *.rom

