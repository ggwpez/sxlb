#include "cpu_port.hpp"

namespace system
{
    uint8_t inb(uint16_t port)
    {
        unsigned char ret;
        asm volatile("inb %%dx,%%al":"=a" (ret) : "d" (port));
        return ret;
    };

    uint16_t inw(uint16_t port)
    {
        unsigned char ret;
        asm volatile("inw %%dx,%%ax":"=a" (ret) : "d" (port));
        return ret;
    };

    uint32_t ind(uint16_t port)
    {
        unsigned char ret;
        asm volatile("in %%dx,%%eax":"=a" (ret) : "d" (port));
        return ret;
    };

    void outb(uint16_t port, uint8_t value)
    {
        asm volatile("outb %%al,%%dx": : "d" (port), "a" (value));
    };

    void outw(uint16_t port, uint16_t value)
    {
        asm volatile("outw %%ax,%%dx": : "d" (port), "a" (value));
    };

    void outd(uint16_t port, uint32_t value)
    {
        asm volatile("out %%eax,%%dx": : "d" (port), "a" (value));
    };
}
