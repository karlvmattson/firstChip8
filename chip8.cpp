// Code samples copied from Laurence Muller's writeup at https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
// Opcode descriptions copied from wikipedia article on the Chip8 Interpreter

#pragma once
#include <chip8.h>
#include <string>
#include <math.h>
#include <SDL.h>
#include <iostream>
#include <Windows.h>
using namespace std;


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

   //// test pattern
   //for (int i = 0; i < 32; i++) {
   //   gfx[i * 64 + i] = 1;
   //   
   //}
   //drawFlag = true;

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

bool chip8::loadApplication(string filename)
{
   initialize();
   printf("Loading: %s\n", filename);

   // Open file
   char* char_arr;
   string str_obj("./roms/" + filename);
   char_arr = &str_obj[0];

#pragma warning(suppress : 4996)
   FILE* pFile = fopen(char_arr, "rb");
   if (pFile == NULL)
   {
      fputs("File error", stderr);
      return false;
   }

   // Check file size
   fseek(pFile, 0, SEEK_END);
   long lSize = ftell(pFile);
   rewind(pFile);
   printf("Filesize: %d\n", (int)lSize);

   // Allocate memory to contain the whole file
   char* buffer = (char*)malloc(sizeof(char) * lSize);
   if (buffer == NULL)
   {
      fputs("Memory error", stderr);
      return false;
   }

   // Copy the file into the buffer
   size_t result = fread(buffer, 1, lSize, pFile);
   if (result != lSize)
   {
      fputs("Reading error", stderr);
      return false;
   }

   // Copy buffer to Chip8 memory
   if ((4096 - 512) > lSize)
   {
      for (int i = 0; i < lSize; ++i)
         memory[i + 512] = buffer[i];
   }
   else
      printf("Error: ROM too big for memory");

   // Close file, free buffer
   fclose(pFile);
   free(buffer);

   return true;
}

void chip8::setKeys() {

   //Keypad                   Keyboard
//	+ -+-+-+-++-+-+-+-+
//	| 1 | 2 | 3 | C|                |1 | 2 | 3 | 4 |
//	+-+-+-+-++-+-+-+-+
//	| 4 | 5 | 6 | D|                |Q | W | E | R |
//	+-+-+-+-+= > +-+-+-+-+
//	| 7 | 8 | 9 | E|                |A | S | D | F |
//	+-+-+-+-++-+-+-+-+
//	| A | 0 | B | F|                |Z | X | C | V |
//	+-+-+-+-++-+-+-+-+


   const Uint8* state = SDL_GetKeyboardState(NULL);
   if (state[SDL_SCANCODE_X]) key[0x0] = 1;
   else key[0x0] = 0;
   if (state[SDL_SCANCODE_1]) key[0x1] = 1;
   else key[0x1] = 0;
   if (state[SDL_SCANCODE_2]) key[0x2] = 1;
   else key[0x2] = 0;
   if (state[SDL_SCANCODE_3]) key[0x3] = 1;
   else key[0x3] = 0;
   if (state[SDL_SCANCODE_Q]) key[0x4] = 1;
   else key[0x4] = 0;
   if (state[SDL_SCANCODE_W]) key[0x5] = 1;
   else key[0x5] = 0;
   if (state[SDL_SCANCODE_E]) key[0x6] = 1;
   else key[0x6] = 0;
   if (state[SDL_SCANCODE_A]) key[0x7] = 1;
   else key[0x7] = 0;
   if (state[SDL_SCANCODE_S]) key[0x8] = 1;
   else key[0x8] = 0;
   if (state[SDL_SCANCODE_D]) key[0x9] = 1;
   else key[0x9] = 0;
   if (state[SDL_SCANCODE_Z]) key[0xA] = 1;
   else key[0xA] = 0;
   if (state[SDL_SCANCODE_C]) key[0xB] = 1;
   else key[0xB] = 0;
   if (state[SDL_SCANCODE_4]) key[0xC] = 1;
   else key[0xC] = 0;
   if (state[SDL_SCANCODE_R]) key[0xD] = 1;
   else key[0xD] = 0;
   if (state[SDL_SCANCODE_F]) key[0xE] = 1;
   else key[0xE] = 0;
   if (state[SDL_SCANCODE_V]) key[0xF] = 1;
   else key[0xF] = 0;

}

unsigned char* chip8::getGfx()
{
   return gfx;
}

void chip8::emulateCycle() {
   // Fetch Opcode
   opcode = memory[pc] << 8 | memory[pc + 1];
   cout << opcode << "\n";

   // Decode Opcode
   switch (opcode & 0xF000) {
      //   0NNN	Call		Calls machine code routine(RCA 1802 for COSMAC VIP) at address NNN.Not necessary for most ROMs.

   case 0x0000:
      switch (opcode & 0x00FF) {
      case 0x00E0:   //   00E0	Display	disp_clear()	Clears the screen.
         memset(gfx, 0, sizeof(gfx));
         drawFlag = true;
         pc += 2;
         break;
         
      case 0x00EE:   //   00EE	Flow	return;	Returns from a subroutine.
         --sp;
         pc = stack[sp];
         pc += 2;
         break;

      default:
         printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
      }
   break;

   case 0x1000:      //   1NNN	Flow	goto NNN;	Jumps to address NNN.
      pc = (opcode & 0x0FFF);
      break;

   case 0x2000:      //   2NNN	Flow * (0xNNN)()	Calls subroutine at NNN.
      stack[sp] = pc; // add current location to the stack trace
      ++sp;           // increment stack pointer
      pc = opcode & 0x0FFF;
      break;

   case 0x3000:      //   3XNN	Cond	if (Vx == NN)	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
      if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
         pc += 4;
      else
         pc += 2;
      break;

   case 0x4000:      //   4XNN	Cond	if (Vx != NN)	Skips the next instruction if VX does not equal NN. (Usually the next instruction is a jump to skip a code block)
      if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
         pc += 4;
      else
         pc += 2;
      break;

   case 0x5000:      //   5XY0	Cond	if (Vx == Vy)	Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
      if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
         pc += 4;
      else
         pc += 2;
      break;

   case 0x6000:      //   6XNN	Const	Vx = NN	Sets VX to NN.
      V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
      pc += 2;
      break;

   case 0x7000:      //   7XNN	Const	Vx += NN	Adds NN to VX. (Carry flag is not changed)
      V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
      pc += 2;
      break;

   case 0x8000:
      switch (opcode & 0x000F) {
      case 0x0000:      //   8XY0	Assign	Vx = Vy	Sets VX to the value of VY.
         V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
         pc += 2;
         break;

      case 0x0001:      //   8XY1	BitOp	Vx = Vx | Vy	Sets VX to VX or VY. (Bitwise OR operation)
         V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
         pc += 2;
         break;

      case 0x0002:      //   8XY2	BitOp	Vx = Vx & Vy	Sets VX to VX and VY. (Bitwise AND operation)
         V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
         pc += 2;
         break;

      case 0x0003:      //   8XY3[a]	BitOp	Vx = Vx ^ Vy	Sets VX to VX xor VY.
         V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
         pc += 2;
         break;

      case 0x0004:   //   8XY4	Math	Vx += Vy	Adds VY to VX.VF is set to 1 when there's a carry, and to 0 when there is not.
         if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
            V[0xF] = 1; // carry
         else
            V[0xF] = 0;
         V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
         pc += 2;
         break;

      case 0x0005:      //   8XY5	Math	Vx -= Vy	VY is subtracted from VX.VF is set to 0 when there's a borrow, and 1 when there is not.
         if (V[(opcode & 0x00F0) >> 4] > (V[(opcode & 0x0F00) >> 8]))
            V[0xF] = 0; // carry
         else
            V[0xF] = 1;
         V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
         pc += 2;
         break;

      case 0x0006:      //   8XY6[a]	BitOp	Vx >>= 1	Stores the least significant bit of VX in VF and then shifts VX to the right by 1.[b]
         V[0xF] = (0x01 & V[(opcode & 0x0F00) >> 8]);
         V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] >> 1;
         pc += 2;
         break;

      case 0x0007:      //   8XY7[a]	Math	Vx = Vy - Vx	Sets VX to VY minus VX.VF is set to 0 when there's a borrow, and 1 when there is not.
         if (V[(opcode & 0x00F0) >> 4] < (V[(opcode & 0x0F00) >> 8]))
            V[0xF] = 0; // carry
         else
            V[0xF] = 1;
         V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
         pc += 2;
         break;

      case 0x000E:      //   8XYE[a]	BitOp	Vx <<= 1	Stores the most significant bit of VX in VF and then shifts VX to the left by 1.[b]
         V[0xF] = (0x80 & V[(opcode & 0x0F00) >> 8]);
         V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
         pc += 2;
         break;
      

      default:
         printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
         break;
      }

   case 0x9000:      //   9XY0	Cond	if (Vx != Vy)	Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block)
      if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
         pc += 4;
      else
         pc += 2;
      break;

   case 0xA000:      //   ANNN	MEM	I = NNN	Sets I to the address NNN.
      // Execute opcode
      I = opcode & 0x0FFF;
      pc += 2;
      break;

   case 0xB000:      //   BNNN	Flow	PC = V0 + NNN	Jumps to the address NNN plus V0.
      pc = V[0] + (opcode & 0x0FFF);
      break;

   case 0xC000:      //   CXNN	Rand	Vx = rand() & NN	Sets VX to the result of a bitwise and operation on a random number(Typically: 0 to 255) and NN.
   {
      unsigned int random = rand() % 256;
      V[(opcode & 0x0F00) >> 8] = random & (opcode & 0x00FF);
      pc += 2;
   }
      break;      

   case 0xD000: {   //   DXYN	Disp	draw(Vx, Vy, N)	Draws a sprite at coordinate(VX, VY) that has a width of 8 pixels and a height of N + 1 pixels.Each row of 8 pixels is read as bit - coded starting from memory location I; I value does not change after the execution of this instruction.As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen
      unsigned short x = V[(opcode & 0x0F00) >> 8];
      unsigned short y = V[(opcode & 0x00F0) >> 4];
      unsigned short height = opcode & 0x000F;
      unsigned short pixel;

      V[0xF] = 0; // start with VF set to zero
      for (int yline = 0; yline < height; yline++) {
         pixel = memory[I + yline];
         for (int xline = 0; xline < 8; xline++) {  // sprites are always 8 pixels wide
            if ((pixel & (0x80 >> xline)) != 0) {
               if (gfx[(x + xline + ((y + yline) * 64))] == 1)
                  V[0xF] = 1;
               gfx[x + xline + ((y + yline) * 64)] ^= 1;
            }
         }
      }

      drawFlag = true;
      pc += 2;
      break;
   }
          
   case 0xE000:
      switch (opcode & 0x00FF) {
      case 0x009E:   //   EX9E	KeyOp	if (key() == Vx)	Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
         if (key[V[(opcode & 0x0F00) >> 8]] != 0)
            pc += 4;
         else
            pc += 2;
         break;
         
      case 0x00A1:      //   EXA1	KeyOp	if (key() != Vx)	Skips the next instruction if the key stored in VX is not pressed. (Usually the next instruction is a jump to skip a code block)
         if (key[V[(opcode & 0x0F00) >> 8]] == 0)
            pc += 4;
         else
            pc += 2;
         break;

      default:
         printf("Unknown opcode [0xE000]: 0x%X\n", opcode);
      break;
      }

   case 0xF000:
      switch (opcode & 0x00FF) {

      case 0x0007:      //   FX07	Timer	Vx = get_delay()	Sets VX to the value of the delay timer.
         V[(opcode & 0x0F00) >> 8] = delay_timer;
         pc += 2;
      break;

      case 0x000A:  {    //   FX0A	KeyOp	Vx = get_key()	A key press is awaited, and then stored in VX. (Blocking Operation.All instruction halted until next key event)
         bool waiting = true;
         while (waiting) {
            // copy old state of keyboard
            unsigned char old_key[16];
            for (int i = 0; i < 16; i++) {
               old_key[i] = key[i];
            }

            // get new state of keyboard
            setKeys();

            // compare new state with old and exit if a new key was pressed
            for (int i = 0; i < 16; i++) {
               if (old_key[i] < key[i]) waiting = false;
            }
         }
         pc += 2;
      }
      break;
                        
      case 0x0015:      //   FX15	Timer	delay_timer(Vx)	Sets the delay timer to VX.
         delay_timer = (opcode & 0x0F00) >> 8;
         pc += 2;
      break;

      case 0x0018:      //   FX18	Sound	sound_timer(Vx)	Sets the sound timer to VX.
         sound_timer = (opcode & 0x0F00) >> 8;
         pc += 2;
      break;

      case 0x001E:      //   FX1E	MEM	I += Vx	Adds VX to I.VF is not affected.[c]
         I += (opcode & 0x0F00) >> 8;
         pc += 2;
      break;

      case 0x0029:      //   FX29	MEM	I = sprite_addr[Vx]	Sets I to the location of the sprite for the character in VX.Characters 0 - F(in hexadecimal) are represented by a 4x5 font.
         I = ((opcode & 0x0F00) >> 8) * 0x5;
         pc += 2;
      break;
         
      case 0x0033:      //   FX33	BCD	set_BCD(Vx);
         //               *(I + 0) = BCD(3);
         //               *(I + 1) = BCD(2);
         //               *(I + 2) = BCD(1);
         //               Stores the binary - coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I + 1, and the ones digit at location I + 2.)
         memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
         memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
         memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
         pc += 2;
      break;

      case 0x0055:  {    //   FX55	MEM	reg_dump(Vx, &I)	Stores V0 to VX(including VX) in memory starting at address I.The offset from I is increased by 1 for each value written, but I itself is left unmodified.[d]
         int start_address = I;
         int end_reg = (opcode & 0x0F00) >> 8;
         for (int i = 0; i <= end_reg; i++) {
            memory[start_address] = V[end_reg];
            start_address++;
         }
         pc += 2;
      }
      break;

      case 0x0065: {     //   FX65	MEM	reg_load(Vx, &I)	Fills V0 to VX(including VX) with values from memory starting at address I.The offset from I is increased by 1 for each value written, but I itself is left unmodified.[d]
         int start_address = I;
         int end_reg = (opcode & 0x0F00) >> 8;
         for (int i = 0; i <= end_reg; i++) {
            V[end_reg] = memory[start_address];
            start_address++;
         }
         pc += 2;
      }
      break;
      
      default:
         printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
      break;
      
      }
      break;

   default:
      printf("Unknown opcode: 0x%X\n", opcode);
   }

   // Update timers
   if (delay_timer > 0)
      --delay_timer;

   if (sound_timer > 0) {
      if (sound_timer == 1)
         Beep(523, 200);
      --sound_timer;
   }
}
