#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include "Nes_Bus.h"
#include "env.h"

Nes_Bus::Nes_Bus() {
    ram.fill(0x00);
    std::cout << "[BUS] Bus inicializado y memoria RAM limpia." << std::endl;
}

Nes_Bus::~Nes_Bus() {
    std::cout << "[BUS] Bus destruido." << std::endl;
}

void Nes_Bus::initializeRom(){
    std::ifstream file(NES_ROM_PATH, std::ios::binary);
    if(!file.is_open()){
        std::cout << "[ERROR] Cannot open rom \n";
    }else{
        file.read(reinterpret_cast<char*>(rom_header.data()), 0x10); // read header
        file.read(reinterpret_cast<char*>(rom.data()), 0x8000); // read up to 32KB - 16 bytes(header) of ROM data
    }
}

uint8_t Nes_Bus::read(uint16_t address){
    
    if (address <= 0x1FFF){
        std::cout << "[BUS] Reading address:" << std::hex << address << std::endl;
        return ram[address & 0x07FF];// mirror every 2KB
    }
    else if (address >= 0x8000){
        std::cout << "[BUS] Reading address:" << std::hex << address << std::endl;
        return rom[address - 0x8000];
    }
    std::cout << "[ERROR] Wrong address:" << std::hex << address << std::endl;
    return -1; // unhandled read
}

void Nes_Bus::write(uint16_t address, uint8_t data){
    if (address <= 0x1FFF){
        ram[address & 0x07FF] = data; // mirror every 2KB
        std::cout << "[BUS] Writing value: " << std::hex << (int)data << " to RAM at address: " << std::hex << address << std::endl;
    }
    else if (address >= 0x8000){
        std::cout << "[ERROR] Attempting to write to ROM at address: " << std::hex << address << " with value: " << std::hex << (int)data << std::endl;
    }
    else {
        std::cout << "[ERROR] Attempting to write to unhandled address: " << std::hex << address << " with value: " << std::hex << (int)data << std::endl;
    }
}
