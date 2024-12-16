#include "resource.h"
#include "../print/printf/printf.h"

void use_resource(process_t* process, resource_t* resource) {
    if (resource->is_in_use) {
        printf("Process PID=%lu is waiting for resource %lu\n", process->pid, resource->id);
        process->state = PROCESS_WAITING;  // Cambiar el estado del proceso
        return;
    }

    resource->is_in_use = 1;
    resource->owner = process;
    printf("Process PID=%lu acquired resource %lu\n", process->pid, resource->id);
}

void release_resource(resource_t* resource) {
    if (!resource->is_in_use) {
        return;
    }

    printf("Resource %lu released by process PID=%lu\n", resource->id, resource->owner->pid);
    resource->is_in_use = 0;
    resource->owner->state = PROCESS_READY;  // Desbloquear el proceso
    resource->owner = NULL;
}
