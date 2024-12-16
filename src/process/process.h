#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define PROCESS_READY      0
#define PROCESS_RUNNING    1
#define PROCESS_TERMINATED 2
#define PROCESS_WAITING    3  // Nuevo estado para procesos en espera

#define DEFAULT_QUANTUM    5  // Tiempo máximo de ejecución para cada proceso

typedef struct process {
    uint64_t pid;
    uint64_t state;
    uint64_t quantum_remaining;  // Quantum actual del proceso
    uint64_t priority;           // Prioridad del proceso
    void (*entry_point)();       // Función asociada al proceso
    struct process* next;        // Apunta al siguiente proceso (lista circular)
} process_t;

extern process_t* process_list;  // Declaración externa
extern process_t* current_process;  // Declaración externa

void create_process(void (*entry_point)(), uint64_t priority);
void init_scheduler();
void scheduler();

#endif

