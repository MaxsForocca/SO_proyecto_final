#ifndef RESOURCE_H
#define RESOURCE_H

#include "../process/process.h"

#define MAX_WAIT_QUEUE 10

typedef struct resource {
    int id;                     // ID del recurso
    int is_in_use;              // 0 = libre, 1 = ocupado
    process_t* owner;           // Proceso que posee el recurso
    int is_shared;              // 1 = compartido, 0 = exclusivo
    process_t* wait_queue[MAX_WAIT_QUEUE]; // Cola de espera
    int queue_size;             // Tamaño de la cola
} resource_t;

// Funciones para manejar recursos
void init_resource(resource_t* resource, int id, int is_shared);
int use_resource(process_t* process, resource_t* resource);
void release_resource(resource_t* resource);

#endif // RESOURCE_H
