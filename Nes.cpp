#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>


void Run();
void Reset();
void emulateCpu();

// Program Counter point to the next byte to process in memory
u_int16_t ProgramCounter;
// math and bitwise operations register
u_int8_t A;
// Index and general purpose register X
u_int8_t X;
// Index and general purpose register Y
u_int8_t Y;

u_int8_t* RAM = new u_int8_t[0x800]; // 2KB internal RAM
u_int8_t* ROM = new u_int8_t[0x8000]; // 32KB rom RAM (for now, not all games fit in here)

char Filepath[] = "/home/rafael/Documentos/My-First-NES-Emulator/__PatreonRoms/1_Example.nes";

bool Cpu_halted = false;   

u_int8_t Read (u_int16_t address){

    if (address <= 0x1FFF){
        return RAM[address & 0x07FF];// mirror every 2KB
    }
    else if (address >= 0x8000){
        return ROM[address - 0x8000];
    }
    return -1; // unhandled read
}

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

void Run(){
    while(Cpu_halted == false){
        emulateCpu();
    }
}
void emulateCpu(){
    u_int8_t opcode = Read(ProgramCounter);
    std ::cout << "Executing opcode: " << std::hex << (int)opcode << " at address: " << std::hex << (u_int16_t)ProgramCounter << std::endl;
    ProgramCounter++;
    switch(opcode){
        case 0x02: //HTL - Halt CPU
            Cpu_halted = true;
            break;
        case 0xA9: // LDA Immediate
            A = Read(ProgramCounter);
            ProgramCounter++;
            break;
        case 0xA0: // LDY Immediate
            Y = Read(ProgramCounter);
            ProgramCounter++;
            break;
        case 0xA2: // LDX Immediate
            X = Read(ProgramCounter);
            ProgramCounter++;
            break;
        default:
            std::cout << "Unhandled opcode: " << std::hex << (int)opcode << " at address: " << std::hex << (uint16_t)(ProgramCounter - 1) << std::endl;
            Cpu_halted = true;
            break;
    }
}


int main() {
    
    Reset(); 
    std::cout << "A Final value: " << std::hex << (uint16_t)A << std::endl;
    std::cout << "X Final value: " << std::hex << (uint16_t)X << std::endl;
    std::cout << "Y Final value: " << std::hex << (uint16_t)Y << std::endl;

    return 0;
}