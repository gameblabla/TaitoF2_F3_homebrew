* Taito F3 Demo

	.include "Includes/Defines.inc"

********************** Exported Symbols **********************
	.globl	_start

	.extern	Font
	.extern	ChaosMap
	.extern	ChaosPal
	.extern	SpritePal

	dc.l	0x41FFFC, _start,  Default, Default, Default, Default, Default, Default
	dc.l	Default,  Default, Default, Default, Default, Default, Default, Default
	dc.l	Default,  Default, Default, Default, Default, Default, Default, Default
	dc.l	Default,  Default, VBlank,  VHard,   Default, Default, Default, Default
	dc.l	Default,  Default, Default, Default, Default, Default, Default, Default
	dc.l	Default,  Default, Default, Default, Default, Default, Default, Default
	dc.l	Default,  Default, Default, Default, Default, Default, Default, Default
	dc.l	Default,  Default, Default, Default, Default, Default, Default, Default

	.align	4

Default:
	rte

	.align	4

VBlank:
	* Activate watchdog timer
	move.b	#0x00, 0x4A0000
	
	move.w	#1, VertBlank

	* Set sprite 1 x position
	move.w	Sprite1XPos, 0x600014 

	* Set sprite 2 x position
	move.w	Sprite2XPos, 0x600264 

	rte

	.align	4

VHard:
	rte

	.align	4

_start:
	* Disable interrupts
	ori.w	#0x0700, sr		

	* Setup stack pointer
	move.l	#0x420000, sp

	* Activate watchdog timer
	move.b	#0x00, 0x4A0000
	
	* Reset coin/joysticks
	jsr		ResetJoysticks

	* Unknown
	move.w	#0x278B, 0x4C0000

	* Setup playfields
	jsr		SetupPlayfields

	* Activate watchdog timer
	move.b	#0x00, 0x4A0000
	
	* Clear ram
	lea		0x400000, a0
	move.l	#0x00, d0
	move.l	#0x1DFD0, d1
	lsr.l	#2,	d1
	sub.l	#1, d1

	jsr		ClearMemory

	* Set initial values
	move.w	#19, Scroll1Pos
	move.w	#20, Scroll2Pos
	move.w	#0x0090, Sprite1XPos
	move.w	#0x00D0, Sprite2XPos
	move.w	#8, AnimatePos
	move.w	#DELAY, AnimateDelay
	move.w	#0, Direction

	* Clear line ram
	jsr		ClearLineRam

	* Clear scroll
	lea		0x624000, a0
	move.w	#0x0000, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear sprite alpha
	lea		0x626000, a0
	move.w	#0x02FF, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear alpha
	lea		0x626200, a0
	move.w	#0xBCBA, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear tile scale
	lea		0x626400, a0
	move.w	#0x7000, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Enable character screen
	lea		0x627000, a0
	move.w	#0x0001, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear CRAM priorities
	lea		0x627200, a0
	move.w	#0x300F, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear sprite clipping
	lea		0x627400, a0
	move.w	#0x0300, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear sprite priorities
	lea		0x627600, a0
	move.w	#0x5DDD, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 1 scale
	lea		0x628000, a0
	move.w	#0x0080, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 2/4 scale
	lea		0x628200, a0
	move.w	#0x0080, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 3 scale
	lea		0x628400, a0
	move.w	#0x0080, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 2/4 scale
	lea		0x628600, a0
	move.w	#0x0080, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 1 rowscroll
	lea		0x62A000, a0
	move.w	#0x003F, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 2 rowscroll
	lea		0x62A200, a0
	move.w	#0x003F, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 3 rowscroll
	lea		0x62A400, a0
	move.w	#0x003F, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 4 rowscroll
	lea		0x62A600, a0
	move.w	#0x003F, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 1 priorities
	lea		0x62B000, a0
	move.w	#0x300B, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 2 priorities
	lea		0x62B200, a0
	move.w	#0x3009, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 3 priorities
	lea		0x62B400, a0
	move.w	#0x3003, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Clear playfield 4 priorities
	lea		0x62B600, a0
	move.w	#0x3001, d0
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	* Set the sync register
	move.w	#0x0800, 0x626000

	* Activate watchdog timer
	move.b	#0x00, 0x4A0000
	
	* Transfer font to VRAM
	lea		Font, a0
	lea		0x61E000, a1
	move.w	#(8192/4)-1, d0
	
FontCopyLoop:
	move.l	(a0)+, (a1)+
	dbra	d0, FontCopyLoop

	* Transfer font palettes (Fonts use the first 64 colors)
	lea		TextPal, a0
	lea		0x440000, a1
	move.w	#64-1, d0
	
FontPaletteCopyLoop:
	move.l	(a0)+, (a1)+
	dbra	d0, FontPaletteCopyLoop

	* Transfer image palette
	lea		ChaosPal, a0
	lea		0x440100, a1
	move.w	#64-1, d0
	
ImagePaletteCopyLoop:
	move.l	(a0)+, (a1)+
	dbra	d0, ImagePaletteCopyLoop

	* Transfer sprite palette
	lea		SpritePal, a0
	lea		0x444000, a1
	move.w	#64-1, d0
	
SpritePaletteCopyLoop:
	move.l	(a0)+, (a1)+
	dbra	d0, SpritePaletteCopyLoop

	* Transfer the tilemap to the 1st playfield
	move.l	#15-1, d0
	lea		ChaosMap, a0
	lea		0x610000, a1

MapCopyOuterLoop:
	* Draw 20 tiles across the tilemap
	move.l	#20-1, d1

MapCopyInnerLoop:
	move.l	(a0)+, (a1)+
	dbra	d1, MapCopyInnerLoop

	* Skip to the start of the next line on the tilemap.
	add.l	#128-(20*4), a1

	dbra	d0, MapCopyOuterLoop

	* Transfer 9 rows of 8 sprites per row, plus the two pad sprites. Each sprite is 4 DWORDs.
	move.l	#((9*8+2)*4)-1, d0
	lea		Sprites, a0
	lea		0x600000, a1

SpriteCopyLoop:
	move.l	(a0)+, (a1)+
	dbra	d0, SpriteCopyLoop

	* Draw a vertical text strip using palette 0
	lea		Text, a0
	move.w	Scroll1Pos, d0
	move.l	#0, d1
	jsr		DrawTextLine

	* Draw a vertical text strip using palette 0
	lea		Text, a0
	move.w	Scroll2Pos, d0
	move.l	#0, d1
	jsr		DrawTextLine

	* Draw a string
	move.w	#0, d0
	lea		String, a0
	lea		0x61CD1A, a1
	jsr		DrawString

	* Clear vertical blank flag
	move.w	#0, VertBlank

	* Enable interrupts
	move.w	#0x2000, sr

Loop:
	jsr		WaitVBlank

	* Only check for button presses when the sprites are not moving
	cmp.w	#0, Direction
	beq.s	CheckInput
	
	cmp.w	#2, Direction
	beq.s	CheckInput
	
	bra		NoInputChange

CheckInput:
	* Read the buttons
	move.l	0x4A0000, d0
	not.l	d0
	
	* Mask out the test switches, start buttons, coins, tilt, and service switches
	and.l	#0x000000FF, d0
	beq.s	NoInputChange

	* Starts the logo moving, based on direction.
	add.w	#1, Direction

	* Remove the 'PRESS A BUTTON' message.
	move.w	#0, d0
	lea		BlankString, a0
	lea		0x61CD1A, a1
	jsr		DrawString

NoInputChange:
	* Move the sprites away from the center and draw vertical text strip
	cmp.w	#1, Direction
	bne		CheckNextDirection

	* Delay between animation changes
	sub.w	#1, AnimateDelay
	bne		Loop

	* Reset delay, and update sprite positions
	move.w	#DELAY, AnimateDelay
	sub.w	#1, Sprite1XPos
	add.w	#1, Sprite2XPos

	* Check if the left sprite is off screen
	cmp.w	#-20, Sprite1XPos
	bne.s	NormalMove

	* Switch to the non moving state
	move.w	#2, Direction
	move.w	#8, AnimatePos

	* Draw the 'PRESS A BUTTON' message.
	move.w	#0, d0
	lea		String, a0
	lea		0x61CD1A, a1
	jsr		DrawString

	sub.w	#1, Scroll1Pos
	add.w	#1, Scroll2Pos

	bra		Loop
	
NormalMove:
	sub.w	#1, AnimatePos
	bne		Loop

	move.w	#8, AnimatePos

	sub.w	#1, Scroll1Pos

	lea		Text, a0
	move.w	Scroll1Pos, d0
	move.l	#0, d1
	jsr		DrawTextLine

	add.w	#1, Scroll2Pos

	lea		Text, a0
	move.w	Scroll2Pos, d0
	move.l	#0, d1
	jsr		DrawTextLine

	bra		Loop

CheckNextDirection:
	cmp.w	#3, Direction
	bne		Loop

	sub.w	#1, AnimateDelay
	bne		Loop

	move.w	#DELAY, AnimateDelay
	add.w	#1, Sprite1XPos
	sub.w	#1, Sprite2XPos

	cmp.w	#0x0090, Sprite1XPos
	bne.s	NormalMoveBack

	move.w	#0, Direction
	move.w	#8, AnimatePos

	* Draw the 'PRESS A BUTTON' message.
	move.w	#0, d0
	lea		String, a0
	lea		0x61CD1A, a1
	jsr		DrawString

	add.w	#1, Scroll1Pos
	sub.w	#1, Scroll2Pos

	* Draw a vertical text strip using palette 0
	lea		Text, a0
	move.w	Scroll1Pos, d0
	move.l	#0, d1
	jsr		DrawTextLine

	* Draw a vertical text strip using palette 0
	lea		Text, a0
	move.w	Scroll2Pos, d0
	move.l	#0, d1
	jsr		DrawTextLine

	bra		Loop
	
NormalMoveBack:
	sub.w	#1, AnimatePos
	bne		Loop

	move.w	#8, AnimatePos

	add.w	#1, Scroll1Pos

	move.w	Scroll1Pos, d0
	move.l	#0, d1
	jsr		ClearTextLine

	sub.w	#1, Scroll2Pos

	move.w	Scroll2Pos, d0
	move.l	#0, d1
	jsr		ClearTextLine

	bra		Loop

WaitVBlank:
	* Wait for the value to be changed by the VBlank interrupt
	cmp		#1, VertBlank
	bne.s	WaitVBlank

	* Reset the vertical blank flag for next time
	move.w	#0, VertBlank

	rts

ResetJoysticks:
	* Reset coin/joysticks
	move.b	#0x00, 0x4A0000
	move.b	#0x00, 0x4A0006
	move.b	#0xFF, 0x4A0016
	move.b	#0x00, 0x4A0004
	move.b	#0x00, 0x4A0014

	rts

SetupPlayfields:
	* Set playfields to 512x512
	move.w	#0x00, 0x66001E

	* Set X scroll values
	move.w	#0xF600, 0x660000
	move.w	#0xF600, 0x660002
	move.w	#0xF600, 0x660004
	move.w	#0xF600, 0x660006

	* Set Y scroll values
	move.w	#0xF380, 0x660008
	move.w	#0xF380, 0x66000A
	move.w	#0xF380, 0x66000C
	move.w	#0xF380, 0x66000E
	
	* Unused (always 0)
	move.w	#0, 0x660010
	move.w	#0, 0x660012
	move.w	#0, 0x660014
	move.w	#0, 0x660016

	* Pixel + VRAM X scroll
	move.w	#0x2A, 0x660018

	* Pixel + VRAM Y scroll
	move.w	#0x04, 0x66001A

	* Unused (always 0)
	move.w	#0, 0x66001C
	
	move.w	#0x00, 0x66001E

	* Set playfield 1 priority
	clr.w	0x62B02E

	* Set playfield 2 priority
	clr.w	0x62B22E

	* Set playfield 3 priority
	clr.w	0x62B42E

	* Set playfield 4 priority
	clr.w	0x62B62E

	rts

* A0 - Memory to clear
* D0 - Clear value (dword)
* D1 - Number of dwords to clear
ClearMemory:
	* Activate watchdog timer
	move.b	#0x00, 0x4A0000

ClearMemoryLoop:
	move.l	d0, (a0)+
	
	dbra	d1, ClearMemoryLoop

	rts	

* A0 - Memory to clear
* D0 - Clear value (dword)
* D1 - Number of dwords to clear
ClearMemoryWord:
	* Activate watchdog timer
	move.b	#0x00, 0x4A0000

ClearMemoryWordLoop:
	move.w	d0, (a0)+
	dbra	d1, ClearMemoryWordLoop

	rts

ClearLineRam:
	* Activate watchdog timer
	move.b	#0x00, 0x4A0000

	* Clear line ram sections
	lea		0x620000, a0
	
	move.l	#0x624000, d0
	lsr.l	#8, d0
	lsr.l	#4, d0
	lsl.l	#8, d0
	lsl.l	#2, d0
	addi.w	#0x0F, d0
	andi.w	#0x7FFF, d0

	* Clear 8 sections
	moveq	#7, d3

ClearLineRamOuterLoop:
	move.w	#0x00FF, d1

	jsr		ClearMemoryWord

	addi.w	#0x0400, d0

	dbra	d3, ClearLineRamOuterLoop

	lea		0x621000, a0

	move.w	#0xF0, (a0)+
	moveq	#0x00, d0
	move.w	#0x1000, d1
	lsr.w	#1, d1
	subq.w	#2, d1

	jsr		ClearMemoryWord
	
	rts

* D0 - Line number
* D1 - Palette number
* A0 - String pointer (40 character)
DrawTextLine:
	* Get the starting location of the string
	add.l	d0, a0

	* Get the starting location for the screen
	lea		0x61C480, a1
	
	rol.l	#1, d0
	add.l	d0, a1	
	
	* Move palette to upper byte
	rol.w	#8, d1
	rol.w	#2, d1

	* Draw 14 lines
	move.l	#14-1, d0

DrawTextLineLoop:
	move.b	(a0), d1
	move.w	d1, (a1)

	* Advance to the next line of text
	add.l	#40, a0

	* Advance to the next line on the text tilemap
	add.l	#128, a1

	dbra	d0, DrawTextLineLoop

	rts

	.global	ClearTextLine

* D0 - Line number
* D1 - Clear value
ClearTextLine:
	* Get the starting location for the screen
	lea		0x61C480, a1
	
	rol.l	#1, d0
	add.l	d0, a1	
	
	* Clear 14 lines
	move.l	#14-1, d0

ClearTextLineLoop:
	move.w	d1, (a1)

	* Advance to the next line of text
	add.l	#40, a0

	* Advance to the next line on the text tilemap
	add.l	#128, a1

	dbra	d0, ClearTextLineLoop

	rts

* D0 - Palette
* A0 - String pointer
* A1 - Screen pointer
DrawString:
	* Move palette to upper byte
	rol.w	#8, d0
	rol.w	#2, d0

DrawStringLoop:
	move.b	(a0)+, d0
	beq.s	DrawStringExit

	move.w	d0, (a1)+

	bra.s	DrawStringLoop

DrawStringExit:
	rts

Text:		 
	*		 		   1		 2		   3
	*		 0123456789012345678901234567890123456789
	.ascii	"CHAOS PRESENTS THE 18TH ANNIVERSARY DEMO"
	.ascii	"                                        "
	.ascii	"CHAOS WAS FORMED AS A DEMO GROUP ON THE "
	.ascii	"COMMODORE 64 COMPUTER.                  "
	.ascii	"                                        "
	.ascii	"CHAOS HAS BEEN TWO MEMBERS FOR MOST OF  "
	.ascii	"ITS HISTORY:                            "
	.ascii	"RASTER   - SOFTWARE FOR MANY SYSTEMS    "
	.ascii	"PICKSTER - HARDWARE HACKING, MODDING,   "
	.ascii	"           AND LED ADDITIONS            "
	.ascii	"                                        "
	.ascii	"VISIT HTTP://WWW.CHAOS89.COM            "
	.ascii	"                                        "
	.ascii	"***********LONG LIVE POOPER*************"

String:
	.ascii	"PRESS A BUTTON"
	dc.b	0

BlankString:
	.ascii	"              "
	dc.b	0

TextPal:
	dc.l	0x00000000, 0x00FFFFFF, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	
	dc.l	0x00000000, 0x00CCCCCC, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000

	dc.l	0x00000000, 0x00888888, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	
	dc.l	0x00000000, 0x00444444, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000
	dc.l	0x00000000, 0x00000000, 0x00000000, 0x00000000

Sprites:
	* Blank sprite entry
	dc.l	0x0000FFFF, 0x00000000, 0x00000000, 0x00000000

	* Mark the beginning of a sprite
	dc.l	0x00010000, 0x0090003C, 0x08000000, 0x00000000	

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00020000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00030000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00040000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00090000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x000A0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x000B0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x000C0000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00110000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00120000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00130000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00140000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00190000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x001A0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x001B0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x001C0000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00210000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00220000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00230000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00240000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00290000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x002A0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x002B0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x002C0000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00310000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00320000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00330000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00340000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00390000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x003A0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x003B0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x003C0000, 0x00000000, 0xD8000000, 0x00000000
	
	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00410000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00420000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00430000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00440000, 0x00000000, 0xD8000000, 0x00000000

	* Blank sprite entry
	dc.l	0x0000FFFF, 0x00000000, 0x00000000, 0x00000000

	* Mark the beginning of a sprite
	dc.l	0x00050000, 0x00D0003C, 0x08000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00060000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00070000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00080000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x000D0000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x000E0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x000F0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00100000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00150000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00160000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00170000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00180000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x001D0000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x001E0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x001F0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00200000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00250000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00260000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00270000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00280000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x002D0000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x002E0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x002F0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00300000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00350000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00360000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00370000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00380000, 0x00000000, 0xD8000000, 0x00000000

	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x003D0000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x003E0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x003F0000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00400000, 0x00000000, 0xD8000000, 0x00000000
	
	* Add 16 to the y, mark as continues sprite, use saved x and current y
	dc.l	0x00450000, 0x00000000, 0x38000000, 0x00000000

	* Add 16 to the x, mark as continues sprite, use current x and current y
	dc.l	0x00460000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00470000, 0x00000000, 0xD8000000, 0x00000000
	dc.l	0x00480000, 0x00000000, 0xD8000000, 0x00000000
