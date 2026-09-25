#include <iostream>
#include <string>
#include <cstdint>
#include <time.h>
#include <thread>
#include "Nes_CPU.h"
#include "Nes_Bus.h"

Nes_CPU::Nes_CPU(){
    programCounter = 0x0000;
    A = X = Y = 0x00;
    std::cout << "[CPU] CPU inicializada y registros limpios." << std::endl;
}

Nes_CPU::~Nes_CPU() {
    m_bus = nullptr;
    std::cout << "[CPU] Instancia destruida." << std::endl;
}

uint8_t Nes_CPU::read(uint16_t address) {
    return m_bus->read(address);
}

void Nes_CPU::write(uint16_t address, uint8_t data) {
    m_bus->write(address, data);
}


void Nes_CPU::reset(){
    tmp_high = m_bus->read(0xFFFD);
    tmp_low = m_bus->read(0xFFFC);
    programCounter = (uint16_t)((tmp_high << 8) | tmp_low);

    A = 0;
    X = 0;
    Y = 0;
    subcycle = 0;
    cpuHalted = false;

}

void Nes_CPU::connectRom(){
    m_bus->initializeRom();
}

void Nes_CPU::clock(){
    
    if (subcycle == 0) {
        opcode = read(programCounter++);
        std::cout << "[DEBUG] fecth opcode: " << std::hex << (uint16_t)opcode << std::endl;

        subcycle = 1;
        return;
    }
    std::cout << "[DEBUG] procesing opcode: " << std::hex << (uint16_t)opcode << " with subcycle: " << +subcycle << std::endl;
    switch (opcode)
    {
    
    case  0x02: // halt jeje

        cpuHalted=true;
        break;
    
    case 0xA0: //LDY Inmediate

        switch (subcycle)
        {
        case 1:
            Y = read(programCounter++);
            subcycle = 0;
            break;
        }
        break;

    case 0xA2: //LDX Inmediate
    
        switch (subcycle)
            {
            case 1:
                X = read(programCounter++);
                subcycle = 0;
                break;
            }
            break;
    
    case 0xA9: //LDA Inmediate
        
        switch (subcycle)
        {
        case 1:
            A = read(programCounter++);
            subcycle = 0;
            break; 
        }
        break;
            Y = read(programCounter++);
            subcycle = 0;

    default:
        std::cout << "[ERROR] opcode not handled :" << std::hex << (uint16_t) opcode << std::endl;
        break;
    }
}




