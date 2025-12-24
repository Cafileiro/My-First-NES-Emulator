#include <iostream>
#include <fstream>
#include <string>

// Program Counter point to the next byte to process in memory
unsigned short ProgramCounter;
// math and bitwise operations register
char A;
// Index and general purpose register X
char X;
// Index and general purpose register Y
char Y;

char* RAM = new char[0x800]; // 2KB internal RAM
char* ROM = new char[0x8000]; // 32KB rom RAM (for now, not all games fit in here)

char Filepath[] = "";

char Read (unsigned short address){

    if (address < 0x0800){ 
        return RAM[address];
    }
    else if (address >= 0x0800 && address <= 0x1FFF){
        address = address & 0x07FF; // mirror every 2KB
        //TODO: Mirroring is not working as expected, need to check why
        return address;
    }
        
    else if (address >= 0x8000){
        return ROM[address - 0x8000];
    }
    return 0;
}

void Reset(){
    char header[16];
    std::ifstream rom(Filepath, std::ios::binary);
    // We don't care about the header for now
    rom.read(header, 0x10); // read header
    rom.read(ROM, 0x8000); // read up to 32KB - 16 bytes(header) of ROM data
    char PCL = Read(0xFFFC);
    char PCH = Read(0xFFFD);
    ProgramCounter = (unsigned short)((PCH * 0x100) + PCL);
    rom.close();
}

int main() {
    
    Reset(); 
    std::cout << "Program Counter initialized to: " << std::hex << ProgramCounter << std::endl;
    return 0;
}