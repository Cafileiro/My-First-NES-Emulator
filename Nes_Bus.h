#pragma once
#include <cstdint>
#include <iostream>
#include <array>


class Nes_Bus
{
private:
    
public:
    Nes_Bus();
    ~Nes_Bus();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);
    void initializeRom();

    std::array<uint8_t,0x800> ram{};
    std::array<uint8_t,16> rom_header{};
    std::array<uint8_t,0x8000> rom{};
};

