#ifndef RESOURCE_H
#define RESOURCE_H

#include "../process/process.h"

typedef struct resource {
    uint64_t id;
    uint64_t is_in_use;   // 1 si está en uso, 0 si está disponible
    process_t* owner;     // Proceso que tiene el recurso
} resource_t;

void use_resource(process_t* process, resource_t* resource);
void release_resource(resource_t* resource);

#endif
