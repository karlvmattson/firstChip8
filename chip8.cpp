#pragma once
#include <chip8.h>


chip8::chip8() {
   drawFlag = false;
}


void chip8::initialize() {

   // Initialize registers and memory once


   // Memory Map
   //   0x000 - 0x1FF - Chip 8 interpreter(contains font set in emu)
   //   0x050 - 0x0A0 - Used for the built in 4x5 pixel font set(0 - F)
   //   0x200 - 0xFFF - Program ROM and work RAM

}

void chip8::loadGame() {

}

void chip8::setKeys() {

}

void chip8::emulateCycle() {
   // Fetch Opcode
   // Decode Opcode
   // Execute Opcode

   // Update timers
}
