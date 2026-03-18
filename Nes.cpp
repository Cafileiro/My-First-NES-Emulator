#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include "env.h"   

void Run();
void Reset();
void emulateCpu();
u_int8_t Read(u_int16_t address);
void Write(u_int16_t Address, u_int8_t Value);


u_int16_t ProgramCounter;   // Program Counter point to the next byte to process in memory

u_int8_t A; // math and bitwise operations register
u_int8_t X; // Index and general purpose register X
u_int8_t Y; // Index and general purpose register Y
u_int8_t* RAM = new u_int8_t[0x800];    // 2KB internal RAM
u_int8_t* ROM = new u_int8_t[0x8000];   // 32KB rom RAM (for now, not all games fit in here)
u_int8_t opcode; // current opcode being executed

char* Filepath = NES_ROM_PATH;  // path to the .nes file to load

bool Cpu_halted = false;   

int cycle = 0;

/**
 * Read function to read from memory. It takes an address as input and returns the value at that address.
 * Handles mirroring for RAM and ROM.
 * @param address The address to read from in 16 bit format
 * @return The value at the specified address, or -1 if the address is not handled
 */

u_int8_t Read(u_int16_t address){

    if (address <= 0x1FFF){
        return RAM[address & 0x07FF];// mirror every 2KB
    }
    else if (address >= 0x8000){
        return ROM[address - 0x8000];
    }
    return -1; // unhandled read
}
/**
 * Reset function to initialize the CPU and load the ROM. 
 * It reads the header of the ROM file, loads the ROM data into memory, and sets the program counter to the address specified in the reset vector (0xFFFC and 0xFFFD).
 * It then starts the CPU emulation by calling the Run function.
 */
void Reset(){
    
    u_int8_t header[16];
    
    std::ifstream rom(Filepath, std::ios::binary);
    // We don't care about the header for now
    
    rom.read(reinterpret_cast<char*>(header), 0x10); // read header
    rom.read(reinterpret_cast<char*>(ROM), 0x8000); // read up to 32KB - 16 bytes(header) of ROM data
    
    u_int8_t PCL = Read(0xFFFC);
    u_int8_t PCH = Read(0xFFFD);
    
    ProgramCounter = (u_int16_t)((PCH * 0x100) + PCL);
    Cpu_halted = false;
    Run();
    rom.close();
}

/**
 * Run function to start the CPU emulation. It continuously calls the emulateCpu function until the CPU is halted.
 */

void Run(){
    while(Cpu_halted == false){
        emulateCpu();
    }
}
/**
 * Emulate CPU function to execute instructions. It reads the opcode at the current program counter, decodes it, and executes the corresponding instruction. 
 * For now, it only handles a few opcodes (LDA, LDY, LDX, and HTL). It also prints the opcode being executed and the address for debugging purposes.
 * 
 */

void emulateCpu(){
    
    if(cycle == 0)
    {
        opcode = Read(ProgramCounter);
        ProgramCounter++;
        cycle++;
        std ::cout << "Executing opcode: " << std::hex << (int)opcode << " at address: " << std::hex << (u_int16_t)ProgramCounter << std::endl; 
    }
    else 
    {
        
        switch(opcode)
        {

            
            
            case 0x02: //HTL - Halt CPU 1 cycle
                Cpu_halted = true;
                cycle = 0;
                break;
            
            case 0xA9: // LDA Immediate 2 cycles
                A = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;
                cycle = 0;
                break;
        
            case 0xA0: // LDY Immediate 2 cycles
                Y = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;
                cycle = 0;
                break;
            
            case 0xA2: // LDX Immediate 2 cycles
                X = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;
                cycle = 0;
                break;
            
            case 0xA5: // LDA Zero Page 3 cycles
                u_int8_t adress = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                u_int8_t value = Read(adress);
                cycle++;

                A = value;
                cycle = 0;
                break;

            case 0xAD : // LDA Absolute 4 cycles
                u_int8_t lowByte = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                u_int8_t highByte = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                u_int16_t address =  highByte * 256 + lowByte;
                u_int16_t value = Read(address);
                cycle++;
                A = value;
                cycle = 0;
                break;

            case 0x84 : // STY Zero Page 3 cycles
                u_int8_t temp = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                Write(temp, Y);
                cycle++;
                
                cycle = 0;
                break;

            case 0x85 : // STA Zero Page 3 cycles
                u_int8_t temp = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;
                Write(temp, A);
                cycle++;
                cycle = 0;
                break;

            case 0x86 : // STX Zero Page 3 cycles
                u_int8_t temp = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                Write(temp, X);
                cycle++;
                
                cycle = 0;
                break;

            case 0x8C : // STY Absolute 4 cycles
                u_int8_t lowByte = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                u_int8_t highByte = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                u_int16_t address =  highByte * 256 + lowByte;
                Write(address, Y);
                cycle++;

                cycle = 0;
                break;

            case 0x8D : // STA Absolute 4 cycles
                u_int8_t lowByte = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                u_int8_t highByte = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                u_int16_t address =  highByte * 256 + lowByte;
                Write(address, A);
                cycle++;
                cycle = 0;
                break;

            case 0x8E : // STX Absolute 4 cycles
                u_int8_t lowByte = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                u_int8_t highByte = Read(ProgramCounter);
                cycle++;
                ProgramCounter++;

                u_int16_t address =  highByte * 256 + lowByte;
                Write(address, X);
                cycle++;

                cycle = 0;
                break;
                
            default:
                std::cout << "Unhandled opcode: " << std::hex << (int)opcode << " at address: " << std::hex << (uint16_t)(ProgramCounter - 1) << std::endl;
                Cpu_halted = true;
                break;
        }
    
    } 
    
}

void Write(u_int16_t Address, u_int8_t Value){
    if (Address <= 0x1FFF){
        RAM[Address & 0x07FF] = Value; // mirror every 2KB
        std::cout << "Writing value: " << std::hex << (int)Value << " to RAM at address: " << std::hex << Address << std::endl;
    }
    else if (Address >= 0x8000){
        std::cout << "Error: Attempting to write to ROM at address: " << std::hex << Address << " with value: " << std::hex << (int)Value << std::endl;
    }
    else {
        std::cout << "Error: Attempting to write to unhandled address: " << std::hex << Address << " with value: " << std::hex << (int)Value << std::endl;
    }
}

int main() {
    
    Reset(); 
    std::cout << "A Final value: " << std::hex << (uint16_t)A << std::endl;
    std::cout << "X Final value: " << std::hex << (uint16_t)X << std::endl;
    std::cout << "Y Final value: " << std::hex << (uint16_t)Y << std::endl;

    return 0;
}