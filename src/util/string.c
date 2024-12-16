#include "string.h"

uint64_t strlen(const char* str) {
    uint64_t len = 0;
    while (str[len] && len < STRING_MAX_SIZE)
        len++;
    return len;
}

char* strncpy(char* dest, const char* src, uint64_t n) {
    uint64_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}
