#ifndef KERNEL_H
#define KERNEL_H

static inline void hlt() {
    __asm__("hlt");
}

#endif
