#ifndef STRING_H
#define STRING_H

#include <stdint.h>

#define STRING_MAX_SIZE 1024  // Puedes ajustar este límite según tus necesidades

uint64_t strlen(const char* str);
char* strncpy(char* dest, const char* src, uint64_t n);

#endif
