PcxF3Font Gfx\Font.pcx Data\Font.bin
bin2elf Data\Font.bin Font Data\Font.o
PcxToF3 0 Gfx\Chaos.pcx e29-08.rom e29-07.rom e29-06.rom Data\Chaos.pal
bin2elf Data\Chaos.pal ChaosPal Data\ChaosPal.o
MapToF3Map 0 20 15 Gfx\Chaos.map Data\Chaos.map 4
bin2elf Data\Chaos.map ChaosMap Data\ChaosMap.o
PcxToF3Sprite 64 Gfx\Sprite.pcx e29-02.rom e29-01.rom Data\Sprite.pal
bin2elf Data\Sprite.pal SpritePal Data\SpritePal.o
copy e29-01.rom \mame\roms\pbobble3
copy e29-02.rom \mame\roms\pbobble3
copy e29-06.rom \mame\roms\pbobble3
copy e29-07.rom \mame\roms\pbobble3
copy e29-08.rom \mame\roms\pbobble3
del Debug\startup.o


