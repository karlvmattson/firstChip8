#pragma once




class chip8 {

   public:
      chip8();
      ~chip8();
      void initialize();
      void loadGame();
      void emulateCycle();
      void setKeys();
      bool drawFlag;



   private:
      unsigned char gfx[32 * 64];   // 1 for pixel on, 0 for off

      unsigned short opcode;        // current opcode
      unsigned char memory[4096];   // system memory
      unsigned char V[16];          // registers
      unsigned short I;             // index
      unsigned short pc;            // program counter

      unsigned char delay_timer;    // delay timer, 60Hz
      unsigned char sound_timer;    // buzzer timer, 60Hz, buzzes when timer reaches 0

      unsigned short stack[16];     // stack to store values before a jump command
      unsigned short sp;            // point to remember which level of the stack is used

      unsigned char key[16];        // current state of each key on the keypad

};




