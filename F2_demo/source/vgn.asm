; Do common setup stuff
    INCLUDE "defines.inc"

	org    $0

    dc.l $0010FFFC, START
    dc.l INT,INT,INT,INT,INT,INT,INT     
    dc.l INT,INT,INT,INT,INT,INT,INT,INT 
    dc.l INT,INT,INT,INT,INT,INT,INT,INT 
    dc.l INT,INT,INT,INT,INT,VBL,INT,INT 
    dc.l INT,INT,INT,INT,INT,INT,INT,INT 
    dc.l INT,INT,INT,INT,INT,INT,INT,INT 
    dc.l INT,INT,INT,INT,INT,INT,INT,INT 
    dc.l INT,INT,INT,INT,INT,INT,INT

    org $400
INT:
	rte

VBL:
    move.w  d0, $B00002
    move.b  #$1, VertBlank
    rte

START:
    move.w  #$2700, sr

    move.w  #$0000, $820000
    move.w  #$0190, $820002
    move.w  #$0000, $820006
    move.w  #$0000, $820008

    lea     Font, a0
    move.l  #$806000, a1
    move.l  #$7ff, d0

FontLoop:
    move.w  (a0)+, (a1)+
    move.w  d0, $B00002
    dbra    d0, FontLoop

    lea     Palette, a0
    move.l  #$200000, a1
    move.w  #48, d0

PaletteLoop:
    move.w  (a0)+, (a1)+
    move.w  d0, $B00002
    dbra    d0, PaletteLoop

    lea     $804350, a0
    move.l  a0, a1
    lea     Message, a2
    move.w  #4, d1
    move.w  #39, d0
    move.w  #$0000, d7

TileLoop:
    move.b  (a2)+, d7
    move.w  d7, -(a0)
    move.w  d0, $B00002
    dbra    d0, TileLoop

    move.w  #39, d0
    sub.l   #128, a1
    move.l  a1, a0
    dbra    d1, TileLoop

    lea     $809db0, a0
    move.l  a0, a1
    lea     Map, a2
    move.w  #21, d1
    move.w  #39, d0

MapLoop:
    move.w  (a2)+, -(a0)
    move.w  #$0002, -(a0)
    move.w  d0, $B00002
    dbra    d0, MapLoop

    move.w  #39, d0
    sub.l   #256, a1
    move.l  a1, a0
    dbra    d1, MapLoop

    move.l  #$900000, a0
    lea     SpriteTable, a1

    move.w  #176, d0

SpriteLoop:
    move.w  (a1)+, (a0)+
    move.w  d0, $B00002
    dbra    d0, SpriteLoop

    move.b  #0, VertBlank
    move.w  #$2000, sr
 
EndLoop:
    cmp.b   #1, VertBlank
    bne     EndLoop

    move.w  $300004, d0
    not.w   d0

    btst    #3, d0
    beq     TestRight

    move.w  $900024, d1
    cmp.w   #0, d1
    beq     EndInput

    sub.w   #2, d1
    move.w  d1, $900024

    bra     EndInput

TestRight:
    btst    #2, d0
    beq     EndInput

    move.w  $900024, d1
    cmp.w   #$B0, d1
    beq     EndInput

    add.w   #2, d1
    move.w  d1, $900024

EndInput:
    move.b  #0, VertBlank

    bra     EndLoop

Font:
    INCBIN "font.bin"
FontEnd:
    FontSize size FontEnd-Font

Palette:
    INCBIN "font.pal"
    INCBIN "arcdev.pal"
    INCBIN "vgnlogo.pal"

Map:
    INCBIN "vgnlogo.mp1"

SpriteTable:
    dc.w $0000, $0000, $0000, $0000, $0000, $0000, $0000, $0000
    dc.w $0000, $0000, $0000, $0000, $0000, $0000, $0000, $0000
    dc.w $0000, $0000, $0058, $0054, $0000, $0000, $0000, $0000
    dc.w $0012, $0000, $0000, $0000, $0801, $0000, $0000, $0000
    dc.w $0011, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $0010, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $000F, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $000E, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $000D, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $000C, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $000B, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $000A, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $0009, $0000, $0000, $0000, $3C01, $0000, $0000, $0000
    dc.w $0008, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $0007, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $0006, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $0005, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $0004, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $0003, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $0002, $0000, $0000, $0000, $DC01, $0000, $0000, $0000
    dc.w $0001, $0000, $0000, $0000, $D401, $0000, $0000, $0000
    dc.w $0000, $0000, $0000, $0000, $0000, $0000, $0000, $0000

;            *        1         2         3     *
;           01234567890123456789012345678901234567890
Message:
    dc.b   "WELCOME TO THE FIRST EVER TAITO F2 DEMO."
    dc.b   "THIS DEMO ALSO MARKS THE ARCADE DEV SITE"
    dc.b   "MOVE TO VINTAGEGAMING.COM. YEAH!!       "
    dc.b   "THANKS TO DAVE FOR HOSTING THE SITE.    "
    dc.b   "CONTACT ME AT CDOTY@NETZERO.NET         "
