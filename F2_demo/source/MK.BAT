maccer -tjas -o vgn.s vgn.asm
jas -N vgn.s -o vgn.o
segald -g -o vgn.bin vgn.o
splitf2 vgn.bin lq11.bin lq09.bin lq12.bin lq10.bin
copy lq*.bin c:\mame\liquidk
move lq*.bin c:\raine\roms\liquidk
del *.o
del *.s
del vgn.bin
