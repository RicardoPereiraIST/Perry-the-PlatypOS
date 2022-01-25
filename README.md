# Perry the PlatypOS

Perry is my experiment at learning the basics of OS development in C++.
It started with the basic tutorials from OS dev and some ideas from Thor-os. It eventually evolved into following the tutorials from BrokenThorn.

It contains the following:
- Multiboot setup
- GDT / IDT / PIC / PIT
- Physical and Virtual Memory
- Keyboard Driver
- FDC / DMA
- Archetype for VFS and Fat12 Filesystem
- TSS and User code

Known issues:
- The code and build scripts are not properly organized.
- C++ setup isn't complete (e.g., compilation issues with virtual destructors).
- Some early setup is incomplete (stack canary, stubs).
- A20 does not work correctly.
- Physical/Virtual Memory, FDC/DMA, and Filesystem were not used throughout development so there might be issues.

Running the project:
With a cross compiler - https://wiki.osdev.org/GCC_Cross-Compiler - and qemu, run the following command from inside the scripts folder:
> ./build_and_run run

Useful resources:
- https://wiki.osdev.org/Main_Page
- https://forum.osdev.org/index.php
- https://github.com/wichtounet/thor-os
- https://github.com/jdah/tetris-os
- http://www.brokenthorn.com/Resources/OSDevIndex.html
- https://github.com/SamyPesse/How-to-Make-a-Computer-Operating-System
- https://github.com/cfenollosa/os-tutorial
