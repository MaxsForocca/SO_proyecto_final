#include "memory.h"
#include "../bootloader/bootservices.h"
#include "../print/printf/printf.h"

// Estructura para representar un bloque de memoria libre
struct memory_block {
    uint64_t base;        // Dirección base del bloque de memoria
    uint64_t size;        // Tamaño del bloque de memoria
    struct memory_block *next;  // Puntero al siguiente bloque
};

// Lista de bloques de memoria libre
struct memory_block *free_memory_list = NULL;

// Estructura para la memoria de trabajo
struct uefi_memory_region working_memory = {
    .type = LIMINE_MEMMAP_BAD_MEMORY,
    .length = 0,
    .base = 0,
};

// Función para inicializar la memoria libre
void init_free_memory() {
    uint64_t entries = get_memory_map_entries();

    // Recorre las regiones de memoria del sistema y las agrega a la lista de bloques libres
    for (uint64_t i = 0; i < entries; i++) {
        uint64_t type = get_memory_map_type(i);
        uint64_t length = get_memory_map_length(i);
        uint64_t base = get_memory_map_base(i);

        // Solo agregamos las regiones de memoria "usables"
        if (type == LIMINE_MEMMAP_USABLE) {
            struct memory_block *block = (struct memory_block*)base;
            block->base = base;
            block->size = length;
            block->next = free_memory_list;
            free_memory_list = block;  // Añadir el bloque a la lista
        }
    }
}

// Función para asignar memoria (first-fit)
void* malloc(uint64_t size) {
    struct memory_block *current = free_memory_list;
    struct memory_block *prev = NULL;

    // Recorre la lista de bloques libres
    while (current != NULL) {
        // Si encontramos un bloque suficientemente grande
        if (current->size >= size) {
            // Si el bloque es más grande de lo necesario, lo dividimos
            if (current->size > size + sizeof(struct memory_block)) {
                struct memory_block *new_block = (struct memory_block*)(current->base + size);
                new_block->base = current->base + size;
                new_block->size = current->size - size;
                new_block->next = current->next;

                // Ajusta el bloque actual
                current->size = size;
                current->next = new_block;
            }

            // Elimina el bloque de la lista de bloques libres
            if (prev == NULL) {
                free_memory_list = current->next;
            } else {
                prev->next = current->next;
            }

            // Retorna la dirección de inicio del bloque asignado
            return (void*)current->base;
        }

        prev = current;
        current = current->next;
    }

    return NULL;  // No hay suficiente memoria
}

// Función para liberar memoria
void free(void* ptr) {
    struct memory_block *block = (struct memory_block*)ptr;

    // Añade el bloque de vuelta a la lista de bloques libres
    block->next = free_memory_list;
    free_memory_list = block;
}

// Función para inicializar la memoria del sistema
void init_memory() {
    printf("Cargando memoria\n");

    uint64_t entries = get_memory_map_entries();

    printf("Se detectaron %d regiones de memoria\n", entries);

    // Inicializa la lista de bloques libres
    init_free_memory();

    uint64_t total_memory = 0;
    uint64_t total_avail_memory = 0;

    // Recorre las regiones de memoria y calcula la memoria total y disponible
    for (uint64_t i = 0; i < entries; i++) {
        uint64_t type = get_memory_map_type(i);
        uint64_t length = get_memory_map_length(i);
        uint64_t base = get_memory_map_base(i);

        printf("Memory region %d base=%x length=%x type=%d\n", i, base, length, type);

        total_memory += length;
        if (type == LIMINE_MEMMAP_USABLE) {
            total_avail_memory += length;
            if (length > working_memory.length) {
                working_memory.length = length;
                working_memory.type = type;
                working_memory.base = base;
            }
        }
    }

    printf("Memoria total: %x, memoria disponible: %x\n", total_memory, total_avail_memory);
    printf("Memoria de trabajo [base=%x, tamaño=%x, tipo=%x]\n", working_memory.base, working_memory.length, working_memory.type);
}