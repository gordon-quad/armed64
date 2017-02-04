MEMORY
{
    rom (rx) : ORIGIN = 0x08005000, LENGTH = 108K
    ram (rwx) : ORIGIN = 0x20000c00, LENGTH = 17K
}
INCLUDE libopencm3_stm32f1.ld

