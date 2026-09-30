#include <cstdint>
#include "Nes_Bus.h"

class Nes_CPU
{

private:
    Nes_Bus* m_bus = nullptr;    

    // bus comunication
    uint8_t read(uint16_t address);
    void    write(uint16_t address, uint8_t data);

    void pushStack(uint8_t data);
    uint8_t pullStack();

    // Innerstates
    uint8_t opcode = 0x00;
    uint8_t subcycle = 0;

    // Temporals register, persistents between subcycles
    uint8_t  tmp_low = 0x00;
    uint8_t  tmp_high = 0x00;
    uint16_t tmp_addr = 0x0000;
    uint8_t stack_pointer = 0x00;

    bool flag_Carry;
    bool flag_Zero;
    bool flag_InterruptDisable;
    bool flag_Decimal;
    bool flag_Overflow;
    bool flag_Negative;

    void update_flags(uint8_t data);

public:

    Nes_CPU();
    ~Nes_CPU();
    //Connect the bus to the CPU
    void connectBus( Nes_Bus* bus ) { m_bus = bus;}
    void connectRom();
    //Main functions that runs only one clocks cycle
    void clock();
    void reset();


    uint16_t programCounter;   // Program Counter point to the next byte to process in memory
    uint8_t A; // math and bitwise operations register
    uint8_t X; // Index and general purpose register X
    uint8_t Y; // Index and general purpose register Y 
    //uint8_t Status;
    bool cpuHalted = false;   
};
