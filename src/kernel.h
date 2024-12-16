#ifndef KERNEL_H
#define KERNEL_H

void test_process_1();
void test_process_2();
void test_process_3();
void test_process_4();
static inline void hlt() {
    __asm__("hlt");
}

#endif
