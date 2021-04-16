#pragma once
#include <chip8.h>
#include <string>


unsigned char chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

chip8::chip8() {}

chip8::~chip8() {}

void chip8::initialize() {

   // Initialize registers and memory once
   pc = 0x200;
   opcode = 0;
   I = 0;
   sp = 0;

   // Clear display
   memset(gfx, 0, sizeof(gfx));
   drawFlag = false;

   // Clear stack
   memset(stack, 0, sizeof(stack));

   // Clear registers V0-Vf
   memset(V, 0, sizeof(V));

   // Clear memory
   memset(memory, 0, sizeof(memory));

   // Clear inputs
   memset(key, 0, sizeof(key));


   // Load fontset into memory
   for (int i = 0; i < 80; ++i)
      memory[i] = chip8_fontset[i];

   // Reset timers
   delay_timer = 0;
   sound_timer = 0;


   // Memory Map
   //   0x000 - 0x1FF - Chip 8 interpreter(contains font set in emu)
   //   0x050 - 0x0A0 - Used for the built in 4x5 pixel font set(0 - F)
   //   0x200 - 0xFFF - Program ROM and work RAM

}

void chip8::loadGame() {

   // use fopen in binary mode to load in a program
   //for (int i = 0; i < bufferSize; ++i)
   //   memory[i + 512] = buffer[i];
}

void chip8::setKeys() {

}

void chip8::emulateCycle() {
   // Fetch Opcode
   opcode = memory[pc] << 8 | memory[pc + 1];

   // Decode Opcode
   switch (opcode & 0xF000) {














      //   0NNN	Call		Calls machine code routine(RCA 1802 for COSMAC VIP) at address NNN.Not necessary for most ROMs.

   case 0x0000:
      switch (opcode & 0x000F) {
         //   00E0	Display	disp_clear()	Clears the screen.
         //   00EE	Flow	return;	Returns from a subroutine.

      default:
         printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
      }
      break;

      //   1NNN	Flow	goto NNN;	Jumps to address NNN.
      //   2NNN	Flow * (0xNNN)()	Calls subroutine at NNN.
      //   3XNN	Cond	if (Vx == NN)	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
      //   4XNN	Cond	if (Vx != NN)	Skips the next instruction if VX does not equal NN. (Usually the next instruction is a jump to skip a code block)
      //   5XY0	Cond	if (Vx == Vy)	Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
      //   6XNN	Const	Vx = NN	Sets VX to NN.
      //   7XNN	Const	Vx += NN	Adds NN to VX. (Carry flag is not changed)
      //   8XY0	Assign	Vx = Vy	Sets VX to the value of VY.
      //   8XY1	BitOp	Vx = Vx | Vy	Sets VX to VX or VY. (Bitwise OR operation)
      //   8XY2	BitOp	Vx = Vx & Vy	Sets VX to VX and VY. (Bitwise AND operation)
      //   8XY3[a]	BitOp	Vx = Vx ^ Vy	Sets VX to VX xor VY.
      //   8XY4	Math	Vx += Vy	Adds VY to VX.VF is set to 1 when there's a carry, and to 0 when there is not.
      //   8XY5	Math	Vx -= Vy	VY is subtracted from VX.VF is set to 0 when there's a borrow, and 1 when there is not.
      //   8XY6[a]	BitOp	Vx >>= 1	Stores the least significant bit of VX in VF and then shifts VX to the right by 1.[b]
      //   8XY7[a]	Math	Vx = Vy - Vx	Sets VX to VY minus VX.VF is set to 0 when there's a borrow, and 1 when there is not.
      //   8XYE[a]	BitOp	Vx <<= 1	Stores the most significant bit of VX in VF and then shifts VX to the left by 1.[b]
      //   9XY0	Cond	if (Vx != Vy)	Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block)
      
   case 0xA000:      //   ANNN	MEM	I = NNN	Sets I to the address NNN.
      // Execute opcode
      I = opcode & 0x0FFF;
      pc += 2;
      break;

      
      
      
      //   BNNN	Flow	PC = V0 + NNN	Jumps to the address NNN plus V0.
      //   CXNN	Rand	Vx = rand() & NN	Sets VX to the result of a bitwise and operation on a random number(Typically: 0 to 255) and NN.
      //   DXYN	Disp	draw(Vx, Vy, N)	Draws a sprite at coordinate(VX, VY) that has a width of 8 pixels and a height of N + 1 pixels.Each row of 8 pixels is read as bit - coded starting from memory location I; I value does not change after the execution of this instruction.As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen
      //   EX9E	KeyOp	if (key() == Vx)	Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
      //   EXA1	KeyOp	if (key() != Vx)	Skips the next instruction if the key stored in VX is not pressed. (Usually the next instruction is a jump to skip a code block)
      //   FX07	Timer	Vx = get_delay()	Sets VX to the value of the delay timer.
      //   FX0A	KeyOp	Vx = get_key()	A key press is awaited, and then stored in VX. (Blocking Operation.All instruction halted until next key event)
      //   FX15	Timer	delay_timer(Vx)	Sets the delay timer to VX.
      //   FX18	Sound	sound_timer(Vx)	Sets the sound timer to VX.
      //   FX1E	MEM	I += Vx	Adds VX to I.VF is not affected.[c]
      //   FX29	MEM	I = sprite_addr[Vx]	Sets I to the location of the sprite for the character in VX.Characters 0 - F(in hexadecimal) are represented by a 4x5 font.
      //   FX33	BCD	set_BCD(Vx);
      //               *(I + 0) = BCD(3);
      //               *(I + 1) = BCD(2);
      //               *(I + 2) = BCD(1);
      //               Stores the binary - coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I + 1, and the ones digit at location I + 2.)
      //   FX55	MEM	reg_dump(Vx, &I)	Stores V0 to VX(including VX) in memory starting at address I.The offset from I is increased by 1 for each value written, but I itself is left unmodified.[d]
      //   FX65	MEM	reg_load(Vx, &I)	Fills V0 to VX(including VX) with values from memory starting at address I.The offset from I is increased by 1 for each value written, but I itself is left unmodified.[d]

   default:
      printf("Unknown opcode: 0x%X\n", opcode);

   }


   // Update timers
   if (delay_timer > 0)
      --delay_timer;

   if (sound_timer > 0) {
      if (sound_timer == 1)
         printf("BEEP!\n");
      --sound_timer;
   }




}
