as -m68020 --register-prefix-optional -o Debug\StartUp.o StartUp.s
gcc -L\NeoDev\lib -m68020 -Wall -nostartfiles -nodefaultlibs -fno-builtin -fomit-frame-pointer -ffast-math -Wl,-Map,Demo.map -T \NeoDev\lib\ldscripts\F3Cart.x -o Debug\Demo.obj Debug\StartUp.o Data\Font.o Data\ChaosPal.o Data\ChaosMap.o Data\SpritePal.o
objcopy --gap-fill=0x00 --pad-to=0x200000 -R .data -O binary Debug\Demo.obj Demo.bin
splitf3 Demo.bin e29-12.rom e29-11.rom e29-10.rom e29-16.rom
copy e29-10.rom \mame\roms\pbobble3
copy e29-11.rom \mame\roms\pbobble3
copy e29-12.rom \mame\roms\pbobble3
copy e29-16.rom \mame\roms\pbobble3

