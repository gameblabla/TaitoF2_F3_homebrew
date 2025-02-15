Simple Taito F3 demo by Charles Doty (cdoty2@houston.rr.com)

This is, as far as I can tell, a fully functioning Taito F3 rom. It works
under MAME. It doesn't include alot of functionality, but includes everything needed to write a
full game (minus sound).

Included in this archive is the demo, source, and tools used to create the
program.

To view the demo, unzip pbobble3.zip to \mame\roms\pbobble3, and type 'mame pbobble3'.
You will need the original pbobble3 roms, as I only include the roms that I created, to
avoid any copyright issues.

You will also need to get the following if you want to recompile the demo:
The NeoDev 68k GCC compiler (http://frogfeast.rastersoft.net/Files/NeoDev.zip)

Before compiling the demo, you will need to run gfx.bat, with the \NeoDev\bin directory and
the included tools in the path.

The demo can be built with Visual Studio 6 or with the included mk.bat file. The mk.bat file
also needs \NeoDev\bin in the path.

Thanks to:
  The MAMEDEV Team (Specifically the IGS PGM crew)
    - The source code provided a lot of information about the F3 hardware, and the debugger 
      is invaluable.

A site dedicated to arcade system development is available at http://arcadedev.emuvibes.com.

