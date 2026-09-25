#include "Nes_Bus.h"
#include "Nes_CPU.h"

int main() {
    
    Nes_Bus bus;
    Nes_CPU cpu;

    cpu.connectBus(&bus);
    cpu.connectRom();
    cpu.reset(); // set the cpu ready

    while(cpu.cpuHalted == false){
        std::cout << "[MAIN] New clock cycle." << std::endl;
        cpu.clock();
        std::cout << "[DEBUG] A current value: " << std::hex << (uint16_t)cpu.A << std::endl;
        std::cout << "[DEBUG] X current value: " << std::hex << (uint16_t)cpu.X << std::endl;
        std::cout << "[DEBUG] Y current value: " << std::hex << (uint16_t)cpu.Y << std::endl;

    }
    std::cout << "[DEBUG] A Final value: " << std::hex << (uint16_t)cpu.A << std::endl;
    std::cout << "[DEBUG] X Final value: " << std::hex << (uint16_t)cpu.X << std::endl;
    std::cout << "[DEBUG] Y Final value: " << std::hex << (uint16_t)cpu.Y << std::endl;

    return 0;
}